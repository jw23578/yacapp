import QtQuick 2.15
import "../items"

Rectangle
{
    anchors.fill: parent
    property alias buttonRow: theButtonRow
    signal closeClicked();
    signal leftClicked();
    property alias leftText: theButtonRow.leftText
    MouseArea
    {
        anchors.fill: parent
    }
    YACTwoButtonRow
    {
        id: theButtonRow
        rightText: qsTr("Close")
        onRightClicked:
        {
            closeClicked()
        }
        onLeftClicked: parent.leftClicked()
    }
}
