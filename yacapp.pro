QT += quick widgets network webview sql

android: include(/home/jw78/Android/Sdk/android_openssl/openssl.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include("JW78CurlLib/JW78CurlLib.pri")


SOURCES += \
        configmodels/appuserconfig.cpp \
        configmodels/backgroundconfig.cpp \
        configmodels/configinterface.cpp \
        configmodels/contentconfig.cpp \
        configmodels/contentitem.cpp \
        configmodels/footerconfig.cpp \
        configmodels/globalprojectconfig.cpp \
        configmodels/headerconfig.cpp \
        configmodels/menueconfig.cpp \
        configmodels/menueitem.cpp \
        configmodels/parsedconfig.cpp \
        configmodels/splashscreenconfig.cpp \
        configurator.cpp \
        configuratormodels/recentproject.cpp \
        constants.cpp \
        datamodels/messagesmodel.cpp \
        datamodels/objectdatamodel.cpp \
        datamodels/profilesmodel.cpp \
        dataobjects/dataobjectinterface.cpp \
        dataobjects/groupobject.cpp \
        dataobjects/messageobject.cpp \
        dataobjects/profileobject.cpp \
        helper.cpp \
        localstorage/localstorage.cpp \
        localstorage/tablenames.cpp \
        main.cpp \
        network/customservernetwork.cpp \
        network/networkinterface.cpp \
        network/yacextservernetwork.cpp \
        network/yacserverbasenetwork.cpp \
        network/yacservernetwork.cpp \
        projectdata.cpp \
        yacapp.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    yacapp_de_DE.ts
CONFIG += lrelease
CONFIG += embed_translations

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  configmodels/appuserconfig.h \
  configmodels/backgroundconfig.h \
  configmodels/configinterface.h \
  configmodels/contentconfig.h \
  configmodels/contentitem.h \
  configmodels/footerconfig.h \
  configmodels/globalprojectconfig.h \
  configmodels/headerconfig.h \
  configmodels/menueconfig.h \
  configmodels/menueitem.h \
  configmodels/parsedconfig.h \
  configmodels/splashscreenconfig.h \
  configurator.h \
  configuratormodels/recentproject.h \
  constants.h \
  datamodels/datamodelinterface.h \
  datamodels/exttemplateddatamodel.h \
  datamodels/messagesmodel.h \
  datamodels/objectdatamodel.h \
  datamodels/profilesmodel.h \
  datamodels/templateddatamodel.h \
  dataobjects/dataobjectinterface.h \
  dataobjects/groupobject.h \
  dataobjects/messageobject.h \
  dataobjects/profileobject.h \
  helper.h \
  localstorage/localstorage.h \
  localstorage/tablenames.h \
  network/customservernetwork.h \
  network/networkinterface.h \
  network/yacextservernetwork.h \
  network/yacserverbasenetwork.h \
  network/yacservernetwork.h \
  projectdata.h \
  yacapp.h \
  yacappmacros.h

DISTFILES += \
  README.md \
  android/AndroidManifest.xml \
  android/build.gradle \
  android/gradle.properties \
  android/gradle/wrapper/gradle-wrapper.jar \
  android/gradle/wrapper/gradle-wrapper.properties \
  android/gradlew \
  android/gradlew.bat \
  android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
