#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# file configure.py
# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.

# \author Bo Peng
# Full author contact details are available in file CREDITS.

import glob, logging, os, re, shutil, subprocess, sys, stat

# set up logging
logging.basicConfig(level = logging.DEBUG,
    format = '%(levelname)s: %(message)s', # ignore application name
    filename = 'configure.log',
    filemode = 'w')
#
# Add a handler to log to console
console = logging.StreamHandler()
console.setLevel(logging.INFO) # the console only print out general information
formatter = logging.Formatter('%(message)s') # only print out the message itself
console.setFormatter(formatter)
logger = logging.getLogger('LyX')
logger.addHandler(console)

def writeToFile(filename, lines, append = False):
    " utility function: write or append lines to filename "
    if append:
        file = open(filename, 'a')
    else:
        file = open(filename, 'w')
    file.write(lines)
    file.close()


def addToRC(lines):
    ''' utility function: shortcut for appending lines to outfile
        add newline at the end of lines.
    '''
    if lines.strip() != '':
        writeToFile(outfile, lines + '\n', append = True)
        logger.debug('Add to RC:\n' + lines + '\n\n')


def removeFiles(filenames):
    '''utility function: 'rm -f'
        ignore errors when file does not exist, or is a directory.
    '''
    for file in filenames:
        try:
            os.remove(file)
            logger.debug('Removing file %s' % file)
        except:
            logger.debug('Failed to remove file %s' % file)
            pass


def cmdOutput(cmd, async = False):
    '''utility function: run a command and get its output as a string
        cmd: command to run
        async: if False, return whole output as a string, otherwise
               return the stdout handle from which the output can be
               read (the caller is then responsible for closing it)
    '''
    if os.name == 'nt':
        b = False
        cmd = 'cmd /d /c pushd ' + shortPath(os.getcwdu()) + '&' + cmd
    else:
        b = True
    pipe = subprocess.Popen(cmd, shell=b, close_fds=b, stdin=subprocess.PIPE, \
                            stdout=subprocess.PIPE, universal_newlines=True)
    pipe.stdin.close()
    if async:
        return pipe.stdout
    output = pipe.stdout.read()
    pipe.stdout.close()
    return output.strip()


def shortPath(path):
    ''' On Windows, return the short version of "path" if possible '''
    if os.name == 'nt':
        from ctypes import windll, create_unicode_buffer
        GetShortPathName = windll.kernel32.GetShortPathNameW
        shortlen = GetShortPathName(path, 0, 0)
        shortpath = create_unicode_buffer(shortlen)
        if GetShortPathName(path, shortpath, shortlen):
            return shortpath.value
    return path


def setEnviron():
    ''' I do not really know why this is useful, but we might as well keep it.
        NLS nuisances.
        Only set these to C if already set.  These must not be set unconditionally
        because not all systems understand e.g. LANG=C (notably SCO).
        Fixing LC_MESSAGES prevents Solaris sh from translating var values in set!
        Non-C LC_CTYPE values break the ctype check.
    '''
    os.environ['LANG'] = os.getenv('LANG', 'C')
    os.environ['LC'] = os.getenv('LC_ALL', 'C')
    os.environ['LC_MESSAGE'] = os.getenv('LC_MESSAGE', 'C')
    os.environ['LC_CTYPE'] = os.getenv('LC_CTYPE', 'C')


def copy_tree(src, dst, preserve_symlinks=False, level=0):
    ''' Copy an entire directory tree 'src' to a new location 'dst'.
 
    Code inspired from distutils.copy_tree.
     Copying ignores non-regular files and the cache directory.
    Pipes may be present as leftovers from LyX for lyx-server.

    If 'preserve_symlinks' is true, symlinks will be
    copied as symlinks (on platforms that support them!); otherwise
    (the default), the destination of the symlink will be copied.
    '''
 
    if not os.path.isdir(src):
        raise FileError, \
              "cannot copy tree '%s': not a directory" % src
    try:
        names = os.listdir(src)
    except os.error, (errno, errstr):
        raise FileError, \
              "error listing files in '%s': %s" % (src, errstr)
 
    if not os.path.isdir(dst):
        os.makedirs(dst)
 
    outputs = []
 
    for name in names:
        src_name = os.path.join(src, name)
        dst_name = os.path.join(dst, name)
        if preserve_symlinks and os.path.islink(src_name):
            link_dest = os.readlink(src_name)
            os.symlink(link_dest, dst_name)
            outputs.append(dst_name)
        elif level == 0 and name == 'cache':
            logger.info("Skip cache %s", src_name)
        elif os.path.isdir(src_name):
            outputs.extend(
                copy_tree(src_name, dst_name, preserve_symlinks, level=(level + 1)))
        elif stat.S_ISREG(os.stat(src_name).st_mode) or os.path.islink(src_name):
            shutil.copy2(src_name, dst_name)
            outputs.append(dst_name)
        else:
            logger.info("Ignore non-regular file %s", src_name)
 
    return outputs


def checkUpgrade():
    ''' Check for upgrade from previous version '''
    cwd = os.getcwd()
    basename = os.path.basename( cwd )
    lyxrc = os.path.join(cwd, outfile)
    if not os.path.isfile( lyxrc ) and basename.endswith( version_suffix ) :
        logger.info('Checking for upgrade from previous version.')
        parent = os.path.dirname(cwd)
        appname = basename[:(-len(version_suffix))]
        for version in ['-2.1', '-2.0', '-1.6' ]:
            logger.debug('Checking for upgrade from previous version ' + version)
            previous = os.path.join(parent, appname + version)
            logger.debug('previous = ' + previous)
            if os.path.isdir( previous ):
                logger.info('Found directory "%s".', previous)
                copy_tree( previous, cwd )
                logger.info('Content copied to directory "%s".', cwd)
                return


def createDirectories():
    ''' Create the build directories if necessary '''
    for dir in ['bind', 'clipart', 'doc', 'examples', 'images', 'kbd', \
        'layouts', 'scripts', 'templates', 'ui' ]:
        if not os.path.isdir( dir ):
            try:
                os.mkdir( dir)
                logger.debug('Create directory %s.' % dir)
            except:
                logger.error('Failed to create directory %s.' % dir)
                sys.exit(1)


def checkTeXPaths():
    ''' Determine the path-style needed by the TeX engine on Win32 (Cygwin) '''
    windows_style_tex_paths = ''
    if LATEX == '':
        return windows_style_tex_paths
    if os.name == 'nt' or sys.platform == 'cygwin':
        from tempfile import mkstemp
        fd, tmpfname = mkstemp(suffix='.ltx')
        if os.name == 'nt':
            from locale import getdefaultlocale
            language, encoding = getdefaultlocale()
            if encoding == None:
                encoding = 'latin1'
            inpname = shortPath(unicode(tmpfname, encoding)).replace('\\', '/')
        else:
            inpname = cmdOutput('cygpath -m ' + tmpfname)
        logname = os.path.basename(re.sub("(?i).ltx", ".log", inpname))
        inpname = inpname.replace('~', '\\string~')
        os.write(fd, r'\relax')
        os.close(fd)
        latex_out = cmdOutput(r'latex "\nonstopmode\input{%s}\makeatletter\@@end"' % inpname)
        if 'Error' in latex_out:
            latex_out = cmdOutput(r'latex "\nonstopmode\input{\"%s\"}\makeatletter\@@end"' % inpname)
        if 'Error' in latex_out:
            logger.warning("configure: TeX engine needs posix-style paths in latex files")
            windows_style_tex_paths = 'false'
        else:
            logger.info("configure: TeX engine needs windows-style paths in latex files")
            windows_style_tex_paths = 'true'
        removeFiles([tmpfname, logname, 'texput.log'])
    return windows_style_tex_paths


## Searching some useful programs
def checkProg(description, progs, rc_entry = [], path = [], not_found = ''):
    '''
        This function will search a program in $PATH plus given path
        If found, return directory and program name (not the options).

        description: description of the program

        progs: check programs, for each prog, the first word is used
            for searching but the whole string is used to replace
            %% for a rc_entry. So, feel free to add '$$i' etc for programs.

        path: additional pathes

        rc_entry: entry to outfile, can be
            1. emtpy: no rc entry will be added
            2. one pattern: %% will be replaced by the first found program,
                or '' if no program is found.
            3. several patterns for each prog and not_found. This is used
                when different programs have different usages. If you do not
                want not_found entry to be added to the RC file, you can specify
                an entry for each prog and use '' for the not_found entry.

        not_found: the value that should be used instead of '' if no program
            was found

    '''
    # one rc entry for each progs plus not_found entry
    if len(rc_entry) > 1 and len(rc_entry) != len(progs) + 1:
        logger.error("rc entry should have one item or item for each prog and not_found.")
        sys.exit(2)
    logger.info('checking for ' + description + '...')
    ## print '(' + ','.join(progs) + ')',
    global java, perl
    for idx in range(len(progs)):
        # ac_prog may have options, ac_word is the command name
        ac_prog = progs[idx]
        ac_word = ac_prog.split(' ')[0]
        if (ac_word.endswith('.class') or ac_word.endswith('.jar')) and java == '':
            continue
        if ac_word.endswith('.pl') and perl == '':
            continue
        msg = '+checking for "' + ac_word + '"... '
        path = os.environ["PATH"].split(os.pathsep) + path
        extlist = ['']
        if "PATHEXT" in os.environ:
            extlist = extlist + os.environ["PATHEXT"].split(os.pathsep)
        for ac_dir in path:
            if hasattr(os, "access") and not os.access(ac_dir, os.F_OK):
                continue
            for ext in extlist:
                if os.path.isfile( os.path.join(ac_dir, ac_word + ext) ):
                    logger.info(msg + ' yes')
                    # deal with java and perl
                    if ac_word.endswith('.class'):
                        ac_prog = ac_prog.replace(ac_word, r'%s \"%s\"' % (java, os.path.join(ac_dir, ac_word[:-6])))
                    elif ac_word.endswith('.jar'):
                        ac_prog = ac_prog.replace(ac_word, r'%s -jar \"%s\"' % (java, os.path.join(ac_dir, ac_word)))
                    elif ac_word.endswith('.pl'):
                        ac_prog = ac_prog.replace(ac_word, r'%s -w \"%s\"' % (perl, os.path.join(ac_dir, ac_word)))
                    # write rc entries for this command
                    if len(rc_entry) == 1:
                        addToRC(rc_entry[0].replace('%%', ac_prog))
                    elif len(rc_entry) > 1:
                        addToRC(rc_entry[idx].replace('%%', ac_prog))
                    return [ac_dir, ac_word]
        # if not successful
        logger.info(msg + ' no')
    # write rc entries for 'not found'
    if len(rc_entry) > 0:  # the last one.
        addToRC(rc_entry[-1].replace('%%', not_found))
    return ['', not_found]


