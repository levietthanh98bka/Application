QT += widgets multimedia multimediawidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    common/loghelper.h \
    TextModel/codeeditor.h \
    MediaModel/playercontrols.h \
    MediaModel/playmedia.h \
    MediaModel/qslidercustom.h \
    MediaModel/videowidget.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    TextModel/codeeditor.cpp \
    MediaModel/playercontrols.cpp \
    MediaModel/playmedia.cpp \
    MediaModel/qslidercustom.cpp \
    MediaModel/videowidget.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
