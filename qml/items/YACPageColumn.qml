import QtQuick 2.0

Column
{
    property bool centerVertical: true
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: centerVertical ? parent.verticalCenter : undefined
    width: parent.width * Constants.defaultWidthFactor
    spacing: parent.height / 25
}
