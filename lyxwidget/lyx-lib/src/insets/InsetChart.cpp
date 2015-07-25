/**
 * \file InsetChart.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Maxim Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "insets/InsetChart.h"
#include "insets/RenderGraphic.h"

#include "Buffer.h"
#include "BufferView.h"
#include "Converter.h"
#include "Cursor.h"
#include "DispatchResult.h"
#include "ErrorList.h"
#include "Exporter.h"
#include "Format.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "InsetIterator.h"
#include "LaTeXFeatures.h"
#include "Length.h"
#include "Lexer.h"
#include "MetricsInfo.h"
#include "Mover.h"
#include "OutputParams.h"
#include "output_xhtml.h"
#include "sgml.h"
#include "TocBackend.h"

#include "frontends/alert.h"
#include "frontends/Application.h"

#include "support/convert.h"
#include "support/debug.h"
#include "support/docstream.h"
#include "support/ExceptionMessage.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/lyxlib.h"
#include "support/lstrings.h"
#include "support/os.h"
#include "support/Systemcall.h"

#include <boost/tuple/tuple.hpp>

#include <algorithm>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace Alert = frontend::Alert;

InsetChart::InsetChart(Buffer * buf)
    : Inset(buf), graphic_(new RenderGraphic(this))
{
}


InsetChart::InsetChart(InsetChart const & ig)
    : Inset(ig), graphic_(new RenderGraphic(*ig.graphic_, this))
{
	setParams(ig.params());
}


Inset * InsetChart::clone() const
{
	return new InsetChart(*this);
}


InsetChart::~InsetChart()
{
	hideDialogs("chart", this);
	delete graphic_;
}


void InsetChart::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action()) {
	case LFUN_INSET_EDIT: {
		InsetChartParams p = params();
		if (!cmd.argument().empty())
            string2params(to_utf8(cmd.argument()), buffer(), p);
		editChart(p);
		break;
	}

	case LFUN_INSET_MODIFY: {
		if (cmd.getArg(0) != "chart") {
			Inset::doDispatch(cur, cmd);
			break;
		}

		InsetChartParams p;
        string2params(to_utf8(cmd.argument()), buffer(), p);
		cur.recordUndo();
		setParams(p);
		break;
	}

	case LFUN_INSET_DIALOG_UPDATE:
		cur.bv().updateDialog("chart", params2string(params(), buffer()));
		break;

	case LFUN_GRAPHICS_RELOAD:
		graphic_->reload();
		break;

	default:
		Inset::doDispatch(cur, cmd);
		break;
	}
}


bool InsetChart::getStatus(Cursor & cur, FuncRequest const & cmd,
		FuncStatus & flag) const
{
	switch (cmd.action()) {
	case LFUN_INSET_MODIFY:
		if (cmd.getArg(0) != "chart")
			return Inset::getStatus(cur, cmd, flag);
	// fall through
	case LFUN_INSET_EDIT:
	case LFUN_INSET_DIALOG_UPDATE:
	case LFUN_GRAPHICS_RELOAD:
		flag.setEnabled(true);
		return true;

	default:
		return Inset::getStatus(cur, cmd, flag);
	}
}


bool InsetChart::showInsetDialog(BufferView * bv) const
{
	bv->showDialog("chart", params2string(params(), bv->buffer()),
		const_cast<InsetChart *>(this));
	return true;
}



void InsetChart::metrics(MetricsInfo & mi, Dimension & dim) const
{
    graphic_->metrics(mi, dim);
}


void InsetChart::draw(PainterInfo & pi, int x, int y) const
{
	graphic_->draw(pi, x, y);
}


void InsetChart::write(ostream & os) const
{
    os << "Chart\n";
    params_.write(os);
}


void InsetChart::read(Lexer & lex)
{
    lex.setContext("InsetChart::read");
    params_.read(lex);
    lex.next();
    string token = lex.getString();
    while (lex && token != "\\end_inset") {
        lex.next();
        token = lex.getString();
    }
    if (!lex)
        lex.printError("Missing \\end_inset at this point. ");
    graphic_->update(params().as_grfxParams());
}


string InsetChart::createLatexOptions() const
{
	// Calculate the options part of the command, we must do it to a string
	// stream since we might have a trailing comma that we would like to remove
	// before writing it to the output stream.
	ostringstream options;
	ostringstream size;
	string opts = options.str();
	// delete last ','
	if (suffixIs(opts, ','))
		opts = opts.substr(0, opts.size() - 1);
	return opts;
}


docstring InsetChart::toDocbookLength(Length const & len) const
{
	odocstringstream result;
	switch (len.unit()) {
		case Length::SP: // Scaled point (65536sp = 1pt) TeX's smallest unit.
			result << len.value() * 65536.0 * 72 / 72.27 << "pt";
			break;
		case Length::PT: // Point = 1/72.27in = 0.351mm
			result << len.value() * 72 / 72.27 << "pt";
			break;
		case Length::BP: // Big point (72bp = 1in), also PostScript point
			result << len.value() << "pt";
			break;
		case Length::DD: // Didot point = 1/72 of a French inch, = 0.376mm
			result << len.value() * 0.376 << "mm";
			break;
		case Length::MM: // Millimeter = 2.845pt
			result << len.value() << "mm";
			break;
		case Length::PC: // Pica = 12pt = 4.218mm
			result << len.value() << "pc";
			break;
		case Length::CC: // Cicero = 12dd = 4.531mm
			result << len.value() * 4.531 << "mm";
			break;
		case Length::CM: // Centimeter = 10mm = 2.371pc
			result << len.value() << "cm";
			break;
		case Length::IN: // Inch = 25.4mm = 72.27pt = 6.022pc
			result << len.value() << "in";
			break;
		case Length::EX: // Height of a small "x" for the current font.
			// Obviously we have to compromise here. Any better ratio than 1.5 ?
			result << len.value() / 1.5 << "em";
			break;
		case Length::EM: // Width of capital "M" in current font.
			result << len.value() << "em";
			break;
		case Length::MU: // Math unit (18mu = 1em) for positioning in math mode
			result << len.value() * 18 << "em";
			break;
		case Length::PTW: // Percent of TextWidth
		case Length::PCW: // Percent of ColumnWidth
		case Length::PPW: // Percent of PageWidth
		case Length::PLW: // Percent of LineWidth
		case Length::PTH: // Percent of TextHeight
		case Length::PPH: // Percent of PaperHeight
			// Sigh, this will go wrong.
			result << len.value() << "%";
			break;
		default:
			result << len.asDocstring();
			break;
	}
	return result.str();
}


docstring InsetChart::createDocBookAttributes() const
{
	// Calculate the options part of the command, we must do it to a string
	// stream since we copied the code from createLatexParams() ;-)

	// FIXME: av: need to translate spec -> Docbook XSL spec
	// (http://www.sagehill.net/docbookxsl/ImageSizing.html)
	// Right now it only works with my version of db2latex :-)

	odocstringstream options;
	// trailing blanks are ok ...
	return options.str();
}

void InsetChart::latex(otexstream & os,
			  OutputParams const & runparams) const
{
    params_.latex(os);
}

int InsetChart::plaintext(odocstringstream &ods, const OutputParams &op, size_t max_length) const
{
    //TODO: дописать
    docstring const str = from_utf8("");
    ods << '<' << str << '>';

    return 2 + str.size();
}

int InsetChart::docbook(odocstream &, const OutputParams &) const
{
    //TODO: Дописать преобразование в DocBook
    return 0;
}

docstring InsetChart::xhtml(XHTMLStream &os, const OutputParams &) const
{
    //TODO: Дописать преобразование в xhtml
    return docstring();
}

void InsetChart::validate(LaTeXFeatures & features) const
{
	features.require("tikz");
	features.require("pgfplots");
    features.require("plotmarks");
}


bool InsetChart::setParams(InsetChartParams const & p)
{
	// If nothing is changed, just return and say so.

	// Copy the new parameters.
	params_ = p;

    // Update the display using the new parameters.
    graphic_->update(params().as_grfxParams());

	// We have changed data, report it.
	return true;
}


InsetChartParams const & InsetChart::params() const
{
	return params_;
}


void InsetChart::editChart(InsetChartParams const & p) const
{
}


void InsetChart::addToToc(DocIterator const & cpit, bool output_active) const
{
	//FIXME UNICODE
    docstring const str = from_utf8("");
	buffer().tocBackend().toc("chart").push_back(TocItem(cpit, 0, str, output_active));
}


string InsetChart::contextMenuName() const
{
	return "context-chart";
}


void InsetChart::string2params(string const & in, Buffer const & buffer,
                               InsetChartParams & params)
{
    if (in.empty())
        return;

    istringstream data(in);
    Lexer lex;
    lex.setStream(data);
    lex.setContext("InsetChart::string2params");
    lex >> "chart";
    params = InsetChartParams();
    params.read(lex);
    lex >> "\\end_inset";
}


string InsetChart::params2string(const InsetChartParams &params, Buffer const & buffer)
{
    ostringstream data;
    data << "chart\n";
    params.write(data);
    data << "\\end_inset\n";
    return data.str();
}


docstring InsetChart::toolTip(BufferView const &, int, int) const
{
    return from_utf8("chart");
}

} // namespace lyx