def checkProgAlternatives(description, progs, rc_entry = [], alt_rc_entry = [], path = [], not_found = ''):
    '''
        The same as checkProg, but additionally, all found programs will be added
        as alt_rc_entries
    '''
    # one rc entry for each progs plus not_found entry
    if len(rc_entry) > 1 and len(rc_entry) != len(progs) + 1:
        logger.error("rc entry should have one item or item for each prog and not_found.")
        sys.exit(2)
    logger.info('checking for ' + description + '...')
    ## print '(' + ','.join(progs) + ')',
    found_prime = False
    real_ac_dir = ''
    real_ac_word = not_found
    global java, perl
    for idx in range(len(progs)):
        # ac_prog may have options, ac_word is the command name
        ac_prog = progs[idx]
        ac_word = ac_prog.split(' ')[0]
        if (ac_word.endswith('.class') or ac_word.endswith('.jar')) and java == '':
            continue
        if ac_word.endswith('.pl') and perl == '':
            continue
        msg = '+checking for "' + ac_word + '"... '
        path = os.environ["PATH"].split(os.pathsep) + path
        extlist = ['']
        if "PATHEXT" in os.environ:
            extlist = extlist + os.environ["PATHEXT"].split(os.pathsep)
        found_alt = False
        for ac_dir in path:
            if hasattr(os, "access") and not os.access(ac_dir, os.F_OK):
                continue
            for ext in extlist:
                if os.path.isfile( os.path.join(ac_dir, ac_word + ext) ):
                    logger.info(msg + ' yes')
                    pr = re.compile(r'(\\\S+)(.*)$')
                    m = None
                    # deal with java and perl
                    if ac_word.endswith('.class'):
                        ac_prog = ac_prog.replace(ac_word, r'%s \"%s\"' % (java, os.path.join(ac_dir, ac_word[:-6])))
                    elif ac_word.endswith('.jar'):
                        ac_prog = ac_prog.replace(ac_word, r'%s -jar \"%s\"' % (java, os.path.join(ac_dir, ac_word)))
                    elif ac_word.endswith('.pl'):
                        ac_prog = ac_prog.replace(ac_word, r'%s -w \"%s\"' % (perl, os.path.join(ac_dir, ac_word)))
                    # write rc entries for this command
                    if found_prime == False:
                        if len(rc_entry) == 1:
                            addToRC(rc_entry[0].replace('%%', ac_prog))
                        elif len(rc_entry) > 1:
                            addToRC(rc_entry[idx].replace('%%', ac_prog))
                        real_ac_dir = ac_dir
                        real_ac_word = ac_word
                        found_prime = True
                    if len(alt_rc_entry) == 1:
                        alt_rc = alt_rc_entry[0]
                        if alt_rc == "":
                            # if no explicit alt_rc is given, construct one
                            m = pr.match(rc_entry[0])
                            if m:
                                alt_rc = m.group(1) + "_alternatives" + m.group(2)
                        addToRC(alt_rc.replace('%%', ac_prog))
                    elif len(alt_rc_entry) > 1:
                        alt_rc = alt_rc_entry[idx]
                        if alt_rc == "":
                            # if no explicit alt_rc is given, construct one
                            m = pr.match(rc_entry[idx])
                            if m:
                                alt_rc = m.group(1) + "_alternatives" + m.group(2)
                        addToRC(alt_rc.replace('%%', ac_prog))
                    found_alt = True
                    break
            if found_alt:
                break
        if found_alt == False:
            # if not successful
            logger.info(msg + ' no')
    if found_prime:
        return [real_ac_dir, real_ac_word]
    # write rc entries for 'not found'
    if len(rc_entry) > 0:  # the last one.
        addToRC(rc_entry[-1].replace('%%', not_found))
    return ['', not_found]


def addAlternatives(rcs, alt_type):
    '''
        Returns a \\prog_alternatives string to be used as an alternative
        rc entry.  alt_type can be a string or a list of strings.
    '''
    r = re.compile(r'\\Format (\S+).*$')
    m = None
    alt = ''
    alt_token = '\\%s_alternatives '
    if isinstance(alt_type, str):
        alt_tokens = [alt_token % alt_type]
    else:
        alt_tokens = map(lambda s: alt_token % s, alt_type)
    for idxx in range(len(rcs)):
        if len(rcs) == 1:
            m = r.match(rcs[0])
            if m:
                alt = '\n'.join([s + m.group(1) + ' "%%"' for s in alt_tokens])
        elif len(rcs) > 1:
            m = r.match(rcs[idxx])
            if m:
                if idxx > 0:
                    alt += '\n'
                alt += '\n'.join([s + m.group(1) + ' "%%"' for s in alt_tokens])
    return alt


def listAlternatives(progs, alt_type, rc_entry = []):
    '''
        Returns a list of \\prog_alternatives strings to be used as alternative
        rc entries.  alt_type can be a string or a list of strings.
    '''
    if len(rc_entry) > 1 and len(rc_entry) != len(progs) + 1:
        logger.error("rc entry should have one item or item for each prog and not_found.")
        sys.exit(2)
    alt_rc_entry = []
    for idx in range(len(progs)):
        if len(rc_entry) == 1:
            rcs = rc_entry[0].split('\n')
            alt = addAlternatives(rcs, alt_type)
            alt_rc_entry.insert(0, alt)
        elif len(rc_entry) > 1:
            rcs = rc_entry[idx].split('\n')
            alt = addAlternatives(rcs, alt_type)
            alt_rc_entry.insert(idx, alt)
    return alt_rc_entry


def checkViewer(description, progs, rc_entry = [], path = []):
    ''' The same as checkProgAlternatives, but for viewers '''
    alt_rc_entry = listAlternatives(progs, 'viewer', rc_entry)
    return checkProgAlternatives(description, progs, rc_entry, alt_rc_entry, path, not_found = 'auto')


def checkEditor(description, progs, rc_entry = [], path = []):
    ''' The same as checkProgAlternatives, but for editors '''
    alt_rc_entry = listAlternatives(progs, 'editor', rc_entry)
    return checkProgAlternatives(description, progs, rc_entry, alt_rc_entry, path, not_found = 'auto')


def checkViewerNoRC(description, progs, rc_entry = [], path = []):
    ''' The same as checkViewer, but do not add rc entry '''
    alt_rc_entry = listAlternatives(progs, 'viewer', rc_entry)
    rc_entry = []
    return checkProgAlternatives(description, progs, rc_entry, alt_rc_entry, path, not_found = 'auto')


def checkEditorNoRC(description, progs, rc_entry = [], path = []):
    ''' The same as checkViewer, but do not add rc entry '''
    alt_rc_entry = listAlternatives(progs, 'editor', rc_entry)
    rc_entry = []
    return checkProgAlternatives(description, progs, rc_entry, alt_rc_entry, path, not_found = 'auto')


def checkViewerEditor(description, progs, rc_entry = [], path = []):
    ''' The same as checkProgAlternatives, but for viewers and editors '''
    alt_rc_entry = listAlternatives(progs, ['editor', 'viewer'], rc_entry)
    return checkProgAlternatives(description, progs, rc_entry, alt_rc_entry, path, not_found = 'auto')


def checkDTLtools():
    ''' Check whether DTL tools are available (Windows only) '''
    # Find programs! Returned path is not used now
    if ((os.name == 'nt' or sys.platform == 'cygwin') and
            checkProg('DVI to DTL converter', ['dv2dt']) != ['', ''] and
            checkProg('DTL to DVI converter', ['dt2dv']) != ['', '']):
        dtl_tools = True
    else:
        dtl_tools = False
    return dtl_tools


def checkLatex(dtl_tools):
    ''' Check latex, return lyx_check_config '''
    path, LATEX = checkProg('a Latex2e program', ['latex $$i', 'latex2e $$i'])
    path, PPLATEX = checkProg('a DVI postprocessing program', ['pplatex $$i'])
    #-----------------------------------------------------------------
    path, PLATEX = checkProg('pLaTeX, the Japanese LaTeX', ['platex $$i'])
    if PLATEX != '':
        # check if PLATEX is pLaTeX2e
        writeToFile('chklatex.ltx', r'\nonstopmode\makeatletter\@@end')
        # run platex on chklatex.ltx and check result
        if cmdOutput(PLATEX + ' chklatex.ltx').find('pLaTeX2e') != -1:
            # We have the Japanese pLaTeX2e
            addToRC(r'\converter platex   dvi       "%s"   "latex=platex"' % PLATEX)
        else:
            PLATEX = ''
            removeFiles(['chklatex.ltx', 'chklatex.log'])
    #-----------------------------------------------------------------
    # use LATEX to convert from latex to dvi if PPLATEX is not available
    if PPLATEX == '':
        PPLATEX = LATEX
    if dtl_tools:
        # Windows only: DraftDVI
        addToRC(r'''\converter latex      dvi2       "%s"   "latex"
\converter dvi2       dvi        "python -tt $$s/scripts/clean_dvi.py $$i $$o"  ""''' % PPLATEX)
    else:
        addToRC(r'\converter latex      dvi        "%s" "latex"' % PPLATEX)
    # no latex
    if LATEX != '':
        # Check if latex is usable
        writeToFile('chklatex.ltx', r'''
\nonstopmode
\ifx\undefined\documentclass\else
  \message{ThisIsLaTeX2e}
\fi
\makeatletter
\@@end
''')
        # run latex on chklatex.ltx and check result
        if cmdOutput(LATEX + ' chklatex.ltx').find('ThisIsLaTeX2e') != -1:
            # valid latex2e
            return LATEX
        else:
            logger.warning("Latex not usable (not LaTeX2e) ")
        # remove temporary files
        removeFiles(['chklatex.ltx', 'chklatex.log'])
    return ''


