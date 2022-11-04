import QtQuick 2.15

Item
{
    property alias color: theRectangle.color    
    property alias radius: theRectangle.radius
    property alias border: theRectangle.border
    property bool radiusTopLeft: false
    property bool radiusTopRight: false
    property bool radiusBottomLeft: false
    property bool radiusBottomRight: false
    Rectangle
    {
        id: theRectangle
        anchors.fill: parent
        border.width: 0
    }

    Rectangle
    {
        id: topLeft
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        anchors.left: parent.left
        anchors.top: parent.top
        visible: !parent.radiusTopLeft
        Rectangle
        {
            width: parent.width
            height: theRectangle.border.width
            color: theRectangle.border.color
        }
        Rectangle
        {
            width: theRectangle.border.width
            height: parent.height
            color: theRectangle.border.color
        }
    }
    Rectangle
    {
        id: topRight
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        anchors.right: parent.right
        anchors.top: parent.top
        visible: !parent.radiusTopRight
        Rectangle
        {
            width: parent.width
            height: theRectangle.border.width
            color: theRectangle.border.color
        }
        Rectangle
        {
            width: theRectangle.border.width
            height: parent.height
            color: theRectangle.border.color
            anchors.right: parent.right
        }
    }
    Rectangle
    {
        id: bottomLeft
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        visible: !parent.radiusBottomLeft
        Rectangle
        {
            width: parent.width
            height: theRectangle.border.width
            color: theRectangle.border.color
            anchors.bottom: parent.bottom
        }
        Rectangle
        {
            width: theRectangle.border.width
            height: parent.height
            color: theRectangle.border.color
        }
    }
    Rectangle
    {
        id: bottomRight
        width: parent.width / 2
        height: parent.height / 2
        color: theRectangle.color
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        visible: !parent.radiusBottomRight
        Rectangle
        {
            width: parent.width
            height: theRectangle.border.width
            color: theRectangle.border.color
            anchors.bottom: parent.bottom
        }
        Rectangle
        {
            width: theRectangle.border.width
            height: parent.height
            color: theRectangle.border.color
            anchors.right: parent.right
        }
    }



}
