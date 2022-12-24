import QtQuick 2.15
import "../items"

Rectangle
{
    property string profileId: ""
    color: "red"
    anchors.fill: parent
    signal closeClicked()
    id: messagePage

    Component
    {
        id: messageDelegate
        Rectangle
        {
            property var message: null
            property bool daySwitch: false
            property bool other: message.senderId != ""
            rotation: 180
            color: "lightgrey"
            width: contentText.contentWidth
            height: contentText.contentHeight * 3
            x: other ? messagePage.width - width : 0
            Text
            {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                visible: daySwitch
                text: Helper.formatDate(message.sent)
            }

            Text
            {
                id: contentText
                anchors.centerIn: parent
                text: message.content
            }
            Text
            {
                anchors.bottom: parent.bottom
                text: Helper.formatTime(message.sent)
            }
        }
    }


    ListView
    {
        rotation: 180
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: theTextEdit.top
        reuseItems: true
        model: MessagesModel
        spacing: 1
        delegate: Loader
        {
            sourceComponent: messageDelegate // message.senderID = "" ? myMessageDelegate : otherMessageDelegate
            onLoaded:
            {
                item.daySwitch = daySwitch
                item.message = message
            }
        }
    }
    YACButton
    {
        id: closeButton
        anchors.top: parent.top
        text: qsTr("Close")
        onClicked: closeClicked()
    }
    YACTextEdit
    {
        id: theTextEdit
        anchors.left: parent.left
        anchors.right: sendButton.left
        anchors.bottom: parent.bottom
        wrapMode: Text.WordWrap
    }
    YACButton
    {
        id: sendButton
        text: ">>"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked:
        {
            if (theTextEdit.text == "")
            {
                return
            }
            yacApp.sendMessage(profileId, theTextEdit.text)
            theTextEdit.text = ""
        }
    }
}