def checkLuatex():
    ''' Check if luatex is there and usable '''
    path, LUATEX = checkProg('LuaTeX', ['lualatex $$i'])
    path, DVILUATEX = checkProg('LuaTeX (DVI)', ['dvilualatex $$i'])
    if LUATEX != '':
    # luatex binary is there
        msg = "checking if LuaTeX is usable ..."
        # Check if luatex is usable
        writeToFile('luatest.tex', r'''
\nonstopmode
\documentclass{minimal}
\usepackage{fontspec}
\begin{document}
.
\end{document}
''')
        # run lualatex on luatest.tex and check result
        luatest = cmdOutput(LUATEX + ' luatest.tex')
        if luatest.find('XeTeX is required to compile this document') != -1:
            # fontspec/luatex too old! We do not support this version.
            logger.info(msg + ' no (probably not recent enough)')
        elif luatest.find('! LaTeX Error: File `fontspec.sty\' not found') != -1:
            # fontspec missing
            logger.info(msg + ' no (missing fontspec)')
        else:
            # working luatex
            logger.info(msg + ' yes')
            addToRC(r'\converter luatex      pdf5       "%s"    "latex=lualatex"' % LUATEX)
            if DVILUATEX != '':
                addToRC(r'\converter luatex      dvi3        "%s"   "latex=dvilualatex"' % DVILUATEX)
        # remove temporary files
        removeFiles(['luatest.tex', 'luatest.log', 'luatest.aux', 'luatest.pdf'])


def checkModule(module):
    ''' Check for a Python module, return the status '''
    msg = 'checking for "' + module + ' module"... '
    try:
      __import__(module)
      logger.info(msg + ' yes')
      return True
    except ImportError:
      logger.info(msg + ' no')
      return False


def checkFormatEntries(dtl_tools):
    ''' Check all formats (\Format entries) '''
    checkViewerEditor('a Tgif viewer and editor', ['tgif'],
        rc_entry = [r'\Format tgif      "obj, tgo" Tgif                 "" "%%" "%%"    "vector"    "application/x-tgif"'])
    #
    checkViewerEditor('a FIG viewer and editor', ['xfig', 'jfig3-itext.jar', 'jfig3.jar'],
        rc_entry = [r'\Format fig        fig     FIG                    "" "%%" "%%"    "vector"    "application/x-xfig"'])
    #
    checkViewerEditor('a Dia viewer and editor', ['dia'],
        rc_entry = [r'\Format dia        dia     DIA                    "" "%%" "%%"    "vector,zipped=native", "application/x-dia-diagram"'])
    #
    checkViewerEditor('an OpenOffice drawing viewer and editor', ['libreoffice', 'lodraw', 'ooffice', 'oodraw', 'soffice'],
        rc_entry = [r'\Format odg        "odg, sxd" "OpenOffice drawing"   "" "%%"  "%%"    "vector,zipped=native"  "application/vnd.oasis.opendocument.graphics"'])
    #
    checkViewerEditor('a Grace viewer and editor', ['xmgrace'],
        rc_entry = [r'\Format agr        agr     Grace                  "" "%%" "%%"    "vector"    ""'])
    #
    checkViewerEditor('a FEN viewer and editor', ['xboard -lpf $$i -mode EditPosition'],
        rc_entry = [r'\Format fen        fen     FEN                    "" "%%" "%%"    ""  ""'])
    #
    checkViewerEditor('a SVG viewer and editor', ['inkscape'],
        rc_entry = [r'\Format svg        svg     SVG                    "" "%%" "%%"    "vector"    "image/svg+xml"'])
    #
    imageformats = r'''\Format bmp        bmp     BMP                    "" "%s"    "%s"    ""  "image/x-bmp"
\Format gif        gif     GIF                    "" "%s"   "%s"    ""  "image/gif"
\Format jpg       "jpg, jpeg" JPEG                "" "%s"   "%s"    ""  "image/jpeg"
\Format pbm        pbm     PBM                    "" "%s"   "%s"    ""  "image/x-portable-bitmap"
\Format pgm        pgm     PGM                    "" "%s"   "%s"    ""  "image/x-portable-graymap"
\Format png        png     PNG                    "" "%s"   "%s"    ""  "image/x-png"
\Format ppm        ppm     PPM                    "" "%s"   "%s"    ""  "image/x-portable-pixmap"
\Format tiff       tif     TIFF                   "" "%s"   "%s"    ""  "image/tiff"
\Format xbm        xbm     XBM                    "" "%s"   "%s"    ""  "image/x-xbitmap"
\Format xpm        xpm     XPM                    "" "%s"   "%s"    ""  "image/x-xpixmap"'''
    path, iv = checkViewerNoRC('a raster image viewer', ['xv', 'kview', 'gimp-remote', 'gimp'], rc_entry = [imageformats])
    path, ie = checkEditorNoRC('a raster image editor', ['gimp-remote', 'gimp'], rc_entry = [imageformats])
    addToRC(imageformats % \
        (iv, ie, iv, ie, iv, ie, iv, ie, iv, ie, iv, ie, iv, ie, iv, ie, iv, ie, iv, ie) )
    #
    checkViewerEditor('a text editor', ['xemacs', 'gvim', 'kedit', 'kwrite', 'kate', \
        'nedit', 'gedit', 'notepad', 'geany', 'leafpad', 'mousepad'],
        rc_entry = [r'''\Format asciichess asc    "Plain text (chess output)"  "" ""    "%%"    ""  ""
\Format asciiimage asc    "Plain text (image)"         "" ""    "%%"    ""  ""
\Format asciixfig  asc    "Plain text (Xfig output)"   "" ""    "%%"    ""  ""
\Format dateout    tmp    "date (output)"         "" "" "%%"    ""  ""
\Format docbook    sgml    DocBook                B  "" "%%"    "document,menu=export"  ""
\Format docbook-xml xml   "DocBook (XML)"         "" "" "%%"    "document,menu=export"  "application/docbook+xml"
\Format dot        dot    "Graphviz Dot"          "" "" "%%"    "vector"    "text/vnd.graphviz"
\Format dviluatex  tex    "LaTeX (dviluatex)"     "" "" "%%"    "document,menu=export"  ""
\Format platex     tex    "LaTeX (pLaTeX)"        "" "" "%%"    "document,menu=export"  ""
\Format literate   nw      NoWeb                  N  "" "%%"    "document,menu=export"  ""
\Format sweave     Rnw    "Sweave"                S  "" "%%"    "document,menu=export"  ""
\Format r          R      "R/S code"              "" "" "%%"    "document,menu=export"  ""
\Format knitr      Rnw    "Rnw (knitr)"           "" "" "%%"    "document,menu=export"  ""
\Format lilypond   ly     "LilyPond music"        "" "" "%%"    "vector"    "text/x-lilypond"
\Format lilypond-book    lytex "LilyPond book (LaTeX)"   "" ""  "%%"    "document,menu=export"  ""
\Format latex      tex    "LaTeX (plain)"         L  "" "%%"    "document,menu=export"  "text/x-tex"
\Format luatex     tex    "LaTeX (LuaTeX)"        "" "" "%%"    "document,menu=export"  ""
\Format pdflatex   tex    "LaTeX (pdflatex)"      "" "" "%%"    "document,menu=export"  ""
\Format xetex      tex    "LaTeX (XeTeX)"         "" "" "%%"    "document,menu=export"  ""
\Format latexclipboard tex "LaTeX (clipboard)"    "" "" "%%"    ""  ""
\Format text       txt    "Plain text"            a  "" "%%"    "document,menu=export"  "text/plain"
\Format text2      txt    "Plain text (pstotext)" "" "" "%%"    "document"  ""
\Format text3      txt    "Plain text (ps2ascii)" "" "" "%%"    "document"  ""
\Format text4      txt    "Plain text (catdvi)"   "" "" "%%"    "document"  ""
\Format textparagraph txt "Plain Text, Join Lines" "" ""    "%%"    "document"  ""
\Format beamer.info pdf.info   "Info (Beamer)"         "" ""   "%%"    "document,menu=export"   ""''' ])
   #Spreadsheets using ssconvert from gnumeric
    checkViewer('gnumeric spreadsheet software', ['gnumeric'],
      rc_entry = [r'''\Format gnumeric gnumeric "Gnumeric spreadsheet" "" ""    "%%"   "document"   "application/x-gnumeric"
\Format excel      xls    "Excel spreadsheet"      "" "" "%%"    "document" "application/vnd.ms-excel"
\Format oocalc     ods    "OpenOffice spreadsheet" "" "" "%%"    "document" "application/vnd.oasis.opendocument.spreadsheet"'''])
 #
    checkViewer('an HTML previewer', ['firefox', 'mozilla file://$$p$$i', 'netscape'],
        rc_entry = [r'\Format xhtml      xhtml   "LyXHTML"              y "%%" ""    "document,menu=export" "application/xhtml+xml"'])
 #
    checkEditor('a BibTeX editor', ['jabref', 'JabRef', \
        'pybliographic', 'bibdesk', 'gbib', 'kbib', \
        'kbibtex', 'sixpack', 'bibedit', 'tkbibtex' \
        'xemacs', 'gvim', 'kedit', 'kwrite', 'kate', \
        'jedit', 'TeXnicCenter', 'WinEdt', 'WinShell', 'PSPad', \
        'nedit', 'gedit', 'notepad', 'geany', 'leafpad', 'mousepad'],
        rc_entry = [r'''\Format bibtex bib    "BibTeX"         "" ""    "%%"    ""  "text/x-bibtex"''' ])
    #
    #checkProg('a Postscript interpreter', ['gs'],
    #  rc_entry = [ r'\ps_command "%%"' ])
    checkViewer('a Postscript previewer', ['kghostview', 'okular', 'qpdfview --unique', 'evince', 'gv', 'ghostview -swap', 'gsview64', 'gsview32'],
        rc_entry = [r'''\Format eps        eps     EPS                    "" "%%"   ""  "vector"    "image/x-eps"
\Format eps2       eps    "EPS (uncropped)"       "" "%%"   ""  "vector"    ""
\Format eps3       eps    "EPS (cropped)"         "" "%%"   ""  "document,menu=export"  ""
\Format ps         ps      Postscript             t  "%%"   ""  "document,vector,menu=export"   "application/postscript"'''])
    # for xdg-open issues look here: http://www.mail-archive.com/lyx-devel@lists.lyx.org/msg151818.html
    # the MIME type is set for pdf6, because that one needs to be autodetectable by libmime
    checkViewer('a PDF previewer', ['pdfview', 'kpdf', 'okular', 'qpdfview --unique', 'evince', 'kghostview', 'xpdf', 'SumatraPDF', 'acrobat', 'acroread', 'mupdf', \
            'gv', 'ghostview', 'AcroRd32', 'gsview64', 'gsview32'],
        rc_entry = [r'''\Format pdf        pdf    "PDF (ps2pdf)"          P  "%%"   ""  "document,vector,menu=export"   ""
\Format pdf2       pdf    "PDF (pdflatex)"        F  "%%"   ""  "document,vector,menu=export"   ""
\Format pdf3       pdf    "PDF (dvipdfm)"         m  "%%"   ""  "document,vector,menu=export"   ""
\Format pdf4       pdf    "PDF (XeTeX)"           X  "%%"   ""  "document,vector,menu=export"   ""
\Format pdf5       pdf    "PDF (LuaTeX)"          u  "%%"   ""  "document,vector,menu=export"   ""
\Format pdf6       pdf    "PDF (graphics)"        "" "%%"   ""  "vector"    "application/pdf"
\Format pdf7       pdf    "PDF (cropped)"         "" "%%"   ""  "document,menu=export"  ""'''])
    #
    checkViewer('a DVI previewer', ['xdvi', 'kdvi', 'okular', 'yap', 'dviout -Set=!m'],
        rc_entry = [r'''\Format dvi        dvi     DVI                    D  "%%"   ""  "document,vector,menu=export"   "application/x-dvi"
\Format dvi3       dvi     "DVI (LuaTeX)"          V  "%%"  ""  "document,vector,menu=export"   ""'''])
    if dtl_tools:
        # Windows only: DraftDVI
        addToRC(r'\Format dvi2       dvi     DraftDVI               ""  ""  ""  "vector"    ""')
    #
    checkViewer('an HTML previewer', ['firefox', 'mozilla file://$$p$$i', 'netscape'],
        rc_entry = [r'\Format html      "html, htm" HTML                H  "%%" ""  "document,menu=export"  "text/html"'])
    #
    checkViewerEditor('Noteedit', ['noteedit'],
        rc_entry = [r'\Format noteedit   not     Noteedit               "" "%%" "%%"    "vector"    ""'])
    #
    checkViewerEditor('an OpenDocument/OpenOffice viewer', ['libreoffice', 'lwriter', 'lowriter', 'oowriter', 'swriter', 'abiword'],
        rc_entry = [r'''\Format odt        odt     OpenDocument           "" "%%"   "%%"    "document,vector,menu=export"   "application/vnd.oasis.opendocument.text"
\Format sxw        sxw    "OpenOffice.Org (sxw)"  "" "" ""  "document,vector"   "application/vnd.sun.xml.writer"'''])
    #
    checkViewerEditor('a Rich Text and Word viewer', ['libreoffice', 'lwriter', 'lowriter', 'oowriter', 'swriter', 'abiword'],
        rc_entry = [r'''\Format rtf        rtf    "Rich Text Format"      "" "%%"   "%%"    "document,vector,menu=export"   "application/rtf"
\Format word       doc    "MS Word"               W  "%%"   "%%"    "document,vector,menu=export"   "application/msword"'''])
    #
    # entries that do not need checkProg
    addToRC(r'''\Format date       ""     "date command"          "" "" ""  ""  ""
\Format csv        csv    "Table (CSV)"           "" "" ""  "document"  "text/csv"
\Format fax        ""      Fax                    "" "" ""  "document"  ""
\Format lyx        lyx     LyX                    "" "" ""  ""  "application/x-lyx"
\Format lyx13x     13.lyx "LyX 1.3.x"             "" "" ""  "document"  ""
\Format lyx14x     14.lyx "LyX 1.4.x"             "" "" ""  "document"  ""
\Format lyx15x     15.lyx "LyX 1.5.x"             "" "" ""  "document"  ""
\Format lyx16x     16.lyx "LyX 1.6.x"             "" "" ""  "document"  ""
\Format lyx20x     20.lyx "LyX 2.0.x"             "" "" ""  "document"  ""
\Format lyx21x     21.lyx "LyX 2.1.x"             "" "" ""  "document,menu=export"  ""
\Format clyx       cjklyx "CJK LyX 1.4.x (big5)"  "" "" ""  "document"  ""
\Format jlyx       cjklyx "CJK LyX 1.4.x (euc-jp)" "" ""    ""  "document"  ""
\Format klyx       cjklyx "CJK LyX 1.4.x (euc-kr)" "" ""    ""  "document"  ""
\Format lyxpreview lyxpreview "LyX Preview"       "" "" ""  ""  ""
\Format pdftex     pdftex_t PDFTEX                "" "" ""  ""  ""
\Format program    ""      Program                "" "" ""  ""  ""
\Format pstex      pstex_t PSTEX                  "" "" ""  ""  ""
\Format wmf        wmf    "Windows Metafile"      "" "" ""  "vector"    "image/x-wmf"
\Format emf        emf    "Enhanced Metafile"     "" "" ""  "vector"    "image/x-emf"
\Format wordhtml  "html, htm" "HTML (MS Word)"    "" "" ""  "document"  ""
''')


