QT += widgets multimedia multimediawidgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    common/loghelper.h \
    TextModel/codeeditor.h \
    common/qslidercustom.h \
    MediaModel/playmedia.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    TextModel/codeeditor.cpp \
    common/qslidercustom.cpp \
    MediaModel/playmedia.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
