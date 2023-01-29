import QtQuick 2.15
import QtGraphicalEffects 1.15

Item
{
    id: theYACRectangle
    property alias theInnerRectangle: anchorItem
    property alias color: theRectangle.color
    property alias radius: theRectangle.radius
    property alias border: theRectangle.border
    property bool radiusTopLeft: false
    property bool radiusTopRight: false
    property bool radiusBottomLeft: false
    property bool radiusBottomRight: false
    Item
    {
        id: anchorItem
        width: theRectangle.width
        height: theRectangle.height
    }

    Item
    {
        id: innerItem
        width: parent.width
        height: parent.height
        Rectangle
        {
            id: theRectangle
            width: parent.width
            height: parent.height
            border.width: 0
        }

        Rectangle
        {
            id: topLeft
            width: parent.width / 2
            height: parent.height / 2
            color: theRectangle.color
            anchors.left: theRectangle.left
            anchors.top: theRectangle.top
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
            anchors.right: theRectangle.right
            anchors.top: theRectangle.top
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
            anchors.left: theRectangle.left
            anchors.bottom: theRectangle.bottom
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
            anchors.right: theRectangle.right
            anchors.bottom: theRectangle.bottom
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
}
