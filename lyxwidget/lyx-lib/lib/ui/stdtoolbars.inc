# -*- text -*-

# file stdtoolbars.inc
# This file is part of LyX, the document processor.
# Licence details can be found in the file COPYING.

# author Jean-Marc Lasgouttes
# author John Levon

# Full author contact details are available in file CREDITS.

# The interface is designed (partially) following the KDE Human Interface
# Guidelines (http://techbase.kde.org/Projects/Usability/HIG)

# Casing rules "The tooltip":
# Capitalize the first letter of the first word, and any other words normally
# capitalized in sentences, such as application names.
#
# Casing Rules for the "GUI Name":
# Capitalize all words in the element, with the following exceptions: 
# * Articles: a, an, the. 
# * Conjunctions: and, but, for, not, so, yet ...  
# * Prepositions of three or fewer letters: at, for, by, in, to ...
#
# (http://library.gnome.org/devel/hig-book/stable/design-text-labels.html.en)

# A Toolbar starts like :
#
# Toolbar "name" "GUI Name"
#
# Only seven commands are allowed inside the Toolbar and End
# directives:
#   Item "The tooltip" "<action> [<parameter>]" adds an icon to the toolbar performing
#          "<action> <parameter>"
#      Examples:
#      Item "Small font" "font-size small"
#      Item Emphasized set-emph
#
#   Layouts adds the layouts combo-box to the toolbar
#
#   Separator adds some spacing to the toolbar
#
#   Minibuffer adds the command buffer (Qt only, only one may exist)
#
#   TableInsert "The tooltip" adds a special widget for quick insertion of tables
#
#   PopupMenu "name" "The tooltip"
#
#   ... inserts a button with a popup menu derived from Toolbar "name"
#
#   IconPalette "name" "The tooltip"
#
#   ... inserts a button with a popup iconpalette derived from Toolbar "name"
#
#
# The icons are found in the lib/images/ direcory under the name
# action.png or action_parameter.png, except for math-insert, which
# is e.g. lib/image/math/sum.png. Note that some characters are
# replaced (e.g. ')' -> rbracket).
#
#  All other lyx commands will get a "unknown" icon.
#
# This is the default toolbar:

Format 2

