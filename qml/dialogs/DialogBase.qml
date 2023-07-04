import QtQuick 2.0
import QtQuick.Dialogs 1.3
import "qrc:/qml/items"

Rectangle
{
    id: dialog
    width: parent.width * 4 / 5
    height: parent.height * 4 / 5
    color: "white"
    anchors.centerIn: parent
    opacity: 0
    visible: opacity > 0
    signal okClicked()
    signal abortClicked()
    property int animationDuration: 200
    focus: true

    ParallelAnimation
    {
        id: openAnimation
        NumberAnimation
        {
            target: dialog
            duration: dialog.animationDuration
            property: "opacity"
            to: 1
        }
        NumberAnimation
        {
            target: dialog
            duration: dialog.animationDuration
            property: "anchors.horizontalCenterOffset"
            to: 0
        }

    }


    function open()
    {
        anchors.horizontalCenterOffset = width
        openAnimation.start()
    }

    ParallelAnimation
    {
        id: closeAnimation
        NumberAnimation
        {
            target: dialog
            duration: dialog.animationDuration
            property: "opacity"
            to: 0
        }
        NumberAnimation
        {
            target: dialog
            duration: dialog.animationDuration
            property: "anchors.horizontalCenterOffset"
            to: -width
        }
    }

    function close()
    {
        closeAnimation.start()
    }

    Rectangle
    {
        z: -1
        anchors.centerIn: parent
        width: parent.parent.width
        height: parent.parent.height
        color: "black"
        opacity: 0.5
        MouseArea
        {
            anchors.fill: parent
        }
    }

    Keys.onEscapePressed:
    {
        close()
        abortClicked()
    }

    YACTwoButtonRow
    {
        rightText: qsTr("OK")
        leftText: qsTr("Abort")
        onLeftClicked:
        {
            close()
            abortClicked()
        }
        onRightClicked:
        {
            okClicked()
        }
    }
}
