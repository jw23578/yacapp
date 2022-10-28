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

    YACRectangle
    {
        anchors.centerIn: parent
        color: "#F09494"
        radius: width / 10
        radiusTopLeft: true
        radiusBottomRight: radiusTopLeft
        width: parent.width * 3 / 4
        height: parent.height * 3 / 4
        YACText
        {
            id: messageText
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
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