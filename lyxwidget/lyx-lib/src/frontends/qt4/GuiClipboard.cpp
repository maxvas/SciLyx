// -*- C++ -*-
/**
 * \file qt4/GuiClipboard.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "FileDialog.h"

#include "support/FileName.h"
#include "GuiClipboard.h"
#include "qt_helpers.h"

#include "Buffer.h"
#include "BufferView.h"
#include "Cursor.h"

#include "support/lassert.h"
#include "support/convert.h"
#include "support/debug.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/lstrings.h"
#include "support/lyxtime.h"

#ifdef Q_OS_MAC
#include "support/linkback/LinkBackProxy.h"
#endif // Q_OS_MAC

#include "frontends/alert.h"

#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDataStream>
#include <QFile>
#include <QImage>
#include <QMimeData>
#include <QString>
#include <QStringList>
#include <QTextDocument>
#include <QVariant>

#include <boost/crc.hpp>

#include <memory>
#include <map>
#include <iostream>

using namespace std;
using namespace lyx::support;


namespace lyx {

namespace frontend {

static QMimeData const * read_clipboard() 
{
	LYXERR(Debug::ACTION, "Getting Clipboard");
	QMimeData const * source =
		qApp->clipboard()->mimeData(QClipboard::Clipboard);
	if (!source) {
		LYXERR0("0 bytes (no QMimeData)");
		return new QMimeData;
	}
	// It appears that doing IO between getting a mimeData object
	// and using it can cause a crash (maybe Qt used IO
	// as an excuse to free() it? Anyway let's not introduce
	// any new IO here, so e.g. leave the following line commented.
	// lyxerr << "Got Clipboard (" << (long) source << ")\n" ;
	return source;
}


void CacheMimeData::update()
{
	time_t const start_time = current_time();
	LYXERR(Debug::ACTION, "Creating CacheMimeData object");
	cached_formats_ = read_clipboard()->formats();

	// Qt times out after 5 seconds if it does not recieve a response.
	if (current_time() - start_time > 3) {
		LYXERR0("No timely response from clipboard, perhaps process "
			<< "holding clipboard is frozen?");
	}
}


QByteArray CacheMimeData::data(QString const & mimeType) const 
{
	return read_clipboard()->data(mimeType);
}

QImage CacheMimeData::image() const
{
    return qvariant_cast<QImage>(read_clipboard()->imageData());
}


QString const lyxMimeType(){ return "application/x-lyx"; }
QString const texMimeType(){ return "text/x-tex"; }
QString const latexMimeType(){ return "application/x-latex"; }
QString const pdfMimeType(){ return "application/pdf"; }
QString const emfMimeType(){ return "image/x-emf"; }
QString const wmfMimeType(){ return "image/x-wmf"; }
QString const filesType(){ return "text/uri-list"; }


GuiClipboard::GuiClipboard()
{
	connect(qApp->clipboard(), SIGNAL(dataChanged()),
		this, SLOT(on_dataChanged()));
	// initialize clipboard status.
	on_dataChanged();
}


string const GuiClipboard::getAsLyX() const
{
	LYXERR(Debug::ACTION, "GuiClipboard::getAsLyX(): `");
	// We don't convert encodings here since the encoding of the
	// clipboard contents is specified in the data itself
	if (cache_.hasFormat(lyxMimeType())) {
		// data from ourself or some other LyX instance
		QByteArray const ar = cache_.data(lyxMimeType());
		string const s(ar.data(), ar.count());
		LYXERR(Debug::ACTION, s << "'");
		return s;
	}
	LYXERR(Debug::ACTION, "'");
	return string();
}


std::string GuiClipboard::getAsGraphics(Cursor const & cur, bool &isFileList) const
{
    if (cache_.hasFormat("text/uri-list"))
    {
        QStringList result;
        QString text = cache_.data("text/uri-list");
        QStringList uriList = text.split("\r\n");
        for (int entryNum = 0; entryNum<uriList.size(); entryNum++)
        {
            QString uri = uriList[entryNum];
            int pos = -1;
            for (int i=uri.length()-1; i>=0; i--){
                if (uri[i]=='.')
                {
                    pos = i;
                    break;
                }
            }
            if (pos==-1)
                continue;
            QString ext = uri.mid(pos+1);
            if (ext=="bmp" || ext=="jpg" || ext=="jpeg" || ext=="png")
            {
                if (uri.startsWith("file:///"))
                {
                    uri = uri.mid(8);
                }
                result.append(uri);
            }
        }
        isFileList = true;
        return result.join("\n").toStdString();
    }
    QImage img = cache_.image();
    QByteArray imageData;
    QBuffer buf(&imageData);
    buf.open(QBuffer::WriteOnly);
    img.save(&buf, "PNG");
    buf.close();
    isFileList = false;
    return imageData.toBase64().toStdString();
}


namespace {
/**
 * Tidy up a HTML chunk coming from the clipboard.
 * This is needed since different applications put different kinds of HTML
 * on the clipboard:
 * - With or without the <?xml> tag
 * - With or without the <!DOCTYPE> tag
 * - With or without the <html> tag
 * - With or without the <body> tag
 * - With or without the <p> tag
 * Since we are going to write a HTML file for external converters we need
 * to ensure that it is a well formed HTML file, including all the mentioned tags.
 */
