import QtQuick 2.0

Rectangle
{
    height: parent.height * header.headerConfig.height
    color: "green"
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
