import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

Column
{
    id: column
    width: parent.width
    property alias headerText: header.text
    property alias color: colorDialog.color
    function find(needle)
    {
        return input.find(needle)
    }
    ColorDialog
    {
        id: colorDialog
    }

    Text
    {
        id: header
        width: parent.width
    }
    Rectangle
    {
        id: input
        color: colorDialog.color
        width: parent.width - x
        height: header.height
        MouseArea
        {
            anchors.fill: parent
            onClicked: colorDialog.open()
        }
    }
}
