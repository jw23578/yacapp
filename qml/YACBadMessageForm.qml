import QtQuick 2.15
import "items"

Rectangle
{
    anchors.fill: parent
    visible: false
    function show(message)
    {
        messageText.text = message
        visible = true
    }
    function hide()
    {
        visible = false
    }

    YACText
    {
        id: messageText
        anchors.centerIn: parent
        width: parent.width * 3 / 4
        wrapMode: Text.WordWrap
    }

    YACButton
    {
        text: qsTr("Ok")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: height
        onClicked: hide()
    }
}
