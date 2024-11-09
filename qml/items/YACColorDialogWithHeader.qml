import QtQuick 2.6
import QtQuick.Controls 2.6
import Qt.labs.platform

Column
{
    id: column
    width: parent.width
    property bool useInConfigurator: false
    property alias headerText: header.text
    property alias color: input.color
    signal accepted(color color)
    function find(needle)
    {
        return input.find(needle)
    }
    ColorDialog
    {
        id: colorDialog
        currentColor: column.color
        onAccepted:
        {
            column.color = color
            column.accepted(color)
        }
    }

    YACText
    {
        useInConfigurator: column.useInConfigurator
        id: header
        width: parent.width
    }
    Rectangle
    {
        id: input
        width: parent.width - x
        height: header.height * 2
        MouseArea
        {
            anchors.fill: parent
            onClicked: colorDialog.open()
        }
    }
}
