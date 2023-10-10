import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "qml"
import "qml/items"
import "qml/apppages"
import "qml/dialogs"
import "qml/menue"

ApplicationWindow
{
    visible: true
    width: 430
    height: 932
    title: yacApp.applicationTitle

    DefaultsSetter {}

    //    onActiveFocusItemChanged:
    //    {
    //        var count = 0
    //        var elem = activeFocusItem
    //        while (elem)
    //        {
    //            print("activeFocusItem " + count + " " + elem)
    //            elem = elem.parent
    //            count += 1
    //        }
    //    }

    Loader
    {
        focus: true
        anchors.fill: parent
        id: mainformLoader
        active: yacApp.mainConfig != null && yacApp.globalConfig.projectID != ""
        source: "qml/MainForm.qml"
    }

    SelectAppForm
    {
        id: selectApp
        visible: yacApp.globalConfig.projectID == ""

    }

    SequentialAnimation
    {
        id: startUpAni
        PauseAnimation
        {
            duration: 3000
        }
        NumberAnimation
        {
            target: splashscreen
            property: "opacity"
            to: 0
            duration: 500
        }
    }
    YACRectangle
    {
        id: splashscreen
        opacity: yacApp.secondStart ? 0 : 1
        visible: opacity > 0
        anchors.fill: parent
        color: "silver"
        shadowEnabled: false
        YACText
        {
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            text: "This is the yacApp"
            font.bold: true
            font.pixelSize: width / 10
            horizontalAlignment: Text.AlignHCenter
        }
        onVisibleChanged:
        {
            Constants.topLevelFormActive = badMessage.visible || goodMessage.visible || yesNoQuestion.visible || splashscreen.visible
            Helper.jsLog("Constants.topLevelFormActive: " + Constants.topLevelFormActive)
        }
    }
    Component
    {
        id: photoComponent
        YACPhoto
        {
            onAbortClicked:
            {
                photoLoader.sourceComponent = null
            }
            onOkClicked:
            {
                target.source = ""
                target.source = image.source
                photoLoader.sourceComponent = null
            }
        }
    }

    Loader
    {
        id: photoLoader
        property bool squared: false
        property bool circled: false
        property var target: null
        anchors.fill: parent
        onLoaded:
        {
            item.circled = circled
            item.squared = squared
        }
    }
    Component
    {
        id: qrScannerComponent
        YACQRScanner
        {

        }
    }

    Loader
    {
        id: qrScannerLoader
        anchors.fill: parent
    }
    DialogDateTimePicker
    {
        visible: false
        id: dateTimePicker
        onVisibleChanged: topLevelVisible()
    }
    DialogDatePicker
    {
        visible: false
        id: datePicker
        onVisibleChanged: topLevelVisible()
    }
    DialogTimePicker
    {
        visible: false
        id: timePicker
        onVisibleChanged: topLevelVisible()
    }
    function topLevelVisible()
    {
        Constants.topLevelFormActive = Constants.superMenueOpen ||  Constants.superMenueClicked || badMessage.visible || goodMessage.visible || yesNoQuestion.visible || splashscreen.visible || timePicker.visible || datePicker.visible || dateTimePicker.visible
    }

    YACBadMessageForm
    {
        id: badMessage
        onVisibleChanged: topLevelVisible()
    }
    YACGoodMessageForm
    {
        id: goodMessage
        onVisibleChanged: topLevelVisible()
    }
    YACYesNoQuestionForm
    {
        id: yesNoQuestion
        onVisibleChanged: topLevelVisible()
    }
    Rectangle
    {
        anchors.fill: parent
        color: Constants.badColor
        visible: yacApp.serverConnectionDefectMessage.length > 0
        Column
        {
            width: parent.width * 3/4
            anchors.centerIn: parent
            YACText
            {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                text: "Netzwerkfehler!"
            }
            YACText
            {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                text: yacApp.serverConnectionDefectMessage
            }
        }
    }

    function checkForAppUpdate()
    {
        if (!yacApp.globalConfig.projectID.length)
        {
            Helper.jsLog("no current app active -> no update check")
            return;
        }

        Helper.jsLog("checking for updates");
        yacApp.yacappServerGetAPP(yacApp.globalConfig.projectID,
                                  "",
                                  yacApp.globalConfig.version,
                                  function(message)
                                  {
                                      if (message == "app version is up to date")
                                      {
                                          Helper.jsLog(message)
                                          return;
                                      }

                                      Helper.jsLog("got an update")
                                  },
                                  function(message)
                                  {
                                      Helper.jsLog("error on updating")
                                  }
                                  )
    }

    Timer
    {
        interval: 1000 * 60 // * 5
        repeat: true
        running: true
        onTriggered: checkForAppUpdate()
    }

    Connections
    {
        target: CPPQMLAppAndConfigurator
        function onBadMessage(message, itemToFocus, okCallback)
        {
            badMessage.show(message, itemToFocus)
        }
        function onGoodMessage(message, itemToFocus, okCallback)
        {
            goodMessage.show(message, itemToFocus, okCallback)
        }
        function onYesNoQuestion(question, itemToFocus, yesCallback, noCallback)
        {
            yesNoQuestion.show(question, itemToFocus, yesCallback, noCallback)
        }
        function onPickDateTime(dateTime, okCallback)
        {
            dateTimePicker.show(dateTime, okCallback)
        }
        function onPickDate(date, okCallback)
        {
            datePicker.show(date, okCallback)
        }
        function onPickTime(time, okCallback)
        {
            timePicker.show(time, okCallback)
        }
        function onNotYetImplemented()
        {
            badMessage.show(qsTr("Not yet implemented"), null)
        }
    }

    Connections
    {
        target: yacApp
        function onTakePhoto(squared, circled, target)
        {
            photoLoader.circled = circled
            photoLoader.squared = squared
            photoLoader.target = target
            photoLoader.sourceComponent = photoComponent
        }

    }

    Component.onCompleted:
    {
        if (!yacApp.secondStart)
        {
            startUpAni.start()
        }
        //        Helper.jsLog("hello")
        //        Helper.jsLog("ProjectID: " + yacApp.globalConfig.projectID)
        //        Helper.jsLog(yacApp.mainConfig)
        //        Helper.jsLog(yacApp.mainConfig.background)
        //        Helper.jsLog(yacApp.mainConfig.background.imageFilename)
        //        Helper.jsLog(yacApp.mainConfig.background.color)
        //        Helper.jsLog(yacApp.mainConfig.content.type)
        //        Helper.jsLog(yacApp.mainConfig.content.items.length)
        //        Helper.jsLog(yacApp.mainConfig.content.items[1].height)
        checkForAppUpdate()
        Constants.appTotalWidth = width
        Constants.designWidthFactor = width / 430.0
        Constants.appTotalHeight = height
        Constants.designHeightFactor = height / 932
    }
}
