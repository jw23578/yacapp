import QtQuick 2.15
import "../items"
import "../menue"

Rectangle
{
    anchors.fill: parent
    property alias showCloseButton: theButtonRow.visible
    property alias buttonRow: theButtonRow
    signal closeClicked();
    signal leftClicked();
    property alias leftText: theButtonRow.leftText
    property alias multiMenueButton: theMultiMenueButton
    MouseArea
    {
        anchors.fill: parent
    }
    MultiMenueButton
    {
        id: theMultiMenueButton
        anchors.left: parent.left
        y: parent.height // - theSuperMenue.smallElemHeight
        visible: false
        z: 1
    }

    YACTwoButtonRow
    {
        z: 1
        id: theButtonRow
        visible: false
        rightText: qsTr("Close")
        onRightClicked: parent.closeClicked()
        onLeftClicked: parent.leftClicked()
    }
}
