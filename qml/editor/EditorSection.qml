import QtQuick 2.0

Column
{
    width: parent.width
    id: sectionColumn
    property alias sectionCaption: theSectionCaption.text
    clip: true
    Behavior on height {
        NumberAnimation {
            duration: 200
        }
    }

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
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    sectionColumn.toggle()
                }
            }
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
    }
    Component.onCompleted:
    {
        collapse()
    }
}