def checkConverterEntries():
    ''' Check all converters (\converter entries) '''
    checkProg('the pdflatex program', ['pdflatex $$i'],
        rc_entry = [ r'\converter pdflatex   pdf2       "%%"    "latex=pdflatex"' ])

    checkProg('the pdflatex program', ['pdflatex -shell-escape $$i'],
        rc_entry = [ r'\converter platex   png       "%%"    ""' ])

    checkProg('XeTeX', ['xelatex $$i'],
        rc_entry = [ r'\converter xetex      pdf4       "%%"    "latex=xelatex"' ])

    checkLuatex()

    # Look for tex2lyx in this order (see bugs #3308 and #6986):
    #   1)  If we're building LyX with autotools then tex2lyx is found
    #       in the subdirectory tex2lyx with respect to the binary dir.
    #   2)  If we're building LyX with cmake then tex2lyx is found
    #       in the binary dir.
    #   3)  If LyX was configured with a version suffix then tex2lyx
    #       will also have this version suffix.
    #   4)  Otherwise always use tex2lyx.
    in_binary_subdir = os.path.join(lyx_binary_dir, 'tex2lyx', 'tex2lyx')
    in_binary_subdir = os.path.abspath(in_binary_subdir)

    in_binary_dir = os.path.join(lyx_binary_dir, 'tex2lyx')
    in_binary_dir = os.path.abspath(in_binary_dir)

    path, t2l = checkProg('a LaTeX/Noweb -> LyX converter', [in_binary_subdir, in_binary_subdir + version_suffix, in_binary_dir, in_binary_dir + version_suffix, 'tex2lyx' + version_suffix, 'tex2lyx'],
        rc_entry = [r'''\converter latex      lyx        "%% -f $$i $$o"    ""
\converter latexclipboard lyx        "%% -fixedenc utf8 -f $$i $$o" ""
\converter literate   lyx        "%% -n -m noweb -f $$i $$o"    ""'''], not_found = 'tex2lyx')
    if path == '':
        logger.warning("Failed to find tex2lyx on your system.")

    #
    checkProg('a Noweb -> LaTeX converter', ['noweave -delay -index $$i > $$o'],
        rc_entry = [r'''\converter literate   latex      "%%"   ""
\converter literate   pdflatex      "%%"    ""
\converter literate   xetex         "%%"    ""
\converter literate   luatex        "%%"    ""'''])
    #
    checkProg('a Sweave -> LaTeX converter', ['Rscript --verbose --no-save --no-restore $$s/scripts/lyxsweave.R $$p$$i $$p$$o $$e $$r'],
        rc_entry = [r'''\converter sweave   latex      "%%" ""
\converter sweave   pdflatex   "%%" ""
\converter sweave   xetex      "%%" ""
\converter sweave   luatex     "%%" ""'''])
    #
    checkProg('a knitr -> LaTeX converter', ['Rscript --verbose --no-save --no-restore $$s/scripts/lyxknitr.R $$p$$i $$p$$o $$e $$r'],
        rc_entry = [r'''\converter knitr   latex      "%%"  ""
\converter knitr   pdflatex   "%%"  ""
\converter knitr   xetex      "%%"  ""
\converter knitr   luatex     "%%"  ""'''])
    #
    checkProg('a Sweave -> R/S code converter', ['Rscript --verbose --no-save --no-restore $$s/scripts/lyxstangle.R $$i $$e $$r'], 
        rc_entry = [ r'\converter sweave      r      "%%"    ""' ])
    #
    checkProg('a knitr -> R/S code converter', ['Rscript --verbose --no-save --no-restore $$s/scripts/lyxknitr.R $$p$$i $$p$$o $$e $$r tangle'],
        rc_entry = [ r'\converter knitr      r      "%%"    ""' ])
    #
    checkProg('an HTML -> LaTeX converter', ['html2latex $$i', 'gnuhtml2latex',
        'htmltolatex -input $$i -output $$o', 'htmltolatex.jar -input $$i -output $$o'],
        rc_entry = [ r'\converter html       latex      "%%"    ""', \
                     r'\converter html       latex      "python -tt $$s/scripts/html2latexwrapper.py %% $$i $$o"    ""', \
                     r'\converter html       latex      "%%"    ""', \
                     r'\converter html       latex      "%%"    ""', '' ])
    #
    checkProg('an MS Word -> LaTeX converter', ['wvCleanLatex $$i $$o'],
        rc_entry = [ r'\converter word       latex      "%%"    ""' ])

    # eLyXer: search as an executable (elyxer.py, elyxer)
    path, elyxer = checkProg('a LyX -> HTML converter',
        ['elyxer.py --directory $$r $$i $$o', 'elyxer --directory $$r $$i $$o'],
        rc_entry = [ r'\converter lyx      html       "%%"  ""' ])
    path, elyxer = checkProg('a LyX -> HTML (MS Word) converter',
        ['elyxer.py --html --directory $$r $$i $$o', 'elyxer --html --directory $$r $$i $$o'],
        rc_entry = [ r'\converter lyx      wordhtml       "%%"  ""' ])
    if elyxer.find('elyxer') >= 0:
      addToRC(r'''\copier    html       "python -tt $$s/scripts/ext_copy.py -e html,png,jpg,jpeg,css $$i $$o"''')
      addToRC(r'''\copier    wordhtml       "python -tt $$s/scripts/ext_copy.py -e html,png,jpg,jpeg,css $$i $$o"''')
    else:
      # search for HTML converters other than eLyXer
      # On SuSE the scripts have a .sh suffix, and on debian they are in /usr/share/tex4ht/
      path, htmlconv = checkProg('a LaTeX -> HTML converter', ['htlatex $$i', 'htlatex.sh $$i', \
          '/usr/share/tex4ht/htlatex $$i', 'tth  -t -e2 -L$$b < $$i > $$o', \
          'latex2html -no_subdir -split 0 -show_section_numbers $$i', 'hevea -s $$i'],
          rc_entry = [ r'\converter latex      html       "%%"  "needaux"' ])
      if htmlconv.find('htlatex') >= 0 or htmlconv == 'latex2html':
        addToRC(r'''\copier    html       "python -tt $$s/scripts/ext_copy.py -e html,png,css $$i $$o"''')
      else:
        addToRC(r'''\copier    html       "python -tt $$s/scripts/ext_copy.py $$i $$o"''')
      path, htmlconv = checkProg('a LaTeX -> HTML (MS Word) converter', ["htlatex $$i 'html,word' 'symbol/!' '-cvalidate'", \
          "htlatex.sh $$i 'html,word' 'symbol/!' '-cvalidate'", \
          "/usr/share/tex4ht/htlatex $$i 'html,word' 'symbol/!' '-cvalidate'"],
          rc_entry = [ r'\converter latex      wordhtml   "%%"  "needaux"' ])
      if htmlconv.find('htlatex') >= 0:
        addToRC(r'''\copier    wordhtml       "python -tt $$s/scripts/ext_copy.py -e html,png,css $$i $$o"''')
      else:
        addToRC(r'''\copier    wordhtml       "python -tt $$s/scripts/ext_copy.py $$i $$o"''')


    # Check if LyXBlogger is installed
    lyxblogger_found = checkModule('lyxblogger')
    if lyxblogger_found:
      addToRC(r'\Format    blog       blog       "LyXBlogger"           "" "" ""  "document"  ""')
      addToRC(r'\converter xhtml      blog       "python -m lyxblogger $$i"       ""')

    #
    checkProg('an OpenOffice.org -> LaTeX converter', ['w2l -clean $$i'],
        rc_entry = [ r'\converter sxw        latex      "%%"    ""' ])
    #
    checkProg('an OpenDocument -> LaTeX converter', ['w2l -clean $$i'],
        rc_entry = [ r'\converter odt        latex      "%%"    ""' ])
    # Only define a converter to pdf6, otherwise the odt format could be
    # used as an intermediate step for export to pdf, which is not wanted.
    checkProg('an OpenDocument -> PDF converter', ['unoconv -f pdf --stdout $$i > $$o'],
        rc_entry = [ r'\converter odt        pdf6       "%%"    ""' ])
    # According to http://www.tug.org/applications/tex4ht/mn-commands.html
    # the command mk4ht oolatex $$i has to be used as default,
    # but as this would require to have Perl installed, in MiKTeX oolatex is
    # directly available as application.
    # On SuSE the scripts have a .sh suffix, and on debian they are in /usr/share/tex4ht/
    # Both SuSE and debian have oolatex
    checkProg('a LaTeX -> Open Document converter', [
        'oolatex $$i', 'mk4ht oolatex $$i', 'oolatex.sh $$i', '/usr/share/tex4ht/oolatex $$i',
        'htlatex $$i \'xhtml,ooffice\' \'ooffice/! -cmozhtf\' \'-coo\' \'-cvalidate\''],
        rc_entry = [ r'\converter latex      odt        "%%"    "needaux"' ])
    # On windows it is called latex2rt.exe
    checkProg('a LaTeX -> RTF converter', ['latex2rtf -p -S -o $$o $$i', 'latex2rt -p -S -o $$o $$i'],
        rc_entry = [ r'\converter latex      rtf        "%%"    "needaux"' ])
    #
    checkProg('a RTF -> HTML converter', ['unrtf --html  $$i > $$o'],
        rc_entry = [ r'\converter rtf      html        "%%" ""' ])
    # Do not define a converter to pdf6, ps is a pure export format 
    checkProg('a PS to PDF converter', ['ps2pdf $$i $$o'],
        rc_entry = [ r'\converter ps         pdf        "%%"    ""' ])
    #
    checkProg('a PS to TXT converter', ['pstotext $$i > $$o'],
        rc_entry = [ r'\converter ps         text2      "%%"    ""' ])
    #
    checkProg('a PS to TXT converter', ['ps2ascii $$i $$o'],
        rc_entry = [ r'\converter ps         text3      "%%"    ""' ])
    # Need to call ps2eps in a pipe, otherwise it would name the output file
    # depending on the extension of the input file. We do not know the input
    # file extension in general, so the resultfile= flag would not help.
    # Since ps2eps crops the image, we do not use it to convert from ps->eps.
    # This would create additional paths in the converter graph with unwanted
    # side effects (e.g. ps->pdf via ps2pdf would create a different result
    # than ps->eps->pdf via ps2eps and epstopdf).
    checkProg('a PS to EPS converter', ['ps2eps -- < $$i > $$o'],
        rc_entry = [ r'\converter eps2       eps      "%%"  ""' ])
    #
    checkProg('a PDF to PS converter', ['pdf2ps $$i $$o', 'pdftops $$i $$o'],
        rc_entry = [ r'\converter pdf         ps        "%%"    ""' ])
    # Only define a converter from pdf6 for graphics
    checkProg('a PDF to EPS converter', ['pdftops -eps -f 1 -l 1 $$i $$o'],
        rc_entry = [ r'\converter pdf6        eps        "%%"   ""' ])
    #
    checkProg('a PDF cropping tool', ['pdfcrop $$i $$o'],
        rc_entry = [ r'\converter pdf2   pdf7       "%%"    ""' ])
    #
    checkProg('a Beamer info extractor', ['makebeamerinfo -p $$i'],
        rc_entry = [ r'\converter pdf2         beamer.info        "%%"  ""' ])
    #
    checkProg('a DVI to TXT converter', ['catdvi $$i > $$o'],
        rc_entry = [ r'\converter dvi        text4      "%%"    ""' ])
    #
    checkProg('a DVI to PS converter', ['dvips -o $$o $$i'],
        rc_entry = [ r'\converter dvi        ps         "%%"    ""' ])
    #
    checkProg('a DVI to cropped EPS converter', ['dvips -E -o $$o $$i'],
        rc_entry = [ r'\converter dvi        eps3         "%%"  ""' ])
    #
    checkProg('a DVI to PDF converter', ['dvipdfmx -o $$o $$i', 'dvipdfm -o $$o $$i'],
        rc_entry = [ r'\converter dvi        pdf3       "%%"    ""' ])
    #
    checkProg('a fax program', ['kdeprintfax $$i', 'ksendfax $$i', 'hylapex $$i'],
        rc_entry = [ r'\converter ps         fax        "%%"    ""'])
    #
    path, fig2dev = checkProg('a FIG -> Image converter', ['fig2dev'])
    if fig2dev == "fig2dev":
        addToRC(r'''\converter fig        eps        "fig2dev -L eps $$i $$o"   ""
\converter fig        ppm        "fig2dev -L ppm $$i $$o"   ""
\converter fig        png        "fig2dev -L png $$i $$o"   ""
\converter fig        pdftex     "python -tt $$s/scripts/fig2pdftex.py $$i $$o" ""
\converter fig        pstex      "python -tt $$s/scripts/fig2pstex.py $$i $$o"  ""''')
    #
    checkProg('a TIFF -> PS converter', ['tiff2ps $$i > $$o'],
        rc_entry = [ r'\converter tiff       eps        "%%"    ""'])
    #
    checkProg('a TGIF -> EPS/PPM converter', ['tgif'],
        rc_entry = [
            r'''\converter tgif       eps        "tgif -print -color -eps -stdout $$i > $$o"    ""
\converter tgif       png        "tgif -print -color -png -o $$d $$i"   ""
\converter tgif       pdf6       "tgif -print -color -pdf -stdout $$i > $$o"    ""'''])
    #
    checkProg('a WMF -> EPS converter', ['metafile2eps $$i $$o', 'wmf2eps -o $$o $$i'],
        rc_entry = [ r'\converter wmf        eps        "%%"    ""'])
    #
    checkProg('an EMF -> EPS converter', ['metafile2eps $$i $$o', 'wmf2eps -o $$o $$i'],
        rc_entry = [ r'\converter emf        eps        "%%"    ""'])
    # Only define a converter to pdf6 for graphics
    checkProg('an EPS -> PDF converter', ['epstopdf'],
        rc_entry = [ r'\converter eps        pdf6       "epstopdf --outfile=$$o $$i"    ""'])
    #
    checkProg('an EPS -> PNG converter', ['convert $$i $$o'],
        rc_entry = [ r'\converter eps        png        "%%"    ""'])
    #
    # no agr -> pdf6 converter, since the pdf library used by gracebat is not
    # free software and therefore not compiled in in many installations.
    # Fortunately, this is not a big problem, because we will use epstopdf to
    # convert from agr to pdf6 via eps without loss of quality.
    checkProg('a Grace -> Image converter', ['gracebat'],
        rc_entry = [
            r'''\converter agr        eps        "gracebat -hardcopy -printfile $$o -hdevice EPS $$i 2>/dev/null"   ""
\converter agr        png        "gracebat -hardcopy -printfile $$o -hdevice PNG $$i 2>/dev/null"   ""
\converter agr        jpg        "gracebat -hardcopy -printfile $$o -hdevice JPEG $$i 2>/dev/null"  ""
\converter agr        ppm        "gracebat -hardcopy -printfile $$o -hdevice PNM $$i 2>/dev/null"   ""'''])
    #
    checkProg('a Dot -> Image converter', ['dot'],
        rc_entry = [
            r'''\converter dot        eps        "dot -Teps $$i -o $$o" ""
\converter dot        png        "dot -Tpng $$i -o $$o" ""'''])
    #
    checkProg('a Dia -> PNG converter', ['dia -e $$o -t png $$i'],
        rc_entry = [ r'\converter dia        png        "%%"    ""'])
    #
    checkProg('a Dia -> EPS converter', ['dia -e $$o -t eps $$i'],
        rc_entry = [ r'\converter dia        eps        "%%"    ""'])
    # Actually, this produces EPS, but with a wrong bounding box (usually A4 or letter).
    # The eps2->eps converter then fixes the bounding box by cropping.
    # Although unoconv can convert to png and pdf as well, do not define
    # odg->png and odg->pdf converters, since the bb would be too large as well.
    checkProg('an OpenOffice -> EPS converter', ['libreoffice -headless -nologo -convert-to eps $$i', 'unoconv -f eps --stdout $$i > $$o'],
        rc_entry = [ r'\converter odg        eps2       "%%"    ""'])
    # Only define a converter to pdf6 for graphics
    checkProg('a SVG -> PDF converter', ['rsvg-convert -f pdf -o $$o $$i', 'inkscape --file=$$i --export-area-drawing --without-gui --export-pdf=$$o'],
        rc_entry = [ r'\converter svg        pdf6       "%%"    ""'])
    #
    checkProg('a SVG -> EPS converter', ['rsvg-convert -f ps -o $$o $$i', 'inkscape --file=$$i --export-area-drawing --without-gui --export-eps=$$o'],
        rc_entry = [ r'\converter svg        eps        "%%"    ""'])
    #
    checkProg('a SVG -> PNG converter', ['rsvg-convert -f png -o $$o $$i', 'inkscape --without-gui --file=$$i --export-png=$$o'],
        rc_entry = [ r'\converter svg        png        "%%"    ""'])

    #
    # gnumeric/xls/ods to tex
    checkProg('a spreadsheet -> latex converter', ['ssconvert'],
       rc_entry = [ r'''\converter gnumeric latex "ssconvert --export-type=Gnumeric_html:latex $$i $$o" ""
\converter oocalc latex "ssconvert --export-type=Gnumeric_html:latex $$i $$o" ""
\converter excel  latex "ssconvert --export-type=Gnumeric_html:latex $$i $$o" ""'''])

    path, lilypond = checkProg('a LilyPond -> EPS/PDF/PNG converter', ['lilypond'])
    if (lilypond != ''):
        version_string = cmdOutput("lilypond --version")
        match = re.match('GNU LilyPond (\S+)', version_string)
        if match:
            version_number = match.groups()[0]
            version = version_number.split('.')
            if int(version[0]) > 2 or (len(version) > 1 and int(version[0]) == 2 and int(version[1]) >= 11):
                addToRC(r'''\converter lilypond   eps        "lilypond -dbackend=eps -dsafe --ps $$i"   ""
\converter lilypond   png        "lilypond -dbackend=eps -dsafe --png $$i"  ""''')
                addToRC(r'\converter lilypond   pdf6       "lilypond -dbackend=eps -dsafe --pdf $$i"    ""')
                logger.info('+  found LilyPond version %s.' % version_number)
            elif int(version[0]) > 2 or (len(version) > 1 and int(version[0]) == 2 and int(version[1]) >= 6):
                addToRC(r'''\converter lilypond   eps        "lilypond -b eps --ps --safe $$i"  ""
\converter lilypond   png        "lilypond -b eps --png $$i"    ""''')
                if int(version[0]) > 2 or (len(version) > 1 and int(version[0]) == 2 and int(version[1]) >= 9):
                    addToRC(r'\converter lilypond   pdf6       "lilypond -b eps --pdf --safe $$i"   ""')
                logger.info('+  found LilyPond version %s.' % version_number)
            else:
                logger.info('+  found LilyPond, but version %s is too old.' % version_number)
        else:
            logger.info('+  found LilyPond, but could not extract version number.')
    #
    path, lilypond_book = checkProg('a LilyPond book (LaTeX) -> LaTeX converter', ['lilypond-book'])
    if (lilypond_book != ''):
        version_string = cmdOutput("lilypond-book --version")
        match = re.match('^(\S+)$', version_string)
        if match:
            version_number = match.groups()[0]
            version = version_number.split('.')
            if int(version[0]) > 2 or (len(version) > 1 and int(version[0]) == 2 and int(version[1]) >= 13):
                # Note: The --lily-output-dir flag is required because lilypond-book
                #       does not process input again unless the input has changed,
                #       even if the output format being requested is different. So
                #       once a .eps file exists, lilypond-book won't create a .pdf
                #       even when requested with --pdf. This is a problem if a user
                #       clicks View PDF after having done a View DVI. To circumvent
                #       this, use different output folders for eps and pdf outputs.
                addToRC(r'\converter lilypond-book latex    "lilypond-book --safe --lily-output-dir=ly-eps $$i"                                ""')
                addToRC(r'\converter lilypond-book pdflatex "lilypond-book --safe --pdf --latex-program=pdflatex --lily-output-dir=ly-pdf $$i" ""')
                addToRC(r'\converter lilypond-book xetex    "lilypond-book --safe --pdf --latex-program=xelatex --lily-output-dir=ly-pdf $$i"  ""')
                addToRC(r'\converter lilypond-book luatex   "lilypond-book --safe --pdf --latex-program=lualatex --lily-output-dir=ly-pdf $$i" ""')
                logger.info('+  found LilyPond-book version %s.' % version_number)
            else:
                logger.info('+  found LilyPond-book, but version %s is too old.' % version_number)
        else:
            logger.info('+  found LilyPond-book, but could not extract version number.')
    #
    checkProg('a Noteedit -> LilyPond converter', ['noteedit --export-lilypond $$i'],
        rc_entry = [ r'\converter noteedit   lilypond   "%%"    ""' ])
    #
    # Currently, lyxpak outputs a gzip compressed tar archive on *nix
    # and a zip archive on Windows.
    # So, we configure the appropriate version according to the platform.
    cmd = r'\converter lyx %s "python -tt $$s/scripts/lyxpak.py $$r/$$f" ""'
    if os.name == 'nt':
        addToRC(r'\Format lyxzip     zip    "LyX Archive (zip)"     "" "" ""  "document,menu=export"    ""')
        addToRC(cmd % "lyxzip")
    else:
        addToRC(r'\Format lyxgz      gz     "LyX Archive (tar.gz)"  "" "" ""  "document,menu=export"    ""')
        addToRC(cmd % "lyxgz")

    #
    # FIXME: no rc_entry? comment it out
    # checkProg('Image converter', ['convert $$i $$o'])
    #
    # Entries that do not need checkProg
    addToRC(r'''
\converter csv        lyx        "python -tt $$s/scripts/csv2lyx.py $$i $$o"    ""
\converter date       dateout    "python -tt $$s/scripts/date.py %d-%m-%Y > $$o"    ""
\converter docbook    docbook-xml "cp $$i $$o"  "xml"
\converter fen        asciichess "python -tt $$s/scripts/fen2ascii.py $$i $$o"  ""
\converter lyx        lyx13x     "python -tt $$s/lyx2lyx/lyx2lyx -t 221 $$i > $$o"  ""
\converter lyx        lyx14x     "python -tt $$s/lyx2lyx/lyx2lyx -t 245 $$i > $$o"  ""
\converter lyx        lyx15x     "python -tt $$s/lyx2lyx/lyx2lyx -t 276 $$i > $$o"  ""
\converter lyx        lyx16x     "python -tt $$s/lyx2lyx/lyx2lyx -t 345 $$i > $$o"  ""
\converter lyx        lyx20x     "python -tt $$s/lyx2lyx/lyx2lyx -t 413 $$i > $$o"  ""
\converter lyx        lyx21x     "python -tt $$s/lyx2lyx/lyx2lyx -t 474 $$i > $$o"  ""
\converter lyx        clyx       "python -tt $$s/lyx2lyx/lyx2lyx -c big5 -t 245 $$i > $$o"  ""
\converter lyx        jlyx       "python -tt $$s/lyx2lyx/lyx2lyx -c euc_jp -t 245 $$i > $$o"    ""
\converter lyx        klyx       "python -tt $$s/lyx2lyx/lyx2lyx -c euc_kr -t 245 $$i > $$o"    ""
\converter clyx       lyx        "python -tt $$s/lyx2lyx/lyx2lyx -c big5 $$i > $$o" ""
\converter jlyx       lyx        "python -tt $$s/lyx2lyx/lyx2lyx -c euc_jp $$i > $$o"   ""
\converter klyx       lyx        "python -tt $$s/lyx2lyx/lyx2lyx -c euc_kr $$i > $$o"   ""
\converter lyxpreview png        "python -tt $$s/scripts/lyxpreview2bitmap.py --png"    ""
\converter lyxpreview ppm        "python -tt $$s/scripts/lyxpreview2bitmap.py --ppm"    ""
''')


