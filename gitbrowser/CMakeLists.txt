cmake_minimum_required(VERSION 2.6.4)
set(PROJECT gitbrowser)

project(${PROJECT})

find_package( Qt5 REQUIRED COMPONENTS Core Quick Declarative Gui Widgets Network Sql QuickWidgets)

if(WIN32)
    SET(GIT_PATH "" CACHE PATH "Path to Git")
endif()

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../qfs)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../qfs)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(CMAKE_SHARED_LIBRARY_PREFIX "")

set(HEADERS
    git/commitinfo.h
    git/gitmanager.h
    git/shell.h
    filesbrowser.h
    newdocument.h
    reposettings.h
    syncdialog.h
    texteditor.h
    versionbrowserdialog.h
)

set(SOURCES
    main.cpp
    git/commitinfo.cpp
    git/gitmanager.cpp
    git/shell.cpp
    filesbrowser.cpp
    newdocument.cpp
    reposettings.cpp
    syncdialog.cpp
    texteditor.cpp
    versionbrowserdialog.cpp
)

set(UI
    filesbrowser.ui
    newdocument.ui
    reposettings.ui
    syncdialog.ui
    texteditor.ui
    versionbrowserdialog.ui
)

set(RESOURCES
    gitbrowser.qrc
)

set(QMLS
    WaitingWindow.qml
)
SET (MOC_SOURCES )
SET (UIS )
SET (RESSRC )
qt5_wrap_cpp (MOC_SOURCES ${HEADERS})
qt5_wrap_ui (UIS ${UI})
qt5_add_resources (RESSRC ${RESOURCES})

if(COMMAND cmake_policy)
    if(POLICY CMP0028)
        cmake_policy(SET CMP0028 OLD)
    endif()
    if(POLICY CMP0020)
        cmake_policy(SET CMP0020 OLD)
    endif()
endif()

if (${UNIX})
    set(CMAKE_CXX_FLAGS "-fPIC")
endif (${UNIX})

add_library ( ${PROJECT} SHARED ${HEADERS} ${SOURCES} ${QMLS} ${MOC_SOURCES} ${UIS} ${RESSRC})
#add_executable(${PROJECT} ${HEADERS} ${SOURCES} ${MOC_SOURCES} ${UIS} ${RESSRC})

qt5_use_modules(${PROJECT} Widgets Quick Declarative Core Network Sql QuickWidgets)

target_link_libraries(${PROJECT} qfs)
install(TARGETS ${PROJECT} DESTINATION ${CMAKE_BINARY_DIR}/bin)
install(FILES ${QMLS} DESTINATION ${CMAKE_BINARY_DIR}/bin)
#if(WIN32)
#    SET(git_files
#        bin/basename.exe
#        bin/git.exe
#        bin/libiconv-2.dll
#        bin/msys-1.0.dll
#        bin/msys-crypto-1.0.0.dll
#        bin/msys-minires.dll
#        bin/msys-z.dll
#        bin/sed.exe
#        bin/sh.exe
#        bin/ssh.exe
#        bin/tr.exe
#        bin/uname.exe
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION ${CMAKE_BINARY_DIR}/bin/git/bin)
#    endforeach()
#    SET(git_files
#        etc/fileList-bindimage.txt
#        etc/fileList-builtins.txt
#        etc/git-completion.bash
#        etc/git-prompt.sh
#        etc/git.ico
#        etc/gitattributes
#        etc/inputrc
#        etc/motd
#        etc/profile
#        etc/termcap
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION ${CMAKE_BINARY_DIR}/bin/git/etc)
#    endforeach()
#    SET(git_files
#        libexec/git-core/git-merge-base.exe
#        libexec/git-core/git-merge-file.exe
#        libexec/git-core/git-merge-index.exe
#        libexec/git-core/git-merge-ours.exe
#        libexec/git-core/git-merge-recursive.exe
#        libexec/git-core/git-merge-subtree.exe
#        libexec/git-core/git-merge-tree.exe
#        libexec/git-core/git-merge.exe
#        libexec/git-core/git-pull
#        libexec/git-core/git-sh-i18n
#        libexec/git-core/git-sh-setup
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION ${CMAKE_BINARY_DIR}/bin/git/libexec/git-core)
#    endforeach()
#    SET(git_files
#        share/antiword/8859-1.txt
#        share/antiword/8859-10.txt
#        share/antiword/8859-11.txt
#        share/antiword/8859-13.txt
#        share/antiword/8859-14.txt
#        share/antiword/8859-15.txt
#        share/antiword/8859-16.txt
#        share/antiword/8859-2.txt
#        share/antiword/8859-3.txt
#        share/antiword/8859-4.txt
#        share/antiword/8859-5.txt
#        share/antiword/8859-6.txt
#        share/antiword/8859-7.txt
#        share/antiword/8859-8.txt
#        share/antiword/8859-9.txt
#        share/antiword/cp1250.txt
#        share/antiword/cp1251.txt
#        share/antiword/cp1252.txt
#        share/antiword/cp437.txt
#        share/antiword/cp850.txt
#        share/antiword/cp852.txt
#        share/antiword/cp862.txt
#        share/antiword/cp864.txt
#        share/antiword/cp866.txt
#        share/antiword/fontnames
#        share/antiword/koi8-r.txt
#        share/antiword/koi8-u.txt
#        share/antiword/MacCyrillic.txt
#        share/antiword/MacRoman.txt
#        share/antiword/roman.txt
#        share/antiword/UTF-8.txt
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION ${CMAKE_BINARY_DIR}/bin/git/share/antiword)
#    endforeach()
#    SET(git_files
#        share/git-core/templates/hooks/applypatch-msg.sample
#        share/git-core/templates/hooks/commit-msg.sample
#        share/git-core/templates/hooks/post-update.sample
#        share/git-core/templates/hooks/pre-applypatch.sample
#        share/git-core/templates/hooks/pre-commit.sample
#        share/git-core/templates/hooks/pre-push.sample
#        share/git-core/templates/hooks/pre-rebase.sample
#        share/git-core/templates/hooks/prepare-commit-msg.sample
#        share/git-core/templates/hooks/update.sample
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION ${CMAKE_BINARY_DIR}/bin/git/share/git-core/templates/hooks)
#    endforeach()
#    SET(git_files
#        share/git-core/templates/info/exclude
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION "${CMAKE_BINARY_DIR}/bin/git/share/git-core/templates/info")
#    endforeach()
#    SET(git_files
#        share/git-core/templates/description
#    )
#    foreach(one_git_file ${git_files})
#        install (FILES "${GIT_PATH}/${one_git_file}" DESTINATION "${CMAKE_BINARY_DIR}/bin/git/share/git-core/templates")
#    endforeach()
#    install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/git-config/ssh/config" DESTINATION "${CMAKE_BINARY_DIR}/bin/git/.ssh")
#    install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/git-config/etc/gitconfig" DESTINATION "${CMAKE_BINARY_DIR}/bin/git/etc")
#endif()
