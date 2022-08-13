import QtQuick 2.0

Column
{
    width: parent.width
    id: sectionColumn
    property alias sectionCaption: theSectionCaption.text
    property int fullHeight: 0
    clip: true
    Behavior on height {
        NumberAnimation {
            duration: 200
        }
    }

    Text
    {
        font.bold: true
        id: theSectionCaption
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                if (sectionColumn.fullHeight == 0)
                {
                    sectionColumn.fullHeight = sectionColumn.height
                }
                if (sectionColumn.height == sectionColumn.fullHeight)
                {
                    sectionColumn.height = theSectionCaption.height
                }
                else
                {
                    sectionColumn.height = sectionColumn.fullHeight
                }
            }
        }
    }

}