def checkDocBook():
    ''' Check docbook '''
    path, DOCBOOK = checkProg('SGML-tools 2.x (DocBook), db2x scripts or xsltproc', ['sgmltools', 'db2dvi', 'xsltproc'],
        rc_entry = [
            r'''\converter docbook    dvi        "sgmltools -b dvi $$i" ""
\converter docbook    html       "sgmltools -b html $$i"    ""
\converter docbook    ps         "sgmltools -b ps $$i"  ""''',
            r'''\converter docbook    dvi        "db2dvi $$i"   ""
\converter docbook    html       "db2html $$i"  ""''',
            r'''\converter docbook    dvi        "" ""
\converter docbook    html       "" ""''',
            r'''\converter docbook    dvi        "" ""
\converter docbook    html       "" ""'''])
    #
    if DOCBOOK != '':
        return ('yes', 'true', '\\def\\hasdocbook{yes}')
    else:
        return ('no', 'false', '')


def checkOtherEntries():
    ''' entries other than Format and Converter '''
    checkProg('ChkTeX', ['chktex -n1 -n3 -n6 -n9 -n22 -n25 -n30 -n38'],
        rc_entry = [ r'\chktex_command "%%"' ])
    checkProgAlternatives('BibTeX or alternative programs', ['bibtex', 'bibtex8', 'biber'],
        rc_entry = [ r'\bibtex_command "%%"' ],
        alt_rc_entry = [ r'\bibtex_alternatives "%%"' ])
    checkProg('a specific Japanese BibTeX variant', ['pbibtex', 'jbibtex', 'bibtex'],
        rc_entry = [ r'\jbibtex_command "%%"' ])
    checkProgAlternatives('available index processors', ['texindy', 'makeindex -c -q', 'xindy'],
        rc_entry = [ r'\index_command "%%"' ],
        alt_rc_entry = [ r'\index_alternatives "%%"' ])
    checkProg('an index processor appropriate to Japanese', ['mendex -c -q', 'jmakeindex -c -q', 'makeindex -c -q'],
        rc_entry = [ r'\jindex_command "%%"' ])
    checkProg('the splitindex processor', ['splitindex.pl', 'splitindex',
        'splitindex.class'], rc_entry = [ r'\splitindex_command "%%"' ])
    checkProg('a nomenclature processor', ['makeindex'],
        rc_entry = [ r'\nomencl_command "makeindex -s nomencl.ist"' ])
    ## FIXME: OCTAVE is not used anywhere
    # path, OCTAVE = checkProg('Octave', ['octave'])
    ## FIXME: MAPLE is not used anywhere
    # path, MAPLE = checkProg('Maple', ['maple'])
    checkProg('a spool command', ['lp', 'lpr'],
        rc_entry = [
            r'''\print_spool_printerprefix "-d "
\print_spool_command "lp"''',
            r'''\print_spool_printerprefix "-P",
\print_spool_command "lpr"''',
            ''])
    # Add the rest of the entries (no checkProg is required)
    addToRC(r'''\copier    fig        "python -tt $$s/scripts/fig_copy.py $$i $$o"
\copier    pstex      "python -tt $$s/scripts/tex_copy.py $$i $$o $$l"
\copier    pdftex     "python -tt $$s/scripts/tex_copy.py $$i $$o $$l"
\copier    program    "python -tt $$s/scripts/ext_copy.py $$i $$o"
''')


