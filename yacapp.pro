QT += quick widgets network webview sql multimedia svg pdf

DEFINES += YACAPPApp
DEFINES += DISABLEQZXING

QZXINGPATH = qzxing/src/QZXing.pri
android {
QZXINGPATH =
}

!android {
#CONFIG += qzxing_qml
#CONFIG += qzxing_multimedia
include($$QZXINGPATH)
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += ORMQTTypes
include("yacAppAndServer/yacAppAndServer.pri")


SOURCES += \
        configmodels/appimagesitem.cpp \
        configmodels/appuserconfig.cpp \
        configmodels/backgroundconfig.cpp \
        configmodels/configinterface.cpp \
        configmodels/contentconfig.cpp \
        configmodels/contentitem.cpp \
        configmodels/globalprojectconfig.cpp \
        configmodels/headerconfig.cpp \
        configmodels/menueconfig.cpp \
        configmodels/menueitem.cpp \
        configmodels/parsedconfig.cpp \
        configmodels/splashscreenconfig.cpp \
        configurator.cpp \
        configuratormodels/recentproject.cpp \
        constants.cpp \
        cppqmlobjects/cppqmlappandconfigurator.cpp \
        cppqmlobjects/thirdpartylogin.cpp \
        datamodels/asyncimageresponse.cpp \
        datamodels/datamodelqobjectinterface.cpp \
        datamodels/messagesmodel.cpp \
        datamodels/objectdatamodel.cpp \
        datamodels/profilesmodel.cpp \
        datamodels/transmissiontracker.cpp \
        dataobjects/appointmentobject.cpp \
        dataobjects/dataobjectinterface.cpp \
        dataobjects/groupobject.cpp \
        dataobjects/messageobject.cpp \
        dataobjects/multipurposeobject.cpp \
        dataobjects/newsobject.cpp \
        dataobjects/profileobject.cpp \
        dataobjects/spaceobject.cpp \
        dataobjects/spacerequestobject.cpp \
        dataobjects/worktimemainobject.cpp \
        dataobjects/worktimeobject.cpp \
        designoptions/globaldesignconfig.cpp \
        designoptions/mainmenueconfig.cpp \
        designoptions/multimenueconfig.cpp \
        designoptions/newspageconfig.cpp \
        firebase2qt.cpp \
        helper.cpp \
        localstorage/localstorage.cpp \
        localstorage/localtablenames.cpp \
        logger.cpp \
        main.cpp \
        network/asyncimageprovider.cpp \
        network/customservernetwork.cpp \
        network/networkinterface.cpp \
        network/yacextservernetwork.cpp \
        network/yacserverbasenetwork.cpp \
        network/yacservernetwork.cpp \
        opensslwrapper.cpp \
        orm-mapper/orm2qjson.cpp \
        projectdata.cpp \
        yacapp.cpp

RESOURCES += qml.qrc \
  fonts.qrc \
  images.qrc

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
  configmodels/appimagesitem.h \
  configmodels/appuserconfig.h \
  configmodels/backgroundconfig.h \
  configmodels/configinterface.h \
  configmodels/contentconfig.h \
  configmodels/contentitem.h \
  configmodels/globalprojectconfig.h \
  configmodels/headerconfig.h \
  configmodels/menueconfig.h \
  configmodels/menueitem.h \
  configmodels/parsedconfig.h \
  configmodels/splashscreenconfig.h \
  configurator.h \
  configuratormodels/recentproject.h \
  constants.h \
  cppqmlobjects/cppqmlappandconfigurator.h \
  cppqmlobjects/thirdpartylogin.h \
  datamodels/asyncimageresponse.h \
  datamodels/datamodelinterface.h \
  datamodels/datamodelqobjectinterface.h \
  datamodels/exttemplateddatamodel.h \
  datamodels/messagesmodel.h \
  datamodels/objectdatamodel.h \
  datamodels/profilesmodel.h \
  datamodels/templateddatamodel.h \
  datamodels/transmissiontracker.h \
  dataobjects/appointmentobject.h \
  dataobjects/dataobjectinterface.h \
  dataobjects/groupobject.h \
  dataobjects/messageobject.h \
  dataobjects/multipurposeobject.h \
  dataobjects/newsobject.h \
  dataobjects/profileobject.h \
  dataobjects/spaceobject.h \
  dataobjects/spacerequestobject.h \
  dataobjects/worktimemainobject.h \
  dataobjects/worktimeobject.h \
  designoptions/globaldesignconfig.h \
  designoptions/mainmenueconfig.h \
  designoptions/multimenueconfig.h \
  designoptions/newspageconfig.h \
  firebase2qt.h \
  helper.h \
  localstorage/localstorage.h \
  localstorage/localtablenames.h \
  logger.h \
  network/asyncimageprovider.h \
  network/customservernetwork.h \
  network/networkinterface.h \
  network/yacextservernetwork.h \
  network/yacserverbasenetwork.h \
  network/yacservernetwork.h \
  opensslwrapper.h \
  orm-mapper/orm2qjson.h \
  projectdata.h \
  yacapp.h \
  yacappmacros.h

DISTFILES += \
  ../../.local/share/jw78/yacapp/yacAppState.json \
  .gitignore \
  README.md \
  android/AndroidManifest.xml \
  android/build.gradle \
  android/google-services.json \
  android/gradle.properties \
  android/gradle/wrapper/gradle-wrapper.jar \
  android/gradle/wrapper/gradle-wrapper.properties \
  android/gradlew \
  android/gradlew.bat \
  android/res/values/libs.xml \
  android/res/xml/qtprovider_paths.xml \
  changelog.txt \
  currentWork.txt \
  deployToJW78.sh \
  otherYacApp.sh


ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += yacappfirebase.h

message(QT_ARCH: $$QT_ARCH)
message(QMAKESPEC: $$QMAKESPEC)

contains(QT_ARCH, x86_64) {
  message("compiling for linux")
  LIBS += -lssl -lcrypto
}

#QT_ANDROID_MIN_SDK_VERSION = 33
#QT_ANDROID_TARGET_SDK_VERSION = 33

android {
  QMAKE_CXXFLAGS += -std=c++2a
  SOURCES += yacappfirebase.cpp
  message("main pro android")
#  include(/home/jw78/Android/Sdk/android_openssl/openssl.pri)
  INCLUDEPATH += /home/jw78/Android/android_openssl/ssl_3/include
  INCLUDEPATH += $$PWD/firebase_cpp_sdk/include

#  QT += androidextras
  QT += core-private

  LIBS += -L$$PWD/firebase_cpp_sdk/libs/android/arm64-v8a -lfirebase_app -lfirebase_messaging
  LIBS += -lssl -lcrypto
  LIBS += -L/home/jw78/Android/android_openssl/ssl_3/arm64-v8a
}

android: include(/home/jw78/Android/android_openssl/openssl.pri)
