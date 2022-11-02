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

    NumberAnimation
    {
        id: closeAnimation
        target: dialog
        duration: 400
        property: "opacity"
        to: 0
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

    YACTwoButtonRow
    {
        leftText: qsTr("OK")
        rightText: qsTr("Abort")
        onRightClicked:
        {
            close()
            abortClicked()
        }
        onLeftClicked:
        {
            okClicked()
        }
    }
}
