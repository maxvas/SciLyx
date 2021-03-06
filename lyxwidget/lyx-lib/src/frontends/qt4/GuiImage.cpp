/**
 * \file GuiImage.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiImage.h"
#include "qt_helpers.h"

#include "Format.h"

#include "graphics/GraphicsParams.h"

#include "support/debug.h"
#include "support/lstrings.h"       // ascii_lowercase
#include "GuiApplication.h"

#include <QPainter>
#include <QImageReader>
#include <QByteArray>

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace graphics {

/// Implement factory method defined in GraphicsImage.h
Image * newImage()
{
	return new GuiImage;
}


GuiImage::GuiImage() : is_transformed_(false)
{}


GuiImage::GuiImage(GuiImage const & other)
	: Image(other), original_(other.original_),
	transformed_(other.transformed_), is_transformed_(other.is_transformed_),
    imageData_(other.imageData_)
{}


Image * GuiImage::clone() const
{
	return new GuiImage(*this);
}


QImage const & GuiImage::image() const
{
	return is_transformed_ ? transformed_ : original_;
}


unsigned int GuiImage::width() const
{
	return is_transformed_ ? transformed_.width() : original_.width();
}


unsigned int GuiImage::height() const
{
	return is_transformed_ ? transformed_.height() : original_.height();
}


bool GuiImage::load(QByteArray const & imageData)
{
//	if (!original_.isNull()) {
//		LYXERR(Debug::GRAPHICS, "Image is loaded already!");
//		return false;
//	}
    imageData_ = imageData;
	return load();
}


bool GuiImage::load()
{
    if (imageData_.length()==7)
    {
        if (imageData_=="loading")
        {
            QPixmap loading = frontend::getPixmap("images/", "loading", "png");
            original_ = loading.toImage();
            return true;
        }
    }
    if (!original_.loadFromData(imageData_)) {
		LYXERR(Debug::GRAPHICS, "Unable to open image");
		return false;
	}
	return true;
}


bool GuiImage::setPixmap(Params const & params)
{
	if (!params.display)
		return false;

	if (original_.isNull()) {
		if (!load())
			return false;
	}
		
	is_transformed_ = clip(params);
	is_transformed_ |= rotate(params);
	is_transformed_ |= scale(params);

	// Clear the pixmap to save some memory.
	if (is_transformed_)
		original_ = QImage();
	else
		transformed_ = QImage();

	return true;
}


bool GuiImage::clip(Params const & params)
{
	if (params.bb.empty())
		// No clipping is necessary.
		return false;

	int const new_width  = params.bb.xr - params.bb.xl;
	int const new_height = params.bb.yt - params.bb.yb;

	QImage const & image = is_transformed_ ? transformed_ : original_;

	// No need to check if the width, height are > 0 because the
	// Bounding Box would be empty() in this case.
	if (new_width > image.width() || new_height > image.height()) {
		// Bounds are invalid.
		return false;
	}

	if (new_width == image.width() && new_height == image.height())
		return false;

	int const xoffset_l = params.bb.xl;
	int const yoffset_t = (image.height() > int(params.bb.yt))
		? image.height() - params.bb.yt : 0;

	transformed_ = image.copy(xoffset_l, yoffset_t, new_width, new_height);
	return true;
}


bool GuiImage::rotate(Params const & params)
{
	if (!params.angle)
		return false;

	QImage const & image = is_transformed_ ? transformed_ : original_;
	QMatrix m;
	m.rotate(- params.angle);
	transformed_ = image.transformed(m);
	return true;
}


bool GuiImage::scale(Params const & params)
{
	QImage const & image = is_transformed_ ? transformed_ : original_;

	if (params.scale == 100)
		return false;

	qreal scale = qreal(params.scale) / 100.0;

#if (QT_VERSION >= 0x040500) && (QT_VERSION <= 0x040502)
	// Due to a bug in Qt, LyX will crash for certain
	// scaling factors and sizes of the image.
	// see bug #5957: http://www.lyx.org/trac/ticket/5957
	scale += 0.0001;
#endif

	QMatrix m;
	m.scale(scale, scale);
	transformed_ = image.transformed(m);
	return true;
}

} // namespace graphics
} // lyx
