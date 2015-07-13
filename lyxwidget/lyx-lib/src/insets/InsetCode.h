// -*- C++ -*-
/**
 * \file InsetCode.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Jürgen Vigna
 * \author Lars Gullik Bjønnes
 * \author Matthias Ettrich
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETCODE_H
#define INSETCODE_H

namespace lyx {

enum InsetCode {
	///
	NO_CODE, // 0
	///
	TOC_CODE,  // do these insets really need a code? (ale)
	///
	QUOTE_CODE,
	///
	MARK_CODE,
	///
	REF_CODE,
	///
	HYPERLINK_CODE, // 5
	///
	SEPARATOR_CODE,
	///
	ENDING_CODE,
	///
	LABEL_CODE,
	///
	NOTE_CODE,
	///
	ACCENT_CODE, // 10
	///
	MATH_CODE,
	///
	INDEX_CODE,
	///
	INCLUDE_CODE,
	///
	GRAPHICS_CODE,
    ///
    CHART_CODE, // 15
    ///
    TABLEDATA_CODE,
    ///
    BIBITEM_CODE,
	///
	BIBTEX_CODE,
	///
	TEXT_CODE,
	///
    ERT_CODE, // 20
	///
    FOOT_CODE,
	///
    MARGIN_CODE,
	///
	FLOAT_CODE,
	///
	WRAP_CODE,
	///
    SPACE_CODE, // 25
	///
    SPECIALCHAR_CODE,
	///
    TABULAR_CODE,
	///
	EXTERNAL_CODE,
	///
	CAPTION_CODE,
	///
    MATHMACRO_CODE, // 30
	///
    CITE_CODE,
	///
    FLOAT_LIST_CODE,
	///
	INDEX_PRINT_CODE,
	///
	ARG_CODE,
	///
    CELL_CODE, // 35
	///
    NEWLINE_CODE,
	///
    LINE_CODE,
	///
	BRANCH_CODE,
	///
	BOX_CODE,
	///
    FLEX_CODE, // 40
	///
    VSPACE_CODE,
	///
    MATH_MACROARG_CODE,
	///
	NOMENCL_CODE,
	///
	NOMENCL_PRINT_CODE,
	///
    NEWPAGE_CODE, // 45
	///
    LISTINGS_CODE,
	///
    INFO_CODE,
	///
	COLLAPSABLE_CODE,
	///
	PHANTOM_CODE,
	///
    MATH_AMSARRAY_CODE, // 50
	///
    MATH_ARRAY_CODE,
	///
    MATH_BIG_CODE,
	///
	MATH_BOLDSYMBOL_CODE,
	///
	MATH_BOX_CODE,
	///
    MATH_BRACE_CODE, // 55
	///
    MATH_CANCEL_CODE,
	///
    MATH_CANCELTO_CODE,
	///
	MATH_CASES_CODE,
	///
	MATH_CHAR_CODE, 
	///
    MATH_COLOR_CODE, // 60
	///
    MATH_COMMENT_CODE,
	///
    MATH_DECORATION_CODE,
	///
	MATH_DELIM_CODE,
	///
	MATH_DIFF_CODE,
	///
    MATH_DOTS_CODE, // 65
	///
    MATH_ENSUREMATH_CODE,
	///
    MATH_ENV_CODE,
	///
	MATH_EXFUNC_CODE,
	///
	MATH_EXINT_CODE,
	///
    MATH_FONT_CODE, // 70
	///
    MATH_FONTOLD_CODE,
	///
    MATH_FRAC_CODE,
	///
	MATH_GRID_CODE,
	///
	MATH_HULL_CODE,
	///
    MATH_KERN_CODE, // 75
	///
    MATH_LEFTEQN_CODE,
	///
    MATH_LIM_CODE,
	///
	MATH_MATRIX_CODE,
	///
	MATH_MBOX_CODE,
	///
    MATH_NEST_CODE, // 80
	///
    MATH_NUMBER_CODE,
	///
    MATH_OVERSET_CODE,
	///
	MATH_PAR_CODE,
	///
	MATH_PHANTOM_CODE,
	///
    MATH_REF_CODE, // 85
	///
    MATH_ROOT_CODE,
	///
    MATH_SCRIPT_CODE,
	///
	MATH_SIZE_CODE,
	///
	MATH_SPACE_CODE,
	///
    MATH_SPECIALCHAR_CODE, // 90
	///
    MATH_SPLIT_CODE,
	///
    MATH_SQRT_CODE,
	///
	MATH_STACKREL_CODE,
	///
	MATH_STRING_CODE,
	///
    MATH_SUBSTACK_CODE, // 95
	///
    MATH_SYMBOL_CODE,
	///
    MATH_TABULAR_CODE,
	///
	MATH_UNDERSET_CODE,
	///
	MATH_UNKNOWN_CODE,
	///
    MATH_XARROW_CODE, // 100
	///
    MATH_XYARROW_CODE,
    ///
    MATH_XYMATRIX_CODE,
	///
	MATH_MACRO_CODE,
	///
	ARGUMENT_PROXY_CODE,
	///
    PREVIEW_CODE, // 105
	///
    MATH_DIAGRAM_CODE,
	///
    SCRIPT_CODE,
	///
	IPA_CODE,
	///
	IPACHAR_CODE,
	///
    IPADECO_CODE, //110
	///
    INSET_CODE_SIZE
};

} // namespace lyx

#endif