def processLayoutFile(file, bool_docbook):
    ''' process layout file and get a line of result

        Declare lines look like this:

        \DeclareLaTeXClass[<requirements>]{<description>}
        
        Optionally, a \DeclareCategory line follows:
        
        \DeclareCategory{<category>}
        
        So for example (article.layout, scrbook.layout, svjog.layout)
        
        \DeclareLaTeXClass{article}
        \DeclareCategory{Articles}
        
        \DeclareLaTeXClass[scrbook]{book (koma-script)}
        \DeclareCategory{Books}
        
        \DeclareLaTeXClass[svjour,svjog.clo]{article (Springer - svjour/jog)}

        we'd expect this output:

        "article" "article" "article" "false" "article.cls" "Articles"
        "scrbook" "scrbook" "book (koma-script)" "false" "scrbook.cls" "Books"
        "svjog" "svjour" "article (Springer - svjour/jog)" "false" "svjour.cls,svjog.clo" ""
    '''
    def checkForClassExtension(x):
        '''if the extension for a latex class is not
           provided, add .cls to the classname'''
        if not '.' in x:
            return x.strip() + '.cls'
        else:
            return x.strip()
    classname = file.split(os.sep)[-1].split('.')[0]
    # return ('LaTeX', '[a,b]', 'a', ',b,c', 'article') for \DeclareLaTeXClass[a,b,c]{article}
    p = re.compile(r'\Declare(LaTeX|DocBook)Class\s*(\[([^,]*)(,.*)*\])*\s*{(.*)}')
    q = re.compile(r'\DeclareCategory{(.*)}')
    classdeclaration = ""
    categorydeclaration = '""'
    for line in open(file).readlines():
        res = p.search(line)
        qres = q.search(line)
        if res != None:
            (classtype, optAll, opt, opt1, desc) = res.groups()
            avai = {'LaTeX':'false', 'DocBook':bool_docbook}[classtype]
            if opt == None:
                opt = classname
                prereq_latex = checkForClassExtension(classname)
            else:
                prereq_list = optAll[1:-1].split(',')
                prereq_list = map(checkForClassExtension, prereq_list)
                prereq_latex = ','.join(prereq_list)
            prereq_docbook = {'true':'', 'false':'docbook'}[bool_docbook]
            prereq = {'LaTeX':prereq_latex, 'DocBook':prereq_docbook}[classtype]
            classdeclaration = '"%s" "%s" "%s" "%s" "%s"' % (classname, opt, desc, avai, prereq)
            if categorydeclaration != '""':
                return classdeclaration + " " + categorydeclaration
        if qres != None:
             categorydeclaration = '"%s"' % (qres.groups()[0])
             if classdeclaration != "":
                 return classdeclaration + " " + categorydeclaration
    if classdeclaration != "":
        return classdeclaration + " " + categorydeclaration
    logger.warning("Layout file " + file + " has no \DeclareXXClass line. ")
    return ""