ToolbarSet

	Toolbar "standard" "Standard"
		Layouts
		Item "New document" "buffer-new"
		Item "Open document" "file-open"
		Item "Save document" "buffer-write"
		Item "Print document" "dialog-show print"
		Separator
		Item "Check spelling" "dialog-show spellchecker"
		Item "Spellcheck continuously" "spelling-continuously"
		Separator
		Item "Undo" "undo"
		Item "Redo" "redo"
		Item "Cut" "cut"
		Item "Copy" "copy"
		Item "Paste" "paste"
		Item "Find and replace" "dialog-show findreplace"
		Item "Find and replace (advanced)" "dialog-toggle findreplaceadv"
		Item "Navigate back" "bookmark-goto 0"
		Separator
		Item "Toggle emphasis" "font-emph"
		Item "Toggle noun" "font-noun"
		Item "Apply last" "textstyle-apply"
		Separator
		Item "Insert graphics" "dialog-show-new-inset graphics"
		TableInsert "Insert table"
		Separator
		Item "Toggle outline" "dialog-toggle toc"
		Item "Toggle table toolbar" "toolbar-toggle table"
	End

	Toolbar "view/update" "View/Update"
		Item "View" "buffer-view"
		Item "Update" "buffer-update"
		Item "View master document" "master-buffer-view"
		Item "Update master document" "master-buffer-update"
		Item "Enable Forward/Reverse Search" "buffer-toggle-output-sync"
		Separator
		StickyPopupMenu "view-others" "View other formats"
		StickyPopupMenu "update-others" "Update other formats"
	End

	Toolbar "extra" "Extra"
		Item "Default" "layout"
		Item "Numbered list" "layout Enumerate"
		Item "Itemized list" "layout Itemize"
		Item "List" "layout List"
		Item "Description" "layout Description"
		Item "Increase depth" "depth-increment"
		Item "Decrease depth" "depth-decrement"
		Separator
		Item "Insert figure float" "float-insert figure"
		Item "Insert table float" "float-insert table"
		Item "Insert label" "label-insert"
		Item "Insert cross-reference" "dialog-show-new-inset ref"
		Item "Insert citation" "dialog-show-new-inset citation"
		Item "Insert index entry" "index-insert"
		Item "Insert nomenclature entry" "nomencl-insert"
		Separator
		Item "Insert footnote" "footnote-insert"
		Item "Insert margin note" "marginalnote-insert"
		Item "Insert LyX note" "note-insert"
		Item "Insert box" "box-insert Frameless"
		Item "Insert hyperlink" "href-insert"
		Item "Insert TeX code" "ert-insert"
		Item "Include file" "dialog-show-new-inset include"
		Separator
		Item "Text style" "dialog-show character"
		Item "Paragraph settings" "layout-paragraph"
		Item "Thesaurus" "thesaurus-entry"
	End
	
	Toolbar "table" "Table"
		Item "Add row" "inset-modify tabular append-row"
		Item "Add column" "inset-modify tabular append-column"
		Item "Delete row" "inset-modify tabular delete-row"
		Item "Delete column" "inset-modify tabular delete-column"
		Item "Move row up" "inset-modify tabular move-row-up"
		Item "Move column left" "inset-modify tabular move-column-left"
		Item "Move row down" "inset-modify tabular move-row-down"
		Item "Move column right" "inset-modify tabular move-column-right"
		Separator
		Item "Set top line" "inset-modify tabular toggle-line-top"
		Item "Set bottom line" "inset-modify tabular toggle-line-bottom"
		Item "Set left line" "inset-modify tabular toggle-line-left"
		Item "Set right line" "inset-modify tabular toggle-line-right"
		Item "Set border lines" "inset-modify tabular set-border-lines"
		Item "Set all lines" "inset-modify tabular set-all-lines"
		Item "Unset all lines" "inset-modify tabular unset-all-lines"
		Separator
		Item "Align left" "command-alternatives inset-modify tabular m-align-left;inset-modify tabular align-left"
		Item "Align center" "command-alternatives inset-modify tabular m-align-center;inset-modify tabular align-center"
		Item "Align right" "command-alternatives inset-modify tabular m-align-right;inset-modify tabular align-right"
		Item "Align on decimal" "inset-modify tabular align-decimal"
		Separator
		Item "Align top" "command-alternatives inset-modify tabular m-valign-top;inset-modify tabular valign-top"
		Item "Align middle" "command-alternatives inset-modify tabular m-valign-middle;inset-modify tabular valign-middle"
		Item "Align bottom" "command-alternatives inset-modify tabular m-valign-bottom;inset-modify tabular valign-bottom"
		Separator
		Item "Rotate cell by 90 degrees or unset rotation" "inset-modify tabular toggle-rotate-cell"
		Item "Rotate table by 90 degrees or unset rotation" "inset-modify tabular toggle-rotate-tabular"
		Item "Set multi-column" "inset-modify tabular multicolumn"
		Item "Set multi-row" "inset-modify tabular multirow"
	End

	Toolbar "ipa" "Phonetic Symbols"
		IconPalette "ipa_pulmonic" "IPA Pulmonic Consonants"
		IconPalette "ipa_nonpulmonic" "IPA Non-Pulmonic Consonants"
		IconPalette "ipa_vowels" "IPA Vowels"
		IconPalette "ipa_others" "IPA Other Symbols"
		IconPalette "ipa_suprasegmentals" "IPA Suprasegmentals"
		IconPalette "ipa_diacritics" "IPA Diacritics"
		IconPalette "ipa_accents" "IPA Tones and Word Accents"
	End

	Toolbar "minibuffer" "Command Buffer"
		Minibuffer
	End
	
	Toolbar "review" "Review[[Toolbar]]"
		Item "Track changes" "changes-track"
		Item "Show changes in output" "changes-output"
		Separator
		Item "Next change" "change-next"
		Item "Accept change inside selection" "change-accept"
		Item "Reject change inside selection" "change-reject"
		Separator
		Item "Merge changes" "changes-merge"
		Item "Accept all changes" "all-changes-accept"
		Item "Reject all changes" "all-changes-reject"
		Separator
		Item "Insert note" "note-insert"
		Item "Next note" "note-next"
	End
	
	Toolbar "view-others" "View Other Formats"
		ViewFormats
	End
	
	Toolbar "update-others" "Update Other Formats"
		UpdateFormats
	End

# The following three toolbars are disabled because of missing icons.
#	Toolbar "import/export" "Import/Export"
#		Item "Import" "buffer-import"
#		Item "Export" "buffer-export"
#		StickyPopupMenu "import-others" "Import other formats"
#		StickyPopupMenu "export-others" "Export other formats"
#	End

#	Toolbar "import-others" "Import Other Formats"
#		ImportFormats
#	End

