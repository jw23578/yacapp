import QtQuick 2.15
import "qrc:/qml/items"

Item
{
//    color: "transparent"
//    border.width: 1
//    border.color: "black"
    id: editorSectionItem
    property bool useInConfigurator: false
    width: parent.width
    property alias sectionCaption: theSectionCaption.text
    clip: true
    height: theBackground.height + Constants.shadowVerticalOffset
    default property alias column: sectionColumn.data
    property color closedColor: Constants.buttonPrimaryColor
    property color openedColor: "lightgrey"
    property bool behaviorEnabled: false

    YACRectangle
    {
        id: theBackground
        width: sectionColumn.width + sectionColumn.x * 2
        height: sectionColumn.height + (sectionColumn.collapsed ? 0 : Constants.radius)
        color: theSectionCaption.closedColor
        radius: Constants.radius
        radiusTopLeft: true
        radiusBottomRight: true
        Behavior on color {
            enabled: editorSectionItem.behaviorEnabled
            ColorAnimation {
                duration: 200
            }
        }
        Behavior on height
        {
            enabled: editorSectionItem.behaviorEnabled
            NumberAnimation {
                duration: 200
            }
        }
    }
    Column
    {
        id: sectionColumn
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
            theBackground.color = closedColor
            for (var i = 1; i < children.length; ++i)
            {
                children[i].visible = false
            }
            collapsed = true
        }
        function expand()
        {
            behaviorEnabled = true
            theBackground.color = openedColor
            for (var i = 1; i < children.length; ++i)
            {
                children[i].visible = true
            }
            collapsed = false
        }

        Item
        {
            height: theSectionCaption.height + 2 * Constants.radius

            width: parent.width
            YACText
            {
                y: Constants.radius
                useInConfigurator: theSectionCaption.useInConfigurator
                font.bold: true
                id: theSectionCaption
                width: parent.width
            }
            YACText
            {
                y: Constants.radius
                useInConfigurator: theSectionCaption.useInConfigurator
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
