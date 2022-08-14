QT += quick widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        configmodels/backgroundconfig.cpp \
        configmodels/contentconfig.cpp \
        configmodels/contentitem.cpp \
        configmodels/footerconfig.cpp \
        configmodels/globalprojectconfig.cpp \
        configmodels/headerconfig.cpp \
        configmodels/menueconfig.cpp \
        configmodels/menueitem.cpp \
        configmodels/parsedconfig.cpp \
        main.cpp \
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
  configmodels/backgroundconfig.h \
  configmodels/contentconfig.h \
  configmodels/contentitem.h \
  configmodels/footerconfig.h \
  configmodels/globalprojectconfig.h \
  configmodels/headerconfig.h \
  configmodels/menueconfig.h \
  configmodels/menueitem.h \
  configmodels/parsedconfig.h \
  yacapp.h \
  yacappmacros.h
