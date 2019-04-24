#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T15:31:33
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += quick qml multimedia multimediawidgets quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = moleculeIdentificator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    identificator.cpp \
    editorscene.cpp \
    atomgraphicitem.cpp \
    atom.cpp \
    bondgraphicsitem.cpp \
    moleculeinfoview.cpp \
    molecule.cpp \
    api.cpp \
    periodictable.cpp \
    ImageRecognition/bonddetector.cpp \
    ImageRecognition/atomfinder.cpp \
    ImageRecognition/ocr.cpp \
    imagecapture.cpp \
    ImageRecognition/moleculegrabber.cpp \
    imagecropdialog.cpp \
    cropscene.cpp \
    imagereader.cpp \
    editorui.cpp \
    editorview.cpp

HEADERS  += mainwindow.h \
    identificator.h \
    editorscene.h \
    atomgraphicitem.h \
    atom.h \
    bondgraphicsitem.h \
    moleculeinfoview.h \
    molecule.h \
    api.h \
    periodictable.h \
    ImageRecognition/bonddetector.h \
    ImageRecognition/atomfinder.h \
    ImageRecognition/ocr.h \
    imagecapture.h \
    ImageRecognition/moleculegrabber.h \
    imagecropdialog.h \
    cropscene.h \
    imagereader.h \
    editorui.h \
    editorview.h

FORMS    += mainwindow.ui \
    moleculeinfoview.ui \
    imagecapture.ui \
    imagecropdialog.ui \
    editorui.ui

CONFIG += mobility
MOBILITY =

RESOURCES +=  moleculeidentificator.qrc


#staticLink
#DEPENDPATH += $$PWD/../../../../usr/local/include
#PRE_TARGETDEPS +=  /usr/local/lib/libopencv_ts.a

#OpenCV library
#LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_features2d
#INCLUDEPATH += $$PWD/../../../../usr/local/include

DISTFILES += \Camera.qml


contains(ANDROID_TARGET_ARCH,x86) {
    INCLUDEPATH += $$PWD/../../koodi/OpenCV-android-sdk/sdk/native/jni/include/

    LIBS += -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/staticlibs/armeabi-v7a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/staticlibs/x86/ \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/ibs/armeabi-v7a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/x86 \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/3party/libs/x86  \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/arm64-v8a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/3rdparty/libs/arm64-v8a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/staticlibs/arm64-v8a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/staticlibs/armabi \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/armabi\
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/3rdparty/libs/armabi \
-lopencv_java3\
-lopencv_calib3d     -lopencv_highgui    -lopencv_photo      -lopencv_videoio\
-lopencv_core        -lopencv_imgcodecs  -lopencv_shape      -lopencv_videostab\
-lopencv_dnn         -lopencv_imgproc    -lopencv_stitching\
-lopencv_features2d  -lopencv_ml         -lopencv_superres\
-lopencv_flann       -lopencv_objdetect  -lopencv_video\


    ANDROID_EXTRA_LIBS = \
        $$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/x86/libopencv_java3.so
}


#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += opencv4


contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    INCLUDEPATH += $$PWD/../../koodi/OpenCV-android-sdk/sdk/native/jni/include/

    LIBS +=    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/arm64-v8a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/3rdparty/libs/arm64-v8a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/staticlibs/arm64-v8a \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/staticlibs/armabi \
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/armabi\
    -L$$PWD/../../koodi/OpenCV-android-sdk/sdk/native/3rdparty/libs/armabi \
-lopencv_java3\
-lopencv_calib3d     -lopencv_highgui    -lopencv_photo      -lopencv_videoio\
-lopencv_core        -lopencv_imgcodecs  -lopencv_shape      -lopencv_videostab\
-lopencv_dnn         -lopencv_imgproc    -lopencv_stitching\
-lopencv_features2d  -lopencv_ml         -lopencv_superres\
-lopencv_flann       -lopencv_objdetect  -lopencv_video\

    ANDROID_EXTRA_LIBS = \
        $$PWD/../../koodi/OpenCV-android-sdk/sdk/native/libs/arm64-v8a/libopencv_java3.so
}
