import QtQuick 2.15

Item
{
    property alias color: theRectangle.color
    property int radiusTopLeft: 0
    property int radiusTopRight: 0
    property int radiusBottomLeft: 0
    property int radiusBottomRight: 0
    Rectangle
    {
        id: topLeft
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        radius: parent.radiusTopLeft
        anchors.left: parent.left
        anchors.top: parent.top
    }
    Rectangle
    {
        id: topRight
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        radius: parent.radiusTopRight
        anchors.right: parent.right
        anchors.top: parent.top
    }
    Rectangle
    {
        id: bottomLeft
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        radius: parent.radiusBottomLeft
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }
    Rectangle
    {
        id: bottomRight
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        radius: parent.radiusBottomRight
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
    Rectangle
    {
        id: theRectangle
        anchors.fill: parent
        radius: Math.max(parent.radiusTopLeft, parent.radiusTopRight, parent.radiusBottomLeft, parent.radiusBottomRight)
    }
}