QString tidyHtml(QString input)
{
	// Misuse QTextDocument to cleanup the HTML.
	// As a side effect, all visual markup like <tt> is converted to CSS,
	// which is ignored by gnuhtml2latex.
	// While this may be seen as a bug by some people it is actually a
	// good thing, since we do import structure, but ignore all visual
	// clutter.
	QTextDocument converter;
	converter.setHtml(input);
	return converter.toHtml("utf-8");
}
}


docstring const GuiClipboard::getAsText(TextType type) const
{
	// text data from other applications
	if ((type == AnyTextType || type == LyXOrPlainTextType) && hasTextContents(LyXTextType))
		type = LyXTextType;
	if (type == AnyTextType && hasTextContents(LaTeXTextType))
		type = LaTeXTextType;
	if (type == AnyTextType && hasTextContents(HtmlTextType))
		type = HtmlTextType;
	QString str;
	switch (type) {
	case LyXTextType:
		// must not convert to docstring, since file can contain
		// mixed encodings (use getAsLyX() instead)
		break;
	case AnyTextType:
	case LyXOrPlainTextType:
	case PlainTextType:
		str = qApp->clipboard()->text(QClipboard::Clipboard)
				.normalized(QString::NormalizationForm_C);
		break;
	case LaTeXTextType: {
		QMimeData const * source =
			qApp->clipboard()->mimeData(QClipboard::Clipboard);
		if (source) {
			// First try LaTeX, then TeX (we do not distinguish
			// for clipboard purposes)
			if (source->hasFormat(latexMimeType())) {
				str = source->data(latexMimeType());
				str = str.normalized(QString::NormalizationForm_C);
			} else if (source->hasFormat(texMimeType())) {
				str = source->data(texMimeType());
				str = str.normalized(QString::NormalizationForm_C);
			}
		}
		break;
	}
	case HtmlTextType: {
		QString subtype = "html";
		str = qApp->clipboard()->text(subtype, QClipboard::Clipboard)
				.normalized(QString::NormalizationForm_C);
		str = tidyHtml(str);
		break;
	}
	}
	LYXERR(Debug::ACTION, "GuiClipboard::getAsText(" << type << "): `" << str << "'");
	if (str.isNull())
		return docstring();

	return internalLineEnding(str);
}


void GuiClipboard::put(string const & text) const
{
	qApp->clipboard()->setText(toqstr(text));
}


void GuiClipboard::put(string const & lyx, docstring const & html, docstring const & text)
{
	LYXERR(Debug::ACTION, "GuiClipboard::put(`" << lyx << "' `"
			      << to_utf8(html) << "' `" << to_utf8(text) << "')");
	// We don't convert the encoding of lyx since the encoding of the
	// clipboard contents is specified in the data itself
	QMimeData * data = new QMimeData;
	if (!lyx.empty()) {
		QByteArray const qlyx(lyx.c_str(), lyx.size());
		data->setData(lyxMimeType(), qlyx);
		// If the OS has not the concept of clipboard ownership,
		// we recognize internal data through its checksum.
		if (!hasInternal()) {
			boost::crc_32_type crc32;
			crc32.process_bytes(lyx.c_str(), lyx.size());
			checksum = crc32.checksum();
		}
	}
	// Don't test for text.empty() since we want to be able to clear the
	// clipboard.
	QString const qtext = toqstr(text);
	data->setText(qtext);
	QString const qhtml = toqstr(html);
	data->setHtml(qhtml);
	qApp->clipboard()->setMimeData(data, QClipboard::Clipboard);
}


