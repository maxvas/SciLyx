# -*- text -*-

# file stdmenus.inc
# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.

# author Jean-Marc Lasgouttes
# author John Levon
# author Michael Gerz

# Full author contact details are available in file CREDITS.

# The interface is designed (partially) following the KDE Human Interface
# Guidelines (http://techbase.kde.org/Projects/Usability/HIG)

# Casing Rules:
# Capitalize all words in the element, with the following exceptions: 
# * Articles: a, an, the. 
# * Conjunctions: and, but, for, not, so, yet ...  
# * Prepositions of three or fewer letters: at, for, by, in, to ...
#   (except when the preposition is part of a verb phrase, such as "Check In")
# (http://library.gnome.org/devel/hig-book/stable/design-text-labels.html.en)

Format 2

Menuset

	Menubar
# Make the first 4 menus be as much as expected as possible
		Submenu "File|F" "file"
		Submenu "Edit|E" "edit"
		Submenu "View|V" "view"
		Submenu "Insert|I" "insert"
		Submenu "Navigate|N" "navigate"
		Submenu "Document|D" "document"
		Submenu "Tools|T" "tools"
		Submenu "Help|H" "help"
	End

#
# FILE MENU
#

	Menu "file"
		Item "New|N" "buffer-new"
		Item "New from Template...|m" "buffer-new-template"
		Item "Open...|O" "file-open"
		Submenu "Open Recent|t" "file_lastfiles"
		Separator
		Item "Close|C" "buffer-close"
		Item "Close All" "buffer-close-all"
		Item "Save|S" "buffer-write"
		Item "Save As...|A" "buffer-write-as"
		Item "Save All|l" "buffer-write-all"
		Item "Revert to Saved|R" "buffer-reload"
		Submenu "Version Control|V" "file_vc"
		Separator
		Submenu "Import|I" "file_import"
		Submenu "Export|E" "file_export"
		Item "Print...|P" "dialog-show print"
		OptItem "Fax...|F" "buffer-export fax"
		Separator
		Item "New Window|W" "window-new"
		Item "Close Window|d" "window-close"
		Separator
		Item "Exit|x" "lyx-quit"
	End

	Menu "file_lastfiles"
		Lastfiles
	End

	Menu "file_vc"
		OptItem "Register...|R" "vc-register"
		OptItem "Check In Changes...|I" "vc-check-in"
		OptItem "Check Out for Edit|O" "vc-check-out"
		OptItem "Copy|p" "vc-copy"
		OptItem "Rename|R" "vc-rename"
		OptItem "Update Local Directory From Repository|d" "vc-repo-update"
		OptItem "Revert to Repository Version|v" "vc-revert"
		OptItem "Undo Last Check In|U" "vc-undo-last"
		OptItem "Compare with Older Revision...|C" "vc-compare"
		OptItem "Show History...|H" "dialog-show vclog"
		OptItem "Use Locking Property|L" "vc-locking-toggle"
	End

	Menu "file_import"
		ImportFormats
	End

	Menu "file_export"
		Item "Export As...|s" "buffer-export-as"
		ExportFormats
		Item "More Formats & Options...|O" "buffer-export custom"
	End

#
# EDIT MENU
#

	Menu "edit"
		Item "Undo|U" "undo"
		Item "Redo|R" "redo"
		Separator
		Item "Cut" "cut"
		Item "Copy" "copy"
		Item "Paste" "paste"
		Submenu "Paste Recent|e" "edit_pasterecent"
		Submenu "Paste Special" "edit_paste"
		Separator
		Item "Select Whole Inset" "inset-select-all"
		Item "Select All" "command-sequence buffer-begin ; buffer-end-select"
		Separator
		Item "Find & Replace (Quick)...|F" "dialog-show findreplace"
		Item "Find & Replace (Advanced)..." "dialog-show findreplaceadv"
		Separator
		Item "Move Paragraph Up|o" "paragraph-move-up"
		Item "Move Paragraph Down|v" "paragraph-move-down"
		Separator
		Submenu "Text Style|S" "edit_textstyles"
		LanguageSelector
		Item "Paragraph Settings...|P" "layout-paragraph"
		Separator
