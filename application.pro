QT += widgets multimedia multimediawidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    codeeditor.h \
    loghelper.h \
    playercontrols.h \
    playmedia.h \
    videowidget.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    codeeditor.cpp \
    playercontrols.cpp \
    playmedia.cpp \
    videowidget.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
