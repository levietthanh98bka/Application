QT += widgets
requires(qtConfig(filedialog))

HEADERS       = mainwindow.h \
    codeeditor.h \
    loghelper.h
SOURCES       = main.cpp \
                mainwindow.cpp \
    codeeditor.cpp
#! [0]
RESOURCES     = application.qrc
#! [0]

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/application
INSTALLS += target
