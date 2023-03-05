import QtQuick 2.15

Item
{
    property alias leftText: leftButton.text
    property alias rightText: rightButton.text
    signal leftClicked()
    signal rightClicked()
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: leftButton.height
    YACButton
    {
        id: leftButton
        onClicked: parent.leftClicked()
        anchors.left: parent.left
        visible: text != ""
    }
    YACButton
    {
        id: rightButton
        onClicked: parent.rightClicked()
        anchors.right: parent.right
        visible: text != ""
    }
}