# Mathed b0rkage means these don't work properly
		OptSubmenu "Table|T" "edit_tabular"
		OptSubmenu "Rows & Columns|C" "edit_tabular_features"
		Separator
# This is where the context-specific stuff is supposed to go.
# The idea is that everything on here can disappear and should
# only appear when relevant (getStatus). Only stuff that needs
# a right-click to bring up the dialog should appear here (it is not
# obvious what the context is for the others)
		OptItem "Increase List Depth|I" "depth-increment"
		OptItem "Decrease List Depth|D" "depth-decrement"
		EnvironmentSeparators
		OptItem "Dissolve Inset" "inset-dissolve"
		OptItem "TeX Code Settings...|C" "inset-settings ert"
# 'a' shortcut to match Insert entry, shouldn't clash with Table Settings
		OptItem "Float Settings...|a" "inset-settings float"
		OptItem "Text Wrap Settings...|W" "inset-settings wrap"
		OptItem "Note Settings...|N" "inset-settings note"
		OptItem "Phantom Settings...|h" "inset-settings phantom"
		OptItem "Branch Settings...|B" "inset-settings branch"
		OptItem "Box Settings...|x" "inset-settings box"
		OptItem "Index Entry Settings...|y" "inset-settings index"
		OptItem "Index Settings...|x" "inset-settings index_print"
		OptItem "Info Settings...|n" "inset-settings info"
		OptItem "Listings Settings...|g" "inset-settings listings"
# Hey, guess what's broken ? Surprise surprise, it's tabular stuff
# This is in the Table submenu instead for now.
#	OptItem "Table Settings...|a" "inset-settings tabular"
		OptItem "Table Settings...|a" "layout-tabular"
	End

	Menu "edit_paste"
		OptItem "Paste from HTML|H" "paste html"
		OptItem "Paste from LaTeX|L" "paste latex"
		OptItem "Paste as LinkBack PDF" "paste linkback"
		OptItem "Paste as PDF" "paste pdf"
		OptItem "Paste as PNG" "paste png"
		OptItem "Paste as JPEG" "paste jpeg"
		OptItem "Paste as EMF" "paste emf"
		Separator
		Item "Plain Text|T" "clipboard-paste"
		Item "Plain Text, Join Lines|J" "clipboard-paste paragraph"
		Separator
		Item "Selection|S" "primary-selection-paste"
		Item "Selection, Join Lines|i" "primary-selection-paste paragraph"
# clipboard-paste-simple is commented because it is irritating (bug 8749)
# until somebody can explain why it is needed in addition to clipboard-paste
#		Separator
#		Item "Unformatted Text|U" "clipboard-paste-simple"
#		Item "Unformatted, Join Lines|o" "clipboard-paste-simple paragraph"
	End

	Menu "edit_pasterecent"
		PasteRecent
	End

	Menu "edit_textstyles"
		Item "Dissolve Text Style" "inset-dissolve charstyle"
		CharStyles
		Elements
		Separator
		Item "Customized...|C" "dialog-show character"
		Separator
		Item "Capitalize|a" "word-capitalize"
		Item "Uppercase|U" "word-upcase"
		Item "Lowercase|L" "word-lowcase"
	End

