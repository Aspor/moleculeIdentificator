#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T15:31:33
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = moleculeIdentificator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    identificator.cpp \
    editorview.cpp \
    editorscene.cpp \
    atomgraphicitem.cpp \
    atom.cpp \
    bondgraphicsitem.cpp \
    moleculeinfoview.cpp \
    molecule.cpp \
    api.cpp \
    periodictable.cpp \
    ImageRecognition/bonddetector.cpp \
    ImageRecognition/blobdetector.cpp \
    ImageRecognition/atomfinder.cpp \
    ImageRecognition/ocr.cpp

HEADERS  += mainwindow.h \
    identificator.h \
    editorview.h \
    editorscene.h \
    atomgraphicitem.h \
    atom.h \
    bondgraphicsitem.h \
    moleculeinfoview.h \
    molecule.h \
    api.h \
    periodictable.h \
    ImageRecognition/bonddetector.h \
    ImageRecognition/blobdetector.h \
    ImageRecognition/atomfinder.h \
    ImageRecognition/ocr.h

FORMS    += mainwindow.ui \
    editorview.ui \
    moleculeinfoview.ui

CONFIG += mobility
MOBILITY =

RESOURCES +=  icons/minus.png \
              icons/moveArrow.png \
              icons/plus.png

#staticLink
#DEPENDPATH += $$PWD/../../../../usr/local/include
#PRE_TARGETDEPS +=  /usr/local/lib/libopencv_ts.a

#OpenCV library
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_features2d -llzma
INCLUDEPATH += $$PWD/../../../../usr/local/include
