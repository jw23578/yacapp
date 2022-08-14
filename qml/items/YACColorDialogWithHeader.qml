import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3

Column
{
    width: parent.width
    property alias headerText: header.text
    property alias color: input.color
    function find(needle)
    {
        return input.find(needle)
    }
    ColorDialog
    {
        id: colorDialog
        onAccepted: input.color = colorDialog.color
    }

    Text
    {
        id: header
        width: parent.width
    }
    Rectangle
    {
        id: input
        width: parent.width - x
        height: header.height
        MouseArea
        {
            anchors.fill: parent
            onClicked: colorDialog.open()
        }
    }
}