# not much we can do to help here
	Menu "edit_tabular"
		Item "Multicolumn|M" "inset-modify tabular multicolumn"
		Item "Multirow|u" "inset-modify tabular multirow"
		Separator
		Item "Top Line|T" "inset-modify tabular toggle-line-top"
		Item "Bottom Line|B" "inset-modify tabular toggle-line-bottom"
		Item "Left Line|L" "inset-modify tabular toggle-line-left"
		Item "Right Line|R" "inset-modify tabular toggle-line-right"
		Separator
		Item "Top|p" "inset-modify tabular tabular-valign-top"
		Item "Middle|i" "inset-modify tabular tabular-valign-middle"
		Item "Bottom|o" "inset-modify tabular tabular-valign-bottom"
	End

	Menu "edit_tabular_features"
		Item "Left|L" "inset-modify tabular align-left"
		Item "Center|C" "inset-modify tabular align-center"
		Item "Right|R" "inset-modify tabular align-right"
		Separator
		Item "Top|T" "inset-modify tabular valign-top"
		Item "Middle|M" "inset-modify tabular valign-middle"
		Item "Bottom|B" "inset-modify tabular valign-bottom"
		Separator
		Item "Add Row|A" "inset-modify tabular append-row"
		Item "Delete Row|D" "inset-modify tabular delete-row"
		Item "Copy Row|o" "inset-modify tabular copy-row"
		Item "Move Row Up" "inset-modify tabular move-row-up"
		Item "Move Row Down" "inset-modify tabular move-row-down"
		# Item "Swap Rows|S" "inset-modify tabular swap-row" # currently broken
		Separator
		Item "Add Column|u" "inset-modify tabular append-column"
		Item "Delete Column|e" "inset-modify tabular delete-column"
		Item "Copy Column|p" "inset-modify tabular copy-column"
		Item "Move Column Right|v" "inset-modify tabular move-column-right"
		Item "Move Column Left" "inset-modify tabular move-column-left"
		# Item "Swap Columns|w" "inset-modify tabular swap-column" # currently broken
	End

#
# VIEW MENU
#

	Menu "view"
		Item "Open All Insets|O" "inset-forall * inset-toggle open"
		Item "Close All Insets|C" "inset-forall * inset-toggle close"
		Separator
		Item "Outline Pane|u" "dialog-toggle toc"
		Item "Messages Pane|g" "dialog-toggle progress"
		Submenu "Toolbars|b" "toolbars"
		Separator
		Item "Split View Into Left and Right Half|i" "view-split horizontal"
		Item "Split View Into Upper and Lower Half|e" "view-split vertical"
		OptItem "Close Current View|w" "tab-group-close"
		Item "Fullscreen|l" "ui-toggle fullscreen"	
		Separator
		Documents
	End

	Menu "toolbars"
		Toolbars
	End
#
# INSERT MENU
#
	Menu "insert"
