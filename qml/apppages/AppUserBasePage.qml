import QtQuick 2.15
import "../items"

Rectangle
{
    signal closeClicked();
    YACButton
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: qsTr("Close")
        onClicked: parent.closeClicked()
    }
}