def checkLatexConfig(check_config, bool_docbook):
    ''' Explore the LaTeX configuration
        Return None (will be passed to sys.exit()) for success.
    '''
    msg = 'checking LaTeX configuration... '
    # if --without-latex-config is forced, or if there is no previous
    # version of textclass.lst, re-generate a default file.
    if not os.path.isfile('textclass.lst') or not check_config:
        # remove the files only if we want to regenerate
        removeFiles(['textclass.lst', 'packages.lst'])
        #
        # Then, generate a default textclass.lst. In case configure.py
        # fails, we still have something to start lyx.
        logger.info(msg + ' default values')
        logger.info('+checking list of textclasses... ')
        tx = open('textclass.lst', 'w')
        tx.write('''
# This file declares layouts and their associated definition files
# (include dir. relative to the place where this file is).
# It contains only default values, since chkconfig.ltx could not be run
# for some reason. Run ./configure.py if you need to update it after a
# configuration change.
''')
        # build the list of available layout files and convert it to commands
        # for chkconfig.ltx
        foundClasses = []
        for file in glob.glob( os.path.join('layouts', '*.layout') ) + \
            glob.glob( os.path.join(srcdir, 'layouts', '*.layout' ) ) :
            # valid file?
            if not os.path.isfile(file):
                continue
            # get stuff between /xxxx.layout .
            classname = file.split(os.sep)[-1].split('.')[0]
            #  tr ' -' '__'`
            cleanclass = classname.replace(' ', '_')
            cleanclass = cleanclass.replace('-', '_')
            # make sure the same class is not considered twice
            if foundClasses.count(cleanclass) == 0: # not found before
                foundClasses.append(cleanclass)
                retval = processLayoutFile(file, bool_docbook)
                if retval != "":
                    tx.write(retval)
        tx.close()
        logger.info('\tdone')
    if not os.path.isfile('packages.lst') or not check_config:
        logger.info('+generating default list of packages... ')
        removeFiles(['packages.lst'])
        tx = open('packages.lst', 'w')
        tx.close()
        logger.info('\tdone')
    if not check_config:
        return None
    # the following will generate textclass.lst.tmp, and packages.lst.tmp
    logger.info(msg + '\tauto')
    removeFiles(['chkconfig.classes', 'chkconfig.vars', 'chklayouts.tex',
        'wrap_chkconfig.ltx'])
    rmcopy = False
    if not os.path.isfile( 'chkconfig.ltx' ):
        shutil.copyfile( os.path.join(srcdir, 'chkconfig.ltx'), 'chkconfig.ltx' )
        rmcopy = True
    writeToFile('wrap_chkconfig.ltx', '%s\n\\input{chkconfig.ltx}\n' % docbook_cmd)
    # Construct the list of classes to test for.
    # build the list of available layout files and convert it to commands
    # for chkconfig.ltx
    declare = re.compile(r'\Declare(LaTeX|DocBook)Class\s*(\[([^,]*)(,.*)*\])*\s*{(.*)}')
    category = re.compile(r'\DeclareCategory{(.*)}')
    empty = re.compile(r'^\s*$')
    testclasses = list()
    for file in glob.glob( os.path.join('layouts', '*.layout') ) + \
        glob.glob( os.path.join(srcdir, 'layouts', '*.layout' ) ) :
        nodeclaration = False
        if not os.path.isfile(file):
            continue
        classname = file.split(os.sep)[-1].split('.')[0]
        decline = ""
        catline = ""
        for line in open(file).readlines():
            if not empty.match(line) and line[0] != '#':
                if decline == "":
                    logger.warning("Failed to find valid \Declare line for layout file `" + file + "'.\n\t=> Skipping this file!")
                    nodeclaration = True
                # A class, but no category declaration. Just break.
                break
            if declare.search(line) != None:
                decline = "\\TestDocClass{%s}{%s}" % (classname, line[1:].strip())
                testclasses.append(decline)
            elif category.search(line) != None:
                catline = "\\DeclareCategory{%s}{%s}" % (classname, category.search(line).groups()[0])
                testclasses.append(catline)
            if catline == "" or decline == "":
                continue
            break
        if nodeclaration:
            continue
    testclasses.sort()
    cl = open('chklayouts.tex', 'w')
    for line in testclasses:
        cl.write(line + '\n')
    cl.close()
    #
    # we have chklayouts.tex, then process it
    latex_out = cmdOutput(LATEX + ' wrap_chkconfig.ltx', True)
    while True:
        line = latex_out.readline()
        if not line:
            break;
        if re.match('^\+', line):
            logger.info(line.strip())
    # if the command succeeds, None will be returned
    ret = latex_out.close()
    #
    # remove the copied file
    if rmcopy:
        removeFiles( [ 'chkconfig.ltx' ] )
    #
    # currently, values in chkconfig are only used to set
    # \font_encoding
    values = {}
    for line in open('chkconfig.vars').readlines():
        key, val = re.sub('-', '_', line).split('=')
        val = val.strip()
        values[key] = val.strip("'")
    # chk_fontenc may not exist
    try:
        addToRC(r'\font_encoding "%s"' % values["chk_fontenc"])
    except:
        pass
    # if configure successed, move textclass.lst.tmp to textclass.lst
    # and packages.lst.tmp to packages.lst
    if os.path.isfile('textclass.lst.tmp') and len(open('textclass.lst.tmp').read()) > 0 \
        and os.path.isfile('packages.lst.tmp') and len(open('packages.lst.tmp').read()) > 0:
        shutil.move('textclass.lst.tmp', 'textclass.lst')
        shutil.move('packages.lst.tmp', 'packages.lst')
    return ret