# Try and group things a little better, and percolate the most
# commonly used to good positions (exception being mathed, but
# most people using mathed learn key shortcuts quite quickly anyway)
		Submenu "Special Character|p" "insert_special"
		Submenu "Formatting|o" "insert_formatting"
		Submenu "List / TOC|i" "insert_toc"
		Submenu "Float|a" "insert_float"
		Submenu "Note|N" "insert_note"
		Submenu "Branch|B" "insert_branches"
		Submenu "Custom Insets" "insert_custom"
		Submenu "File|e" "insert_file"
		Submenu "Box[[Menu]]|x" "insert_box"
		OptSubmenu "Insert Regular Expression" "context-edit-regexp"
		Separator
		Item "Citation...|C" "dialog-show-new-inset citation"
		Item "Cross-Reference...|R" "dialog-show-new-inset ref"
		Item "Label...|L" "label-insert"
		Captions
		Indices
		Item "Nomenclature Entry...|y" "nomencl-insert"
		Separator
		Item "Table...|T" "tabular-insert"
		Item "Таблица из источника...|и" "tabular-data-insert"
		Item "Рисунок...|Р" "dialog-show-new-inset graphics"
		Item "График...|ф" "dialog-show-new-inset chart"
		Item "URL|U" "flex-insert URL"
		Item "Hyperlink...|k" "href-insert"
		Item "Footnote|F" "footnote-insert"
		Item "Marginal Note|M" "marginalnote-insert"
		Arguments
		Item "TeX Code" "ert-insert"
		Item "Program Listing[[Menu]]" "listing-insert"
		Item "Date" "date-insert"
		Item "Preview|w" "preview-insert"
	End

	Menu "insert_special"
		Item "Symbols...|b" "dialog-show symbols"
		Item "Ellipsis|i" "specialchar-insert dots"
		Item "End of Sentence|E" "specialchar-insert end-of-sentence"
		Item "Ordinary Quote|Q" "self-insert \""
		Item "Single Quote|S" "quote-insert single"
		Item "Protected Hyphen|y" "specialchar-insert nobreakdash"
		Item "Breakable Slash|a" "specialchar-insert slash"
		Item "Visible Space|V" "space-insert visible"
		Item "Menu Separator|M" "specialchar-insert menu-separator"
		Item "Phonetic Symbols|P" "ipa-insert"
	End

	Menu "insert_formatting"
		Item "Superscript|S" "script-insert superscript"
		Item "Subscript|u" "script-insert subscript"
		Separator
		Item "Protected Space|P" "space-insert protected"
		Item "Interword Space|w" "space-insert normal"
		Item "Thin Space|T" "space-insert thin"
		Item "Horizontal Space...|o" "command-alternatives dialog-show-new-inset space;dialog-show-new-inset mathspace"
		Item "Horizontal Line...|L" "dialog-show-new-inset line"
		Item "Vertical Space...|V" "dialog-show-new-inset vspace"
		Submenu "Phantom|m" "insert_phantom"
		Separator
		Item "Hyphenation Point|H" "specialchar-insert hyphenation"
		Item "Ligature Break|k" "specialchar-insert ligature-break"
		Item "Ragged Line Break|R" "newline-insert newline"
		Item "Justified Line Break|J" "newline-insert linebreak"
		Separator
		Item "New Page|N" "newpage-insert newpage"
		Item "Page Break|a" "newpage-insert pagebreak"
		Item "Clear Page|C" "newpage-insert clearpage"
		Item "Clear Double Page|D" "newpage-insert cleardoublepage"
	End

	Menu "insert_float"
		FloatInsert
# This has got to be better than "Wrap", anyway
		Item "Figure Wrap Float|F" "wrap-insert figure"
		Item "Table Wrap Float|T" "wrap-insert table"
	End

	Menu "insert_toc"
		Item "Table of Contents|C" "inset-insert toc"
		FloatListInsert
		IndicesLists
		Item "List of Listings|L" "inset-insert toc CommandInset toc LatexCommand lstlistoflistings \end_inset"
		Item "Nomenclature|N" "nomencl-print"
		Item "BibTeX Bibliography...|B" "dialog-show-new-inset bibtex"
	End

	Menu "insert_file"
		Item "LyX Document...|X" "file-insert"
		Item "Plain Text...|T" "file-insert-plaintext"
		Item "Plain Text, Join Lines...|J" "file-insert-plaintext-para"
		Separator
		Item "External Material...|M" "dialog-show-new-inset external"
		Item "Child Document...|d" "dialog-show-new-inset include"
	End

	Menu "insert_box"
		Item "Frameless|l" "box-insert Frameless"
		Item "Simple Frame|F" "box-insert Boxed"
		Item "Oval, Thin|a" "box-insert ovalbox"
		Item "Oval, Thick|v" "box-insert Ovalbox"
		Item "Drop Shadow|w" "box-insert Shadowbox"
		Item "Shaded Background|B" "box-insert Shaded"
		Item "Double Frame|u" "box-insert Doublebox"
	End

	Menu "insert_note"
		Item "LyX Note|N" "note-insert Note"
		Item "Comment|C" "note-insert Comment"
		Item "Greyed Out|G" "note-insert Greyedout"
	End

	Menu "insert_branches"
		Branches
		Separator
		Item "Insert New Branch...|I" "branch-add-insert"
	End
	
	Menu "insert_phantom"
		Item "Phantom|P" "phantom-insert Phantom"
		Item "Horizontal Phantom|H" "phantom-insert HPhantom"
		Item "Vertical Phantom|V" "phantom-insert VPhantom"
	End

	Menu "insert_custom"
		Custom
	End

