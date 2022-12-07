import QtQuick 2.15
import "../items"

FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    signal closeClicked()
    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            YACLineEditWithHeader
            {
                headerText: qsTr("Name")
            }
            YACLineEditWithHeader
            {
                headerText: qsTr("Surname")
            }
            YACButton
            {
                text: qsTr("Save")
                width: parent.width
            }
            YACButton
            {
                text: qsTr("Close")
                onClicked: closeClicked()
                width: parent.width
            }
        }
    }
}
