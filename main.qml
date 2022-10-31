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
        active: yacApp.mainConfig != null
        source: "qml/MainForm.qml"
    }

    SelectAppForm
    {
        id: selectApp
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
        visible: opacity > 0
        id: splashscreen
        anchors.fill: parent
        color: "silver"
        YACText
        {
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            text: "This is the yacApp"
            font.bold: true
            font.pixelSize: width / 10
            horizontalAlignment: Text.AlignHCenter
        }
    }

    YACBadMessageForm
    {
        id: badMessage
    }
    YACGoodMessageForm
    {
        id: goodMessage
    }
    YACYesNoQuestionForm
    {
        id: yesNoQuestion
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
        function onBadMessage(message, itemToFocus)
        {
            badMessage.show(message, itemToFocus)
        }
        function onGoodMessage(message, itemToFocus)
        {
            goodMessage.show(message, itemToFocus)
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
