import QtQuick 2.15
import "qrc:/MenueFunctions.js" as MenueFunctions
import com.yacapp.menueconfig 1.0
import "items"

Item
{
    id: defaultMenue
    anchors.fill: parent
    property MenueConfig theMenue: null
    property var stackView: null
    property bool once: true
    property bool isClosed: true
    property bool menueOpen: !isClosed
    function toggle()
    {
        if (isClosed)
        {
            open()
        }
        else
        {
            close()
        }
    }
    YACButton
    {
        id: sizeButton
        visible: false
        text: "+"
    }

    function open()
    {
        Helper.jsLog("hello2")
        isClosed = false
        if (once)
        {
            once = false
            buttonColumn.anchors.centerIn = null
            openXAni.to = buttonColumn.x
            openYAni.to = buttonColumn.y
            openWidthAni.to = buttonColumn.width
            openHeightAni.to = buttonColumn.height
            buttonColumn.width = sizeButton.width
            buttonColumn.height = sizeButton.height
            buttonColumn.x = -sizeButton.width
            buttonColumn.y = parent.height
        }
        buttonColumn.visible = true
        openAni.start()
    }

    function close()
    {
        isClosed = true
        closeAni.start()
    }

    ParallelAnimation
    {
        id: openAni

        NumberAnimation
        {
            id: openXAni
            target: buttonColumn
            property: "x"
            duration: 200
            easing.type: Easing.InOutQuad
        }
        NumberAnimation
        {
            id: openYAni
            target: buttonColumn
            property: "y"
            duration: 200
            easing.type: Easing.InOutQuad
        }
        NumberAnimation
        {
            id: openWidthAni
            target: buttonColumn
            property: "width"
            duration: 200
            easing.type: Easing.InOutQuad
        }
        NumberAnimation
        {
            id: openHeightAni
            target: buttonColumn
            property: "height"
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }

    ParallelAnimation
    {
        id: closeAni

        NumberAnimation
        {
            id: closeXAni
            target: buttonColumn
            property: "x"
            duration: 200
            easing.type: Easing.InOutQuad
            to: -sizeButton.width
        }
        NumberAnimation
        {
            id: closeYAni
            target: buttonColumn
            property: "y"
            duration: 200
            easing.type: Easing.InOutQuad
            to: parent.height
        }
        NumberAnimation
        {
            id: closeWidthAni
            target: buttonColumn
            property: "width"
            duration: 200
            easing.type: Easing.InOutQuad
            to: sizeButton.width
        }
        NumberAnimation
        {
            id: closeHeightAni
            target: buttonColumn
            property: "height"
            duration: 200
            easing.type: Easing.InOutQuad
            to: sizeButton.height
        }
    }

    Column
    {
        visible: false
        id: buttonColumn
        anchors.centerIn: parent
        width: parent.width * 3 / 4

        Repeater
        {
            model: theMenue.itemCount
            YACButton
            {
                width: parent.width
                text: theMenue.items[index].caption
                onClicked:
                {
                    defaultMenue.close()
                    MenueFunctions.openTarget(yacApp, stackView, theMenue.items[index].target, defaultMenue)
                }
            }
        }
    }
}
