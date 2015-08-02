# SciLyx

SciLyx is a text editor with embedded versions control. SciLyx based on LyX, but has a number of improvements:

  - SciLyx has a file manager, that allows you to work with a lyx files from git repo using GUI
  - SciLyx has an GUI to draw charts with mathematical graphics
  - SciLyx stores images directly in lyx document
  - SciLyx has an API to write a plugins for extending it's features

### Requirements
Install it before building SciLyx:
  - Qt5
  - CMake
  - git
  - TexLive (for Windows - add it in Path variable)
  - Python (for Windows - add it in Path variable)
  - GhostScript (for Windows - add it in Path variable)

### Building
To build the SciLyx folow this instructions:
  1. Clone this repo to get SciLyx sources
  2. Use cmake-gui to set the CMake variables (replace pathes to own):
  
     GIT_PATH=C:/Program Files (x86)/Git

     GNUWIN32_DIR=C:/gnuwin32
     
  3. cmake
  4. make
  5. make install
Now you have SciLyx binaries in **bin** folder.
