import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

import "qml"
import "qml/items"

ApplicationWindow
{
    visible: true
    width: 480
    height: 640
    title: qsTr("Hello World")

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
        onVisibleChanged: Constants.topLevelFormActive = badMessage.visible || goodMessage.visible || yesNoQuestion.visible || splashscreen.visible
    }

    YACBadMessageForm
    {
        id: badMessage
        onVisibleChanged: Constants.topLevelFormActive = badMessage.visible || goodMessage.visible || yesNoQuestion.visible || splashscreen.visible
    }
    YACGoodMessageForm
    {
        id: goodMessage
        onVisibleChanged: Constants.topLevelFormActive = badMessage.visible || goodMessage.visible || yesNoQuestion.visible || splashscreen.visible
    }
    YACYesNoQuestionForm
    {
        id: yesNoQuestion
        onVisibleChanged: Constants.topLevelFormActive = badMessage.visible || goodMessage.visible || yesNoQuestion.visible || splashscreen.visible
    }

    function checkForAppUpdate()
    {
        if (!yacApp.globalConfig.projectID.length)
        {
            console.log("no current app active -> no update check")
            return;
        }

        console.log("checking for updates");
        yacApp.yacappServerGetAPP(yacApp.globalConfig.projectID,
                                  yacApp.globalConfig.version,
                                  function(message)
                                  {
                                      if (message == "app version is up to date")
                                      {
                                          console.log(message)
                                          return;
                                      }

                                      console.log("got an update")
                                  },
                                  function(message)
                                  {
                                      console.log("error on updating")
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
        target: yacApp
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
    }

    Component.onCompleted:
    {
        startUpAni.start()
//        console.log("hello")
//        console.log("ProjectID: " + yacApp.globalConfig.projectID)
//        console.log(yacApp.mainConfig)
//        console.log(yacApp.mainConfig.background)
//        console.log(yacApp.mainConfig.background.imageFilename)
//        console.log(yacApp.mainConfig.background.color)
//        console.log(yacApp.mainConfig.content.type)
//        console.log(yacApp.mainConfig.content.items.length)
//        console.log(yacApp.mainConfig.content.items[1].height)
        checkForAppUpdate()
    }
}
