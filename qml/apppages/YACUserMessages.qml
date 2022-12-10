import QtQuick 2.15

Rectangle
{
    color: "red"
    anchors.fill: parent
    ListView
    {
        rotation: 180
        anchors.fill: parent
        reuseItems: true
        model: MessagesModel
        spacing: 1
        delegate:
            Rectangle
        {
            rotation: 180
            color: "lightgrey"
            width: parent.width
            height: 200
            Text
            {
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                visible: daySwitch
                text: Helper.formatDate(message.sent)
            }

            Text
            {
                anchors.centerIn: parent
                text: message.content
            }
            Text
            {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: Helper.formatTime(message.sent)
            }
        }
    }
}