def checkModulesConfig():
  removeFiles(['lyxmodules.lst', 'chkmodules.tex'])

  logger.info('+checking list of modules... ')
  tx = open('lyxmodules.lst', 'w')
  tx.write('''## This file declares modules and their associated definition files.
## It has been automatically generated by configure
## Use "Options/Reconfigure" if you need to update it after a
## configuration change.
## "ModuleName" "filename" "Description" "Packages" "Requires" "Excludes" "Category"
''')

  # build the list of available modules
  seen = []
  # note that this searches the local directory first, then the
  # system directory. that way, we pick up the user's version first.
  for file in glob.glob( os.path.join('layouts', '*.module') ) + \
      glob.glob( os.path.join(srcdir, 'layouts', '*.module' ) ) :
      # valid file?
      logger.info(file)
      if not os.path.isfile(file):
          continue

      filename = file.split(os.sep)[-1]
      filename = filename[:-7]
      if seen.count(filename):
          continue

      seen.append(filename)
      retval = processModuleFile(file, filename, bool_docbook)
      if retval != "":
          tx.write(retval)
  tx.close()
  logger.info('\tdone')


def processModuleFile(file, filename, bool_docbook):
    ''' process module file and get a line of result

        The top of a module file should look like this:
          #\DeclareLyXModule[LaTeX Packages]{ModuleName}
          #DescriptionBegin
          #...body of description...
          #DescriptionEnd
          #Requires: [list of required modules]
          #Excludes: [list of excluded modules]
          #Category: [category name]
        The last three lines are optional (though do give a category).
        We expect output:
          "ModuleName" "filename" "Description" "Packages" "Requires" "Excludes" "Category"
    '''
    remods = re.compile(r'\DeclareLyXModule\s*(?:\[([^]]*?)\])?{(.*)}')
    rereqs = re.compile(r'#+\s*Requires: (.*)')
    reexcs = re.compile(r'#+\s*Excludes: (.*)')
    recaty = re.compile(r'#+\s*Category: (.*)')
    redbeg = re.compile(r'#+\s*DescriptionBegin\s*$')
    redend = re.compile(r'#+\s*DescriptionEnd\s*$')

    modname = desc = pkgs = req = excl = catgy = ""
    readingDescription = False
    descLines = []

    for line in open(file).readlines():
      if readingDescription:
        res = redend.search(line)
        if res != None:
          readingDescription = False
          desc = " ".join(descLines)
          # Escape quotes.
          desc = desc.replace('"', '\\"')
          continue
        descLines.append(line[1:].strip())
        continue
      res = redbeg.search(line)
      if res != None:
        readingDescription = True
        continue
      res = remods.search(line)
      if res != None:
          (pkgs, modname) = res.groups()
          if pkgs == None:
            pkgs = ""
          else:
            tmp = [s.strip() for s in pkgs.split(",")]
            pkgs = ",".join(tmp)
          continue
      res = rereqs.search(line)
      if res != None:
        req = res.group(1)
        tmp = [s.strip() for s in req.split("|")]
        req = "|".join(tmp)
        continue
      res = reexcs.search(line)
      if res != None:
        excl = res.group(1)
        tmp = [s.strip() for s in excl.split("|")]
        excl = "|".join(tmp)
        continue
      res = recaty.search(line)
      if res != None:
        catgy = res.group(1)
        continue

    if modname == "":
      logger.warning("Module file without \DeclareLyXModule line. ")
      return ""

    if pkgs != "":
        # this module has some latex dependencies:
        # append the dependencies to chkmodules.tex,
        # which is \input'ed by chkconfig.ltx
        testpackages = list()
        for pkg in pkgs.split(","):
            if "->" in pkg:
                # this is a converter dependency: skip
                continue
            if pkg.endswith(".sty"):
                pkg = pkg[:-4]
            testpackages.append("\\TestPackage{%s}" % (pkg,))
        cm = open('chkmodules.tex', 'a')
        for line in testpackages:
            cm.write(line + '\n')
        cm.close()

    return '"%s" "%s" "%s" "%s" "%s" "%s" "%s"\n' % (modname, filename, desc, pkgs, req, excl, catgy)


def checkTeXAllowSpaces():
    ''' Let's check whether spaces are allowed in TeX file names '''
    tex_allows_spaces = 'false'
    if lyx_check_config:
        msg = "Checking whether TeX allows spaces in file names... "
        writeToFile('a b.tex', r'\message{working^^J}' )
        if LATEX != '':
            if os.name == 'nt' or sys.platform == 'cygwin':
                latex_out = cmdOutput(LATEX + r""" "\nonstopmode\input{\"a b\"}\makeatletter\@@end" """)
            else:
                latex_out = cmdOutput(LATEX + r""" '\nonstopmode\input{"a b"}\makeatletter\@@end' """)
        else:
            latex_out = ''
        if 'working' in latex_out:
            logger.info(msg + 'yes')
            tex_allows_spaces = 'true'
        else:
            logger.info(msg + 'no')
            tex_allows_spaces = 'false'
        addToRC(r'\tex_allows_spaces ' + tex_allows_spaces)
        removeFiles( [ 'a b.tex', 'a b.log', 'texput.log' ])


def rescanTeXFiles():
    ''' Run kpsewhich to update information about TeX files '''
    logger.info("+Indexing TeX files... ")
    if not os.path.isfile( os.path.join(srcdir, 'scripts', 'TeXFiles.py') ):
        logger.error("configure: error: cannot find TeXFiles.py script")
        sys.exit(1)
    tfp = cmdOutput("python -tt " + os.path.join(srcdir, 'scripts', 'TeXFiles.py'))
    logger.info(tfp)
    logger.info("\tdone")


def removeTempFiles():
    # Final clean-up
    if not lyx_keep_temps:
        removeFiles(['chkconfig.vars', 'chklatex.ltx', 'chklatex.log',
            'chklayouts.tex', 'chkmodules.tex', 'missfont.log',
            'wrap_chkconfig.ltx', 'wrap_chkconfig.log'])


if __name__ == '__main__':
    lyx_check_config = True
    lyx_kpsewhich = True
    outfile = 'lyxrc.defaults'
    lyxrc_fileformat = 17
    rc_entries = ''
    lyx_keep_temps = False
    version_suffix = ''
    lyx_binary_dir = ''
    ## Parse the command line
    for op in sys.argv[1:]:   # default shell/for list is $*, the options
        if op in [ '-help', '--help', '-h' ]:
            print '''Usage: configure [options]
Options:
    --help                   show this help lines
    --keep-temps             keep temporary files (for debug. purposes)
    --without-kpsewhich      do not update TeX files information via kpsewhich
    --without-latex-config   do not run LaTeX to determine configuration
    --with-version-suffix=suffix suffix of binary installed files
    --binary-dir=directory   directory of binary installed files
'''
            sys.exit(0)
        elif op == '--without-kpsewhich':
            lyx_kpsewhich = False
        elif op == '--without-latex-config':
            lyx_check_config = False
        elif op == '--keep-temps':
            lyx_keep_temps = True
        elif op[0:22] == '--with-version-suffix=':  # never mind if op is not long enough
            version_suffix = op[22:]
        elif op[0:13] == '--binary-dir=':
            lyx_binary_dir = op[13:]
        else:
            print "Unknown option", op
            sys.exit(1)
    #
    # check if we run from the right directory
    srcdir = os.path.dirname(sys.argv[0])
    if srcdir == '':
        srcdir = '.'
    if not os.path.isfile( os.path.join(srcdir, 'chkconfig.ltx') ):
        logger.error("configure: error: cannot find chkconfig.ltx script")
        sys.exit(1)
    setEnviron()
    if sys.platform == 'darwin' and len(version_suffix) > 0:
        checkUpgrade()
    createDirectories()
    dtl_tools = checkDTLtools()
    ## Write the first part of outfile
    writeToFile(outfile, '''# This file has been automatically generated by LyX' lib/configure.py
# script. It contains default settings that have been determined by
# examining your system. PLEASE DO NOT MODIFY ANYTHING HERE! If you
# want to customize LyX, use LyX' Preferences dialog or modify directly
# the "preferences" file instead. Any setting in that file will
# override the values given here.

Format %i

''' % lyxrc_fileformat)
    # check latex
    LATEX = checkLatex(dtl_tools)
    # check java and perl before any checkProg that may require them
    java = checkProg('a java interpreter', ['java'])[1]
    perl = checkProg('a perl interpreter', ['perl'])[1]
    checkFormatEntries(dtl_tools)
    checkConverterEntries()
    (chk_docbook, bool_docbook, docbook_cmd) = checkDocBook()
    checkTeXAllowSpaces()
    windows_style_tex_paths = checkTeXPaths()
    if windows_style_tex_paths != '':
        addToRC(r'\tex_expects_windows_paths %s' % windows_style_tex_paths)
    checkOtherEntries()
    if lyx_kpsewhich:
        rescanTeXFiles()
    checkModulesConfig()
    # --without-latex-config can disable lyx_check_config
    ret = checkLatexConfig(lyx_check_config and LATEX != '', bool_docbook)
    removeTempFiles()
    # The return error code can be 256. Because most systems expect an error code
    # in the range 0-127, 256 can be interpretted as 'success'. Because we expect
    # a None for success, 'ret is not None' is used to exit.
    sys.exit(ret is not None)
