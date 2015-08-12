/**
 * \file InsetChartParams.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Maxim Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetChartParams.h"

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
#include "support/docstream.h"
#include "support/FileName.h"
#include "support/gettext.h"
#include "support/lassert.h"
#include "../Encoding.h"

#include <ostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
#include <QString>
#include <QByteArray>
#include <QImage>

//#include "../frontends/qt4/ChartConverter.h"

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace {

bool string2type(string const & str, bool & num)
{
    if (str == "true")
        num = true;
    else if (str == "false")
        num = false;
    else
        return false;
    return true;
}


bool getTokenValue(string const & str, char const * token, string & ret)
{
    ret.erase();
    size_t token_length = strlen(token);
    size_t pos = str.find(token);

    if (pos == string::npos || pos + token_length + 1 >= str.length()
        || str[pos + token_length] != '=')
        return false;
    pos += token_length + 1;
    char ch = str[pos];
    if (ch != '"' && ch != '\'') { // only read till next space
        ret += ch;
        ch = ' ';
    }
    while (pos < str.length() - 1 && str[++pos] != ch)
        ret += str[pos];

    return true;
}


bool getTokenValue(string const & str, char const * token, docstring & ret)
{
    string tmp;
    bool const success = getTokenValue(str, token, tmp);
    ret = from_utf8(tmp);
    return success;
}


bool getTokenValue(string const & str, char const * token, int & num)
{
    string tmp;
    num = 0;
    if (!getTokenValue(str, token, tmp))
        return false;
    num = convert<int>(tmp);
    return true;
}

bool getTokenValue(string const & str, char const * token, double & num)
{
    string tmp;
    num = 0;
    if (!getTokenValue(str, token, tmp))
        return false;
    num = convert<double>(tmp);
    return true;
}

bool getTokenValue(string const & str, char const * token, bool & flag)
{
    // set the flag always to false as this should be the default for bools
    // not in the file-format.
    flag = false;
    string tmp;
    return getTokenValue(str, token, tmp) && string2type(tmp, flag);
}


bool getTokenValue(string const & str, char const * token, Length & len)
{
    // set the length to be zero() as default as this it should be if not
    // in the file format.
    len = Length();
    string tmp;
    return getTokenValue(str, token, tmp) && isValidLength(tmp, &len);
}


bool getTokenValue(string const & str, char const * token, Length & len, bool & flag)
{
    len = Length();
    flag = false;
    string tmp;
    if (!getTokenValue(str, token, tmp))
        return false;
    if (tmp == "default") {
        flag = true;
        return  true;
    }
    return isValidLength(tmp, &len);
}


void l_getline(istream & is, string & str)
{
    str.erase();
    while (str.empty()) {
        getline(is, str);
        if (!str.empty() && str[str.length() - 1] == '\r')
            str.erase(str.length() - 1);
    }
}

string const write_attribute(string const & name, string const & t)
{
    return t.empty() ? t : " " + name + "=\"" + t + "\"";
}

string const write_attribute(string const & name, docstring const & t)
{
    return t.empty() ? string() : " " + name + "=\"" + to_utf8(t) + "\"";
}

string const write_attribute(string const & name, bool const & b)
{
    // we write only true attribute values so we remove a bit of the
    // file format bloat for tabulars.
    return b ? write_attribute(name, convert<string>(b)) : string();
}

string const write_attribute(string const & name, int const & i)
{
    // we write only true attribute values so we remove a bit of the
    // file format bloat for tabulars.
    return i ? write_attribute(name, convert<string>(i)) : string();
}

string const write_attribute(string const & name, double const & i)
{
    // we write only true attribute values so we remove a bit of the
    // file format bloat for tabulars.
    return i ? write_attribute(name, convert<string>(i)) : string();
}

string const write_attribute(string const & name, Length const & value)
{
    // we write only the value if we really have one same reson as above.
    return value.zero() ? string() : write_attribute(name, value.asString());
}
}

InsetChartParams::InsetChartParams()
{
    init();
//    converter = new InsetChartConverter();
//    connect(converter, SIGNAL(finished()), this, SLOT(onConverted()));
}


InsetChartParams::InsetChartParams(InsetChartParams const & igp)
{
    copy(igp);
//    converter = new InsetChartConverter();
//    connect(converter, SIGNAL(finished()), this, SLOT(onConverted()));
}

void InsetChartParams::operator=(InsetChartParams const & params)
{
    // Are we assigning the object into itself?
    if (this == &params)
        return;
    copy(params);
}

void InsetChartParams::init()
{
    clear();
    lyxscale = 100;			// lyx scaling in percentage
    xLabel = "X";
    yLabel = "Y";
    grid = false;
    legend = false;
    ChartLine *line = new ChartLine;
    line->name = "Ряд 1";
    lines.push_back(line);
}

void InsetChartParams::clear()
{
    for (int i=0; i<(int)lines.size(); i++)
    {
        delete lines[i];
    }
    lines.clear();
    imageData.clear();
}

void InsetChartParams::copy(InsetChartParams const & igp)
{
    clear();
    imageData = QByteArray(igp.imageData.data(), igp.imageData.size());
    lyxscale = igp.lyxscale;
    title = igp.title;
    legend = igp.legend;
    xLabel = igp.xLabel;
    yLabel = igp.yLabel;
    grid = igp.grid;
    int linesCount = (int)igp.lines.size();
    for (int i=0; i<linesCount; i++){
        ChartLine *line = igp.lines[i];
        lines.push_back(line->clone());
    }
}

//void InsetChartParams::onConverted()
//{
//    imageData = converter->getImageData();
//}

void InsetChartParams::toStream(ostream &os) const
{
    os << "<lyxchart ";
    os << "imagedata=\"" << imageData.toBase64(QByteArray::Base64UrlEncoding).data() << "\" ";
    os << "lyxscale=\"" << lyxscale << "\"";
    os << ">\n";
    int linesCount = (int)lines.size();
    os << "<features"
       << write_attribute("title", title)
       << write_attribute("legend", legend)
       << write_attribute("xLabel", xLabel)
       << write_attribute("yLabel", yLabel)
       << write_attribute("grid", grid)
       << write_attribute("linesCount", linesCount);
    os << ">\n";
    for (int i=0; i<linesCount; i++){
        ChartLine *line = lines[i];
        int pointsCount = (int)line->data.size();
        os << "<line"
           << write_attribute("name", line->name)
           << write_attribute("smooth", line->smooth)
           << write_attribute("lineColor", line->lineColor)
           << write_attribute("lineType", line->lineType)
           << write_attribute("markerType", line->markerType)
           << write_attribute("pointsCount", pointsCount);
        os << ">\n";
        for (int i=0; i<pointsCount; i++){
            ChartPoint *p = line->data[i];
            os << "<point"
               << write_attribute("x", p->x)
               << write_attribute("y", p->y);
            os << ">\n";
        }
        os << "</line>\n";
    }
    os << "</lyxchart>\n";
    os.flush();
}

void InsetChartParams::latex(otexstream & os, OutputParams const & runparams) const
{
    Encoding const & encoding = *(runparams.encoding);
    os<<"\\begin{tikzpicture}\n";
    os<<"\\begin{axis}[\n";
    os<<"title=";
    os<<encoding.latexString(from_utf8(title)).first;
    os<<",\n";
    if (legend){
        os<<"legend style={xshift=3.5cm,yshift=-.2cm},\n";
    }
    if (grid){
        os<<"grid=major,\n";
    }
    os<<"xlabel="<<encoding.latexString(from_utf8(xLabel)).first<<",\n";
    os<<"ylabel="<<encoding.latexString(from_utf8(yLabel)).first<<"\n";
    os<<"]\n";

    for (std::vector<ChartLine* >::const_iterator i=lines.begin(); i!=lines.end();i++){
        ChartLine *line = *i;
        string smoothValue = "";
        if (line->smooth){
            smoothValue= ", smooth";
        }
        os<<"\\addplot["<<line->lineType<<", mark = "<<line->markerType<<smoothValue<<", mark options={solid}, color="<<line->lineColor<<"] coordinates {\n";
        for (std::vector<ChartPoint* >::iterator i=line->data.begin(); i!=line->data.end(); i++){
            ChartPoint *point = *i;
            os<<"( "<<point->x<<", "<<point->y<<" )\n";
        }
        os<<"};\n";
        if (legend){
            os<<"\\addlegendentry{"<<encoding.latexString(from_utf8(line->name)).first<<"}\n";
        }
    }
    os<<"\\end{axis}\n";
    os<<"\\end{tikzpicture}\n";
}

bool operator==(InsetChartParams const & left,
        InsetChartParams const & right)
{
    ostringstream streamLeft;
    left.toStream(streamLeft);
    ostringstream streamRight;
    right.toStream(streamRight);
    string str1 = streamLeft.str();
    string str2 = streamRight.str();
    return str1 == str2;
}

bool operator!=(InsetChartParams const & left,
        InsetChartParams const & right)
{
    return	!(left == right);
}

void InsetChartParams::write(ostream & os) const
{
    toStream(os);
}

bool InsetChartParams::read(Lexer & lex)
{
//    ostringstream streamOld;
//    this->toStream(streamOld);
    clear();
    string line;
    istream & is = lex.getStream();

    l_getline(is, line);
    if (!prefixIs(line, "<lyxchart")) {
        LASSERT(false, return false);
    }
    string imgData;
    if (!getTokenValue(line, "imagedata", imgData))
    {
        imageData = QString("loading").toUtf8();
    } else
    {
        QByteArray arr(imgData.c_str());
        arr = QByteArray::fromBase64(arr, QByteArray::Base64UrlEncoding);
        imageData = arr;
    }
    int num = 100;
    if (!getTokenValue(line, "lyxscale", num))
    {
        lyxscale = 100;
    }else {
        lyxscale = num;
    }
    l_getline(is, line);
    if (!prefixIs(line, "<features")) {
        lyxerr << "Wrong chart format (expected <features ...> got "
               << line << ')' << endl;
        return false;
    }

    getTokenValue(line, "title", title);
    getTokenValue(line, "legend", legend);
    getTokenValue(line, "xLabel", xLabel);
    getTokenValue(line, "yLabel", yLabel);
    getTokenValue(line, "grid", grid);
//    title = urlDecode(title);
//    xLabel = urlDecode(xLabel);
//    yLabel = urlDecode(yLabel);
    int linesCount = 0;
    getTokenValue(line, "linesCount", linesCount);
    for (int i=0; i<linesCount; i++){
        l_getline(is, line);
        if (!prefixIs(line, "<line")) {
            lyxerr << "Wrong chart format (expected <line ...> got "
                   << line << ')' << endl;
            return false;
        }
        ChartLine *chartLine = new ChartLine;
        lines.push_back(chartLine);
        getTokenValue(line, "name", chartLine->name);
//        chartLine->name = urlDecode(chartLine->name);
        getTokenValue(line, "smooth", chartLine->smooth);
        getTokenValue(line, "lineColor", chartLine->lineColor);
        getTokenValue(line, "lineType", chartLine->lineType);
        getTokenValue(line, "markerType", chartLine->markerType);
        int pointsCount = 0;
        getTokenValue(line, "pointsCount", pointsCount);
        for (int i=0; i<pointsCount; i++){
            l_getline(is, line);
            if (!prefixIs(line, "<point")) {
                lyxerr << "Wrong chart format (expected <point ...> got "
                       << line << ')' << endl;
                return false;
            }
            ChartPoint *p = new ChartPoint;
            chartLine->data.push_back(p);
            getTokenValue(line, "x", p->x);
            getTokenValue(line, "y", p->y);
        }
        l_getline(is, line);
        if (!prefixIs(line, "</line>")) {
            lyxerr << "Wrong tabular format (expected </point> got "
                   << line << ')' << endl;
            return false;
        }
    }
    l_getline(is, line);
    if (!prefixIs(line, "</lyxchart>")) {
        lyxerr << "Wrong tabular format (expected </lyxchart> got "
               << line << ')' << endl;
        return false;
    }
//    if (imageData=="loading")
//    {
//        converter->startConvertation(this);
//    }
//    ostringstream streamNew;
//    this->toStream(streamNew);
//    if (streamOld.str()!=streamNew.str())
//    {
//        converter->startConvertation(this);
//    }
    return true;
}

graphics::Params InsetChartParams::as_grfxParams() const
{
    graphics::Params pars;
    pars.imageData = imageData;
    pars.scale = lyxscale;
    pars.angle = 0;
    pars.display = true;
    return pars;
}

} // namespace lyx

