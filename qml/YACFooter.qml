import QtQuick 2.0

Rectangle
{
    property var footerConfig: null
    height: parent.height * footerConfig.height
    width: parent.width
    color: footerConfig.color
}