#
# DOCUMENT MENU
#

	Menu "document"
		Submenu "Change Tracking|C" "document_change"
		OptItem "Build Program|B" "build-program"
		Item "LaTeX Log|L" "dialog-show latexlog"
		Item "Start Appendix Here|A" "appendix"
		Separator
		ViewFormats
		UpdateFormats
		OptItem "View Master Document|M" "master-buffer-view"
		OptItem "Update Master Document|a" "master-buffer-update"
		Separator
		Item "Compressed|m" "buffer-toggle-compression"
		Item "Settings...|S" "dialog-show document"
	End

	Menu "document_change"
		Item "Track Changes|T" "changes-track"
		Item "Merge Changes...|M" "changes-merge"
		Item "Accept Change|A" "change-accept"
		Item "Reject Change|R" "change-reject"
		Item "Accept All Changes|c" "all-changes-accept"
		Item "Reject All Changes|e" "all-changes-reject"
		Item "Show Changes in Output|S" "changes-output"
	End

#
# NAVIGATE MENU
#
	Menu "navigate"
		Submenu "Bookmarks|B" "navigate_bookmarks"
		Item "Next Note|N" "note-next"
		Item "Next Change|C" "change-next"
		Item "Next Cross-Reference|R" "reference-next"
		OptItem "Go to Label|L" "label-goto"
		Item "Forward Search|F" "forward-search"
		Separator
		Toc
	End

	Menu "navigate_bookmarks"
		Item "Save Bookmark 1|S" "bookmark-save 1"
		Item "Save Bookmark 2" "bookmark-save 2"
		Item "Save Bookmark 3" "bookmark-save 3"
		Item "Save Bookmark 4" "bookmark-save 4"
		Item "Save Bookmark 5" "bookmark-save 5"
		Item "Clear Bookmarks|C" "bookmark-clear"
		Separator
		Item "Navigate Back|B" "bookmark-goto 0"
		Bookmarks
	End

#
# TOOLS MENU
#

	Menu "tools"
		Item "Spellchecker...|S" "dialog-show spellchecker"
		OptItem "Thesaurus...|T" "thesaurus-entry"
		Item "Statistics...|a" "statistics"
		OptItem "Check TeX|h" "buffer-chktex"
		Item "TeX Information|I" "dialog-show texinfo"
		Item "Compare...|C" "dialog-show compare"
		Separator
# A LOT of applications have Tools->Prefs. Remember this
# should be rarely used - Edit menu is not a good place to
# have it.
		Item "Reconfigure|R" "reconfigure"
		Item "Preferences...|P" "dialog-show prefs"
	End

#
# HELP MENU
#
	Menu "help"
		Item "Introduction|I" "help-open Intro"
		Item "Tutorial|T" "help-open Tutorial"
		Item "User's Guide|U" "help-open UserGuide"
		Item "Additional Features|F" "help-open Additional"
		Item "Embedded Objects|O" "help-open EmbeddedObjects"
		Item "Customization|C" "help-open Customization"
		Item "Shortcuts|S" "help-open Shortcuts"
		Item "LyX Functions|y" "help-open LFUNs"
		Item "LaTeX Configuration|L" "help-open LaTeXConfig"
		Submenu "Specific Manuals|p" "examples"
		Separator
		Item "About LyX|X" "dialog-show aboutlyx"
	End

	Menu "examples"
		Item "Beamer Presentations|B" "help-open beamer"
		Item "Braille|a" "help-open Braille"
		Item "Feynman-diagram|F" "help-open FeynmanDiagrams"
		Item "Knitr|K" "help-open knitr"
		Item "LilyPond|P" "help-open lilypond"
		Item "Linguistics|L" "help-open linguistics"
		Item "Multilingual Captions|C" "help-open MultilingualCaptions"
		Item "Risk and Safety Statements|R" "help-open R-S-statements"
		Item "Sweave|S" "help-open sweave"
		Item "XY-pic|X" "help-open xypic"
	End


End
