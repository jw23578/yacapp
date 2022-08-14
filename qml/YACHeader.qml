import QtQuick 2.0

Rectangle
{
    height: parent.height * headerConfig.height
    color: headerConfig.color
    anchors.left: parent.left
    anchors.right: parent.right
    property var headerConfig: null
    Text
    {
        id: headerText
        anchors.centerIn: parent
        text: header.headerConfig.title
    }
}
