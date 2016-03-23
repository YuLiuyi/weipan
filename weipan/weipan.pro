#-------------------------------------------------
#
# Project generated by QtCreator for SyberOS.
#
#-------------------------------------------------

QML_IMPORT_PATH = $$[QT_INSTALL_QML]

RESOURCES += res.qrc

CONFIG += link_pkgconfig

PKGCONFIG += syberos-application syberos-application-cache

# This is needed for using syberos-application and syberos-qt module
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/../syberos_application \
			   $$[QT_INSTALL_HEADERS]/../framework

QT += gui qml quick
QT += core gui network

TARGET = MyApp

#Please Do not modify these macros, otherwise your app will not installed correctly and will not run at all.
APP_DIR = /data/apps
APP_DATA = /data/data
INS_DIR = $$APP_DIR/com.mycompany.weiPan
DATA_DIR = $$APP_DATA/com.mycompany.weiPan

DEFINES += SOP_ID=\\\"com.mycompany.weiPan\\\"
DEFINES += APP_DIR_ENVVAR=\\\"APPDIR_REGULAR\\\"
# Currently home screen sets the environment variable, so when run from
# elsewhere, use this work-around instead.
DEFINES += APP_DIR=\\\"$$APP_DIR\\\"

# The .cpp file which was generated for your project.
SOURCES += src/main.cpp \
           src/MyApp_Workspace.cpp \
    src/controller.cpp \
    src/publicFun.cpp \
    src/mainListModel.cpp \
    src/filesInfo.cpp \
    src/thread.cpp

HEADERS += src/MyApp_Workspace.h \
    src/controller.h \
    src/publicFun.h \
    src/mainListModel.h \
    src/filesInfo.h \
    src/thread.h

# Installation path
target.path = $$INS_DIR/bin

res.files = res
res.path = $$INS_DIR/

meta.files = META-INF
meta.path = $$INS_DIR/

syber.files = sopconfig.xml
syber.path = $$INS_DIR/

INSTALLS += target res meta syber

QML_FILES = qml/*.qml

OTHER_FILES += $$QML_FILES *.qm \
    qml/MainPage.qml \
    qml/UserInfo.qml \
    qml/Upload.qml \
    qml/InputDialog.qml \
    qml/ShowFile.qml \
    qml/ShowPage.qml
               