bool GuiClipboard::hasTextContents(Clipboard::TextType type) const
{
	switch (type) {
	case AnyTextType:
        if (cache_.hasFormat(filesType()))
        {
            return false;
        }
		return cache_.hasFormat(lyxMimeType()) || cache_.hasText() ||
		       cache_.hasHtml() || cache_.hasFormat(latexMimeType()) ||
		       cache_.hasFormat(texMimeType());
	case LyXOrPlainTextType:
		return cache_.hasFormat(lyxMimeType()) || cache_.hasText();
	case LyXTextType:
		return cache_.hasFormat(lyxMimeType());
	case PlainTextType:
		return cache_.hasText();       
	case HtmlTextType:
		return cache_.hasHtml();
	case LaTeXTextType:
		return cache_.hasFormat(latexMimeType()) ||
		       cache_.hasFormat(texMimeType());
	}
	// shut up compiler
	return false;
}


bool GuiClipboard::hasGraphicsContents() const
{
    if (cache_.hasFormat("text/uri-list"))
    {
        QString text = cache_.data("text/uri-list");
        QStringList uriList = text.split("\r\n");
        for (int entryNum = 0; entryNum<uriList.size(); entryNum++)
        {
            QString uri = uriList[entryNum];
            int pos = -1;
            for (int i=uri.length()-1; i>=0; i--){
                if (uri[i]=='.')
                {
                    pos = i;
                    break;
                }
            }
            if (pos==-1)
                continue;
            QString ext = uri.mid(pos+1);
            if (ext=="bmp" || ext=="jpg" || ext=="jpeg" || ext=="png")
            {
                return true;
            }
        }
        return false;
    }
    return cache_.hasImage();
}


bool GuiClipboard::isInternal() const
{
	if (!hasTextContents(LyXTextType))
		return false;

	// ownsClipboard() is also true for stuff coming from dialogs, e.g.
	// the preamble dialog. This does only work on X11 and Windows, since
	// ownsClipboard() is hardwired to return false on OS X.
	if (hasInternal())
		return qApp->clipboard()->ownsClipboard();

	// We are running on OS X: Check whether clipboard data is from
	// ourself by comparing its checksum with the stored one.
	QByteArray const ar = cache_.data(lyxMimeType());
	string const data(ar.data(), ar.count());
	boost::crc_32_type crc32;
	crc32.process_bytes(data.c_str(), data.size());
	return checksum == crc32.checksum();
}


bool GuiClipboard::hasInternal() const
{
	// Windows and Mac OS X does not have the concept of ownership;
	// the clipboard is a fully global resource so all applications 
	// are notified of changes. However, on Windows ownership is
	// emulated by Qt through the OleIsCurrentClipboard() API, while
	// on Mac OS X we deal with this issue by ourself.
#if defined(Q_WS_X11) || defined(Q_OS_WIN) || defined(Q_CYGWIN_WIN)
	return true;
#else
	return false;
#endif
}


void GuiClipboard::on_dataChanged()
{
	//Note: we do not really need to run cache_.update() unless the
	//data has been changed *and* the GuiClipboard has been queried.
	//However if run cache_.update() the moment a process grabs the
	//clipboard, the process holding the clipboard presumably won't
	//yet be frozen, and so we won't need to wait 5 seconds for Qt
	//to time-out waiting for the clipboard.
	cache_.update();
	QStringList l = cache_.formats();
	LYXERR(Debug::ACTION, "Qt Clipboard changed. We found the following mime types:");
	for (int i = 0; i < l.count(); i++)
		LYXERR(Debug::ACTION, l.value(i));

	plaintext_clipboard_empty_ = qApp->clipboard()->
		text(QClipboard::Clipboard).isEmpty();

	has_text_contents_ = hasTextContents();
	has_graphics_contents_ = hasGraphicsContents();
}


bool GuiClipboard::empty() const
{
	// We need to check both the plaintext and the LyX version of the
	// clipboard. The plaintext version is empty if the LyX version
	// contains only one inset, and the LyX version is empty if the
	// clipboard does not come from LyX.
	if (!plaintext_clipboard_empty_)
		return false;
	return !has_text_contents_ && !has_graphics_contents_;
}

} // namespace frontend
} // namespace lyx

#include "moc_GuiClipboard.cpp"
