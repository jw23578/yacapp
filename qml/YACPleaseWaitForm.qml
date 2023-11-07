import QtQuick 2.15
import "items"

Rectangle
{
    anchors.fill: parent
    visible: false
    id: theWaitForm
    property var itemToFocus: null
    property var okCallback: null
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
        color: Constants.waitColor
        radius: width / 10
        radiusTopLeft: true
        radiusBottomRight: radiusTopLeft
        width: parent.width * 3 / 4
        height: parent.height * 3 / 5
        YACText
        {
            id: messageText
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
