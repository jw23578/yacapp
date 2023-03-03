import QtQuick 2.15
import "../items"

Rectangle
{
    anchors.fill: parent
    property alias closeButton: theCloseButton
    signal closeClicked();
    YACButton
    {
        id: theCloseButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: qsTr("Close")
        onClicked: parent.closeClicked()
    }    
}