#	Toolbar "export-others" "Export Other Formats"
#		ExportFormats
#	End

	Toolbar "vcs" "Version Control"
		Item "Register" "vc-register"
		Item "Check-out for edit" "vc-check-out"
		Item "Check-in changes" "vc-check-in"
		Item "View revision log" "dialog-show vclog"
		Item "Revert changes" "vc-revert"
		Item "Compare with older revision" "vc-compare"
		Item "Compare with last revision" "vc-compare 0"
		Item "Insert Version Info" "info-insert buffer vcs-revision"
		Separator
		Item "Use SVN file locking property" "vc-locking-toggle"
		Separator
		Item "Update local directory from repository" "vc-repo-update"
	End

	Toolbar "ipa_pulmonic" "IPA Pulmonic Consonants"
		Item "Voiceless bilabial plosive" "unicode-insert 0x0070"
		Item "Voiced bilabial plosive" "unicode-insert 0x0062"
		Item "Voiceless alveolar plosive" "unicode-insert 0x0074"
		Item "Voiced alveolar plosive" "unicode-insert 0x0064"
		Item "Voiceless retroflex plosive" "unicode-insert 0x0288"
		Item "Voiced retroflex plosive" "unicode-insert 0x0256"
		Item "Voiceless palatal plosive" "unicode-insert 0x0063"
		Item "Voiced palatal plosive" "unicode-insert 0x025f"
		Item "Voiceless velar plosive" "unicode-insert 0x006b"
		Item "Voiced velar plosive" "unicode-insert 0x0261"
		Item "Voiceless uvular plosive" "unicode-insert 0x0071"
		Item "Voiced uvular plosive" "unicode-insert 0x0262"
		Item "Glottal plosive" "unicode-insert 0x0294"
		Item "Voiced bilabial nasal" "unicode-insert 0x006d"
		Item "Voiced labiodental nasal" "unicode-insert 0x0271"
		Item "Voiced alveolar nasal" "unicode-insert 0x006e"
		Item "Voiced retroflex nasal" "unicode-insert 0x0273"
		Item "Voiced palatal nasal" "unicode-insert 0x0272"
		Item "Voiced velar nasal" "unicode-insert 0x014b"
		Item "Voiced uvular nasal" "unicode-insert 0x0274"
		Item "Voiced bilabial trill" "unicode-insert 0x0299"
		Item "Voiced alveolar trill" "unicode-insert 0x0072"
		Item "Voiced uvular trill" "unicode-insert 0x0280"
