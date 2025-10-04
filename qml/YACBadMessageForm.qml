import QtQuick 2.15
import "items"

Rectangle
{
    id: theBadMessageForm
    anchors.fill: parent
    layer.enabled: true
    opacity: 0
    visible: opacity > 0
    Behavior on opacity {
        NumberAnimation {
            duration: Constants.fastAnimationDuration
        }
    }
    color: Constants.backgroundPrimaryColor
    property var itemToFocus: null
    property var okCallback: null
    function show(message, itemToFocus, okCallback)
    {
        messageText.text = message
        theBadMessageForm.itemToFocus = itemToFocus
        theBadMessageForm.okCallback = okCallback
        opacity = 1
    }
    function hide()
    {
        opacity = 0
        if (itemToFocus != null)
        {
            itemToFocus.forceActiveFocus()
        }
        if (okCallback != null)
        {
            okCallback()
        }
    }

    YACRectangle
    {
        anchors.centerIn: parent
        color: Constants.badColor
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
        MouseArea
        {
            anchors.fill: parent
            onClicked: hide()
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
