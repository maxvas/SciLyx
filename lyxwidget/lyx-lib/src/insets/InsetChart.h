// -*- C++ -*-
/**
 * \file InsetChart.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Maxim Kuznetcov
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_CHART_H
#define INSET_CHART_H

#include "Inset.h"
#include "InsetChartParams.h"

#include <set>

namespace lyx {

class RenderGraphic;
class LaTeXFeatures;

/////////////////////////////////////////////////////////////////////////
//
// InsetChart
//
/////////////////////////////////////////////////////////////////////////

/// Used for functions graphics, diagrams etc.
class InsetChart : public Inset
{
public:
	///
	InsetChart(Buffer * buf);
	///
	~InsetChart();

	///
    static void string2params(string const & in, const Buffer &buffer, InsetChartParams &params);
	///
    static std::string params2string(InsetChartParams const & params, Buffer const & buffer);

	/** Set the inset parameters, used by the GUIndependent dialog.
	    Return true of new params are different from what was so far.
	*/
	bool setParams(InsetChartParams const & params);

    InsetChartParams getParams() const { return params_;}
	///
	bool clickable(int, int) const { return true; }

private:
	///
	InsetChart(InsetChart const &);

	///
	bool isLabeled() const { return true; }
	void metrics(MetricsInfo &, Dimension &) const;
	///
	bool hasSettings() const { return true; }
	///
	void write(std::ostream &) const;
	///
	void read(Lexer & lex);
	///
	void latex(otexstream &, OutputParams const &) const;
    ///
    int plaintext(odocstringstream & ods, OutputParams const & op,
                  size_t max_length = INT_MAX) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	docstring xhtml(XHTMLStream & os, OutputParams const &) const;
	/** Tell LyX what the latex features you need i.e. what latex packages
	    you need to be included.
	 */
	void validate(LaTeXFeatures & features) const;
	/// returns LyX code associated with the inset. Used for TOC, ...)
	InsetCode lyxCode() const { return CHART_CODE; }
	///
	docstring layoutName() const { return from_ascii("Chart"); }
	/// Get the inset parameters, used by the GUIndependent dialog.
	InsetChartParams const & params() const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	bool showInsetDialog(BufferView * bv) const;
	///
	void editChart(InsetChartParams const &) const;
	///
	bool getStatus(Cursor &, FuncRequest const &, FuncStatus &) const;
	///
	void addToToc(DocIterator const & di, bool output_active) const;
	///
	std::string contextMenuName() const;
	/// Force inset into LTR environment if surroundings are RTL
	bool forceLTR() const { return true; }
	///
	void doDispatch(Cursor & cur, FuncRequest & cmd);
	///
	Inset * clone() const;
	/// Get the status message, depends on the image loading status.
	std::string statusMessage() const;
	/// Create the options for the latex command.
	std::string createLatexOptions() const;
	/// Create length values for docbook export.
	docstring toDocbookLength(Length const & len) const;
	/// Create the atributes for docbook export.
	docstring createDocBookAttributes() const;
	///
	InsetChartParams params_;
	/// holds the entity name that defines the chart location (SGML).
	docstring const chart_label;
	///
	docstring toolTip(BufferView const & bv, int x, int y) const;
	/// The thing that actually draws the image on LyX's screen.
	RenderGraphic * graphic_;
};

} // namespace lyx

#endif // INSET_CHART_H