#		Item "Voiced labiodental flap" "unicode-insert 0x2c71" // Not yet covered by TIPA
		Item "Voiced alveolar tap" "unicode-insert 0x027e"
		Item "Voiced retroflex flap" "unicode-insert 0x027d"
		Item "Voiceless bilabial fricative" "unicode-insert 0x0278"
		Item "Voiced bilabial fricative" "unicode-insert 0x03b2"
		Item "Voiceless labiodental fricative" "unicode-insert 0x0066"
		Item "Voiced labiodental fricative" "unicode-insert 0x0076"
		Item "Voiceless dental fricative" "unicode-insert 0x03b8"
		Item "Voiced dental fricative" "unicode-insert 0x00f0"
		Item "Voiceless alveolar fricative" "unicode-insert 0x0073"
		Item "Voiced alveolar fricative" "unicode-insert 0x007a"
		Item "Voiceless postalveolar fricative" "unicode-insert 0x0283"
		Item "Voiced postalveolar fricative" "unicode-insert 0x0292"
		Item "Voiceless retroflex fricative" "unicode-insert 0x0282"
		Item "Voiced retroflex fricative" "unicode-insert 0x0290"
		Item "Voiceless palatal fricative" "unicode-insert 0x00e7"
		Item "Voiced palatal fricative" "unicode-insert 0x029d"
		Item "Voiceless velar fricative" "unicode-insert 0x0078"
		Item "Voiced velar fricative" "unicode-insert 0x0263"
		Item "Voiceless uvular fricative" "unicode-insert 0x03c7"
		Item "Voiced uvular fricative" "unicode-insert 0x0281"
		Item "Voiceless pharyngeal fricative" "unicode-insert 0x0127"
		Item "Voiced pharyngeal fricative" "unicode-insert 0x0295"
		Item "Voiceless glottal fricative" "unicode-insert 0x0068"
		Item "Voiced glottal fricative" "unicode-insert 0x0266"
		Item "Voiceless alveolar lateral fricative" "unicode-insert 0x026c"
		Item "Voiced alveolar lateral fricative" "unicode-insert 0x026e"
		Item "Voiced labiodental approximant" "unicode-insert 0x028b"
		Item "Voiced alveolar approximant" "unicode-insert 0x0279"
		Item "Voiced retroflex approximant" "unicode-insert 0x027b"
		Item "Voiced palatal approximant" "unicode-insert 0x0068"
		Item "Voiced velar approximant" "unicode-insert 0x0270"
		Item "Voiced alveolar lateral approximant" "unicode-insert 0x006c"
		Item "Voiced retroflex lateral approximant" "unicode-insert 0x026d"
		Item "Voiced palatal lateral approximant" "unicode-insert 0x028e"
		Item "Voiced velar lateral approximant" "unicode-insert 0x029f"
	End

	Toolbar "ipa_nonpulmonic" "IPA Non-Pulmonic Consonants"
		Item "Bilabial click" "unicode-insert 0x0298"
		Item "Dental click" "unicode-insert 0x01c0"
		Item "(Post)alveolar click" "unicode-insert 0x01c3"
		Item "Palatoalveolar click" "unicode-insert 0x01c2"
		Item "Alveolar lateral click" "unicode-insert 0x01c1"
		Item "Voiced bilabial implosive" "unicode-insert 0x0253"
		Item "Voiced dental/alveolar implosive" "unicode-insert 0x0257"
		Item "Voiced palatal implosive" "unicode-insert 0x0284"
		Item "Voiced velar implosive" "unicode-insert 0x0260"
		Item "Voiced uvular implosive" "unicode-insert 0x029b"
		Item "Ejective mark" "unicode-insert 0x02bc"
	End

	Toolbar "ipa_vowels" "IPA Vowels"
		Item "Close front unrounded vowel" "unicode-insert 0x0069"
		Item "Close front rounded vowel" "unicode-insert 0x0079"
		Item "Close central unrounded vowel" "unicode-insert 0x0268"
		Item "Close central rounded vowel" "unicode-insert 0x0289"
		Item "Close back unrounded vowel" "unicode-insert 0x026f"
		Item "Close back rounded vowel" "unicode-insert 0x0075"
		Item "Near-close near-front unrounded vowel" "unicode-insert 0x026a"
		Item "Near-close near-front rounded vowel" "unicode-insert 0x028f"
		Item "Near-close near-back rounded vowel" "unicode-insert 0x028a"
		Item "Close-mid front unrounded vowel" "unicode-insert 0x0065"
		Item "Close-mid front rounded vowel" "unicode-insert 0x00f8"
		Item "Close-mid central unrounded vowel" "unicode-insert 0x0258"
		Item "Close-mid central rounded vowel" "unicode-insert 0x0275"
		Item "Close-mid back unrounded vowel" "unicode-insert 0x0264"
		Item "Close-mid back rounded vowel" "unicode-insert 0x006f"
		Item "Mid-central vowel (Schwa)" "unicode-insert 0x0259"
		Item "Open-mid front unrounded vowel" "unicode-insert 0x025b"
		Item "Open-mid front rounded vowel" "unicode-insert 0x0153"
		Item "Open-mid central unrounded vowel" "unicode-insert 0x025c"
		Item "Open-mid central rounded vowel" "unicode-insert 0x025e"
		Item "Open-mid back unrounded vowel" "unicode-insert 0x028c"
		Item "Open-mid back rounded vowel" "unicode-insert 0x0254"
		Item "Near-open front unrounded vowel" "unicode-insert 0x00e6"
		Item "Near-open vowel" "unicode-insert 0x0250"
		Item "Open front unrounded vowel" "unicode-insert 0x0061"
		Item "Open front rounded vowel" "unicode-insert 0x0276"
		Item "Open back unrounded vowel" "unicode-insert 0x0251"
		Item "Open back rounded vowel" "unicode-insert 0x0252"
	End

	Toolbar "ipa_others" "IPA Other Symbols"
		Item "Voiceless labial-velar fricative" "unicode-insert 0x028d"
		Item "Voiced labial-velar approximant" "unicode-insert 0x0077"
		Item "Voiced labial-palatal approximant" "unicode-insert 0x0265"
		Item "Voiceless epiglottal fricative" "unicode-insert 0x029c"
		Item "Voiced epiglottal fricative" "unicode-insert 0x02a2"
		Item "Epiglottal plosive" "unicode-insert 0x02a1"
		Item "Voiceless alveolo-palatal fricative" "unicode-insert 0x0255"
		Item "Voiced alveolo-palatal fricative" "unicode-insert 0x0291"
		Item "Voiced alveolar lateral flap" "unicode-insert 0x027a"
		Item "Simultaneous voiceless postalveolar and velar fricative" "unicode-insert 0x0267"
		Item "Top tie bar" "ipamacro-insert deco toptiebar"
		Item "Bottom tie bar" "ipamacro-insert deco bottomtiebar"
	End

	Toolbar "ipa_suprasegmentals" "IPA Suprasegmentals"
		Item "Long" "unicode-insert 0x02d0"
		Item "Half-long" "unicode-insert 0x02d1"
		Item "Extra short" "unicode-insert 0x0306"
		Item "Primary stress" "unicode-insert 0x02c8"
		Item "Secondary stress" "unicode-insert 0x02cc"
		Item "Minor (foot) group" "unicode-insert 0x007c"
		Item "Major (intonation) group" "unicode-insert 0x2016"
		Item "Syllable break" "unicode-insert 0x002e"
		Item "Linking (absence of a break)" "unicode-insert 0x203f"
	End

	Toolbar "ipa_diacritics" "IPA Diacritics"
		Item "Voiceless" "unicode-insert 0x0325"
		Item "Voiceless (above)" "unicode-insert 0x030a"
		Item "Voiced" "unicode-insert 0x032c"
		Item "Breathy voiced" "unicode-insert 0x0324"
		Item "Creaky voiced" "unicode-insert 0x0330"
		Item "Linguolabial" "unicode-insert 0x033c"
		Item "Dental" "unicode-insert 0x032a"
		Item "Apical" "unicode-insert 0x033a"
		Item "Laminal" "unicode-insert 0x033b"
		Item "Aspirated" "unicode-insert 0x02b0"
		Item "More rounded" "unicode-insert 0x0339"
		Item "Less rounded" "unicode-insert 0x031c"
		Item "Advanced" "unicode-insert 0x031f"
		Item "Retracted" "unicode-insert 0x0320"
		Item "Centralized" "unicode-insert 0x0308"
		Item "Mid-centralized" "unicode-insert 0x033d"
		Item "Syllabic" "unicode-insert 0x0329"
		Item "Non-syllabic" "unicode-insert 0x032f"
		Item "Rhoticity" "unicode-insert 0x02de"
		Item "Labialized" "unicode-insert 0x02b7"
		Item "Palatized" "unicode-insert 0x02b2"
		Item "Velarized" "unicode-insert 0x02e0"
		Item "Pharyngialized" "unicode-insert 0x02e4"
		Item "Velarized or pharyngialized" "unicode-insert 0x0334"
		Item "Raised" "unicode-insert 0x031d"
		Item "Lowered" "unicode-insert 0x031e"
		Item "Advanced tongue root" "unicode-insert 0x0318"
		Item "Retracted tongue root" "unicode-insert 0x0319"
		Item "Nasalized" "unicode-insert 0x0303"
		Item "Nasal release" "unicode-insert 0x207f"
		Item "Lateral release" "unicode-insert 0x02e1"
		Item "No audible release" "unicode-insert 0x02fa"
	End

	Toolbar "ipa_accents" "IPA Tones and Word Accents"
		Item "Extra high (accent)" "unicode-insert 0x030b"
		Item "Extra high (tone letter)" "unicode-insert 0x02e5"
		Item "High (accent)" "unicode-insert 0x0300"
		Item "High (tone letter)" "unicode-insert 0x02e6"
		Item "Mid (accent)" "unicode-insert 0x0304"
		Item "Mid (tone letter)" "unicode-insert 0x02e7"
		Item "Low (accent)" "unicode-insert 0x0301"
		Item "Low (tone letter)" "unicode-insert 0x02e8"
		Item "Extra low (accent)" "unicode-insert 0x030f"
		Item "Extra low (tone letter)" "unicode-insert 0x02e9"
		Item "Downstep" "unicode-insert 0xa71c"
		Item "Upstep" "unicode-insert 0xa71b"
		Item "Rising (accent)" "unicode-insert 0x030c"
		Item "Rising (tone letter)" "ipamacro-insert tone-rising"
		Item "Falling (accent)" "unicode-insert 0x0302"
		Item "Falling (tone letter)" "ipamacro-insert tone-falling"
		Item "High rising (accent)" "unicode-insert 0x1dc4"
		Item "High rising (tone letter)" "ipamacro-insert tone-high-rising"
		Item "Low rising (accent)" "unicode-insert 0x1dc5"
		Item "Low rising (tone letter)" "ipamacro-insert tone-low-rising"
		Item "Rising-falling (accent)" "unicode-insert 0x1dc8"
		Item "Rising-falling (tone letter)" "ipamacro-insert tone-high-rising-falling"
		Item "Global rise" "unicode-insert 0x2197"
		Item "Global fall" "unicode-insert 0x2198"
	End

End
