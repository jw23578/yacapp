import QtQuick 2.0

Rectangle
{
    property var footerConfig: null
    height: parent.height * footerConfig.height
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    color: footerConfig.color
}
