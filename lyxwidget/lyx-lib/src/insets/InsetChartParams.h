// -*- C++ -*-
/**
 * \file InsetChartParams.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Maxim Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETCHARTPARAMS_H
#define INSETCHARTPARAMS_H


#include "Length.h"

#include "support/FileName.h"

#include <string>

//
#include "Buffer.h"
#include "LyX.h" // for use_gui
#include "Lexer.h"
#include "LyXRC.h"

#include "graphics/epstools.h"
#include "graphics/GraphicsParams.h"
#include "graphics/GraphicsTypes.h"

#include "support/convert.h"
#include "support/debug.h"
#include "support/lyxlib.h"
#include "support/lstrings.h"
#include "support/Translator.h"
#include "support/TempFile.h"

#include <ostream>

//#include "../frontends/qt4/ChartConverter.h"


using namespace std;
using namespace lyx::support;

//class InsetChartConverter;

namespace lyx {

namespace graphics { class Params; }

class Lexer;
class Buffer;

class ChartPoint{
public:
    double x;
    double y;
    ChartPoint(){
        x = 0;
        y = 0;
    }
    ChartPoint *clone(){
        ChartPoint *result = new ChartPoint;
        result->x = x;
        result->y = y;
        return result;
    }
};

class ChartLine{
public:
    ChartLine(){
        name = "";
        smooth = false;
        lineColor = "black";
        lineType = "solid";
        markerType = "none";
    }
    ChartLine *clone(){
        ChartLine *result = new ChartLine;
        result->name = name;
        result->smooth = smooth;
        result->lineColor = lineColor;
        result->lineType = lineType;
        result->markerType = markerType;
        for (std::vector<ChartPoint* >::iterator i = data.begin(); i!=data.end(); ++i){
            ChartPoint *p = (*i);
            result->data.push_back(p->clone());
        }
        return result;
    }
    vector< ChartPoint* > data;
    string name;
    bool smooth;
    string lineColor;
    string lineType;
    string markerType;
};

/// This class holds all the parameters needed by insetChart.
class InsetChartParams
{
//    Q_OBJECT
public:
    QByteArray imageData;

    /// Scaling the Screen inside Lyx
    unsigned int lyxscale;

	///
    InsetChartParams();

	///
    InsetChartParams(InsetChartParams const & igp);
	///
    void operator=(InsetChartParams const & params);
	/// Save the parameters in the LyX format stream.
	/// Buffer is needed to figure out if a figure is embedded.
    void write(ostream & os) const;
	/// If the token belongs to our parameters, read it.
    bool read(Lexer & lex);
    ///
    graphics::Params as_grfxParams() const;
    ///
    string title;
    bool legend;
    string xLabel;
    string yLabel;
    bool grid;
    vector<ChartLine* > lines;
    void toStream(ostream & os) const;
    void latex(otexstream &os) const;
private:
//    InsetChartConverter *converter;
	/// Initialize the object to a default status.
    void init();
    void clear();
	/// Copy the other objects content to us, used in copy c-tor and assignment
    void copy(InsetChartParams const & igp);

//private Q_SLOTS:
//    void onConverted();
};

///
bool operator==(InsetChartParams const & left, InsetChartParams const & right);
///
bool operator!=(InsetChartParams const & left, InsetChartParams const & right);

} // namespace lyx

#endif
