import QtQuick 2.15
import QtGraphicalEffects 1.15

Item
{
    id: theYACRectangle
    property alias color: theRectangle.color
    property alias radius: theRectangle.radius
    property alias border: theRectangle.border
    property alias shadow: theDropShadow
    property bool radiusTopLeft: false
    property bool radiusTopRight: false
    property bool radiusBottomLeft: false
    property bool radiusBottomRight: false
    Item
    {
        id: innerItem
        width: parent.width
        height: parent.height
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
            visible: !theYACRectangle.radiusTopLeft
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
            visible: !theYACRectangle.radiusTopRight
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
            visible: !theYACRectangle.radiusBottomLeft
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
            visible: !theYACRectangle.radiusBottomRight
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
    YACDropShadow
    {
        id: theDropShadow
        anchors.fill: innerItem
        source: innerItem
    }
}
