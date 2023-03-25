import QtQuick 2.15
import "qrc:/qml/items"

Item
{
//    color: "transparent"
//    border.width: 1
//    border.color: "black"
    id: editorSectionItem
    width: parent.width
    property alias sectionCaption: theSectionCaption.text
    clip: true
    height: theBackground.height + Constants.shadowVerticalOffset
    default property alias column: sectionColumn.data
    Behavior on height
    {
        NumberAnimation {
            duration: 200
        }
    }
    YACRectangle
    {
        id: theBackground
        width: sectionColumn.width + sectionColumn.x * 2
        height: sectionColumn.height + sectionColumn.y * 2
        color: "lightgrey"
        radius: Constants.radius
        radiusTopLeft: true
        radiusBottomRight: true
    }
    Column
    {
        id: sectionColumn
        y: Constants.radius
        x: Constants.radius
        width: parent.width - x * 2 - Constants.shadowHorizontalOffset
        property bool collapsed: false
        function toggle()
        {
            if (collapsed)
            {
                expand()
            }
            else
            {
                collapse()
            }
        }

        function collapse()
        {
            for (var i = 1; i < children.length; ++i)
            {
                children[i].visible = false
            }
            collapsed = true
        }
        function expand()
        {
            for (var i = 1; i < children.length; ++i)
            {
                children[i].visible = true
            }
            collapsed = false
        }

        Item
        {
            height: theSectionCaption.height
            width: parent.width
            Text
            {
                font.bold: true
                id: theSectionCaption
                width: parent.width
            }
            Text
            {
                font.bold: true
                anchors.right: parent.right
                anchors.rightMargin: width / 2
                text: ">"
                rotation: sectionColumn.collapsed ? 90 : -90
                Behavior on rotation {
                    NumberAnimation {
                        duration: 200
                    }
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    sectionColumn.toggle()
                }
            }
        }
        Component.onCompleted:
        {
            collapse()
        }
    }
}
