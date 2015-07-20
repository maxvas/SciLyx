/*
 * \file config.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * This is the compilation configuration file for LyX.
 * It was generated by cmake.
 * You might want to change some of the defaults if something goes wrong
 * during the compilation.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

// obligatory flags
#define QT_NO_STL 1
#define QT_NO_KEYWORDS 1
#define HAVE_ICONV 1

#include "configCompiler.h"

#ifdef LYX_ENABLE_PCH
#include "pcheaders.h"
#endif


#cmakedefine LYX_DEVEL_VERSION 1
#if defined(LYX_DEVEL_VERSION)
	#define DEVEL_VERSION 1
#else
	#undef DEVEL_VERSION
#endif
#cmakedefine PACKAGE "${PACKAGE}"
#cmakedefine PACKAGE_VERSION "${PACKAGE_VERSION}"
#define PACKAGE_STRING "LyX ${PACKAGE_VERSION}"
#cmakedefine PACKAGE_BUGREPORT "${PACKAGE_BUGREPORT}"
#cmakedefine VERSION_INFO "${VERSION_INFO}"
#cmakedefine LYX_DIR_VER "${LYX_DIR_VER}"
#cmakedefine LYX_USERDIR_VER "${LYX_USERDIR_VER}"
#define LYX_MAJOR_VERSION ${LYX_MAJOR_VERSION}
#define LYX_MINOR_VERSION ${LYX_MINOR_VERSION}

#define PROGRAM_SUFFIX "${PROGRAM_SUFFIX}"
#define LYX_ABS_INSTALLED_DATADIR "${LYX_ABS_INSTALLED_DATADIR}"
#define LYX_ABS_INSTALLED_LOCALEDIR "${LYX_ABS_INSTALLED_LOCALEDIR}"
#define LYX_ABS_TOP_SRCDIR "${TOP_SRC_DIR}"

#cmakedefine USE_POSIX_PACKAGING 1
#cmakedefine USE_WINDOWS_PACKAGING 1
#cmakedefine USE_MACOSX_PACKAGING 1
#cmakedefine PATH_MAX ${PATH_MAX}

#cmakedefine WORDS_BIGENDIAN 1

#cmakedefine LYX_MERGE_FILES 1

#cmakedefine LYX_USE_TR1 1
#cmakedefine LYX_USE_TR1_REGEX 1

#cmakedefine Z_PREFIX 1

${Include_used_spellchecker}

#cmakedefine AIKSAURUSLIB_FOUND 1
#ifdef AIKSAURUSLIB_FOUND
#define HAVE_LIBAIKSAURUS 1
#define AIKSAURUS_H_LOCATION "${AIKSAURUSLIB_H}"
#endif

#cmakedefine LYX_NLS 1
#ifdef LYX_NLS
#define ENABLE_NLS 1
#endif


#endif // config.h guard



// Unguarded cleanup of global namespace:

#ifdef ColorMode
#undef ColorMode
#endif

#ifdef FocusOut
#undef FocusOut
#endif

#ifdef FocusIn
#undef FocusIn
#endif

#ifdef KeyRelease
#undef KeyRelease
#endif

#ifdef CursorShape
#undef CursorShape
#endif

#ifdef IGNORE
#undef IGNORE
#endif

#ifdef GrayScale
#undef GrayScale
#endif

#ifdef Status
#undef Status
#endif

#ifdef IN
#undef IN
#endif

#ifdef KeyPress
#undef KeyPress
#endif
