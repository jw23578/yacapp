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
                anchors.centerIn: parent
                text: message.content
            }
        }
    }
}
