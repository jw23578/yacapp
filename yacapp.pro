QT += quick widgets network webview sql multimedia svg

DEFINES += YACAPPApp

CONFIG += qzxing_qml
CONFIG += qzxing_multimedia
include(qzxing/src/QZXing.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include("yacAppAndServer/yacAppAndServer.pri")


SOURCES += \
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
        datamodels/appointmentsmodel.cpp \
        datamodels/asyncimageresponse.cpp \
        datamodels/messagesmodel.cpp \
        datamodels/objectdatamodel.cpp \
        datamodels/profilesmodel.cpp \
        dataobjects/appointmentobject.cpp \
        dataobjects/dataobjectinterface.cpp \
        dataobjects/groupobject.cpp \
        dataobjects/messageobject.cpp \
        dataobjects/multipurposeobject.cpp \
        dataobjects/profileobject.cpp \
        dataobjects/rightgroupobject.cpp \
        dataobjects/spaceobject.cpp \
        dataobjects/worktimemainobject.cpp \
        dataobjects/worktimeobject.cpp \
        designoptions/globaldesignconfig.cpp \
        designoptions/mainmenueconfig.cpp \
        designoptions/multimenueconfig.cpp \
        designoptions/newspageconfig.cpp \
        firebase2qt.cpp \
        helper.cpp \
        localstorage/localstorage.cpp \
        localstorage/tablenames.cpp \
        main.cpp \
        network/asyncimageprovider.cpp \
        network/customservernetwork.cpp \
        network/networkinterface.cpp \
        network/yacextservernetwork.cpp \
        network/yacserverbasenetwork.cpp \
        network/yacservernetwork.cpp \
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
  datamodels/appointmentsmodel.h \
  datamodels/asyncimageresponse.h \
  datamodels/datamodelinterface.h \
  datamodels/exttemplateddatamodel.h \
  datamodels/messagesmodel.h \
  datamodels/objectdatamodel.h \
  datamodels/profilesmodel.h \
  datamodels/templateddatamodel.h \
  dataobjects/appointmentobject.h \
  dataobjects/dataobjectinterface.h \
  dataobjects/groupobject.h \
  dataobjects/messageobject.h \
  dataobjects/multipurposeobject.h \
  dataobjects/profileobject.h \
  dataobjects/rightgroupobject.h \
  dataobjects/spaceobject.h \
  dataobjects/worktimemainobject.h \
  dataobjects/worktimeobject.h \
  designoptions/globaldesignconfig.h \
  designoptions/mainmenueconfig.h \
  designoptions/multimenueconfig.h \
  designoptions/newspageconfig.h \
  firebase2qt.h \
  helper.h \
  localstorage/localstorage.h \
  localstorage/tablenames.h \
  network/asyncimageprovider.h \
  network/customservernetwork.h \
  network/networkinterface.h \
  network/yacextservernetwork.h \
  network/yacserverbasenetwork.h \
  network/yacservernetwork.h \
  projectdata.h \
  yacapp.h \
  yacappmacros.h

DISTFILES += \
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
  android/settings.gradle \
  changelog.txt \
  deployToJW78.sh

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += yacappfirebase.h

android {
  SOURCES += yacappfirebase.cpp
  message("main pro android")
  include(/home/jw78/Android/Sdk/android_openssl/openssl.pri)
  INCLUDEPATH += $$PWD/firebase_cpp_sdk/include

  QT += androidextras

  LIBS += -L$$PWD/firebase_cpp_sdk/libs/android/armeabi-v7a -lfirebase_app -lfirebase_messaging
}
