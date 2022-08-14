import QtQuick 2.0

Rectangle
{
    property var footerConfig: null
    height: parent.height * footerConfig.height
    anchors.left: parent.left
    anchors.right: parent.right
    color: footerConfig.color
}
