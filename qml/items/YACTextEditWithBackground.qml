import QtQuick 2.15

Rectangle
{
    height: theTextEdit.contentHeight + radius
    property alias preeditText: theTextEdit.preeditText
    property alias text: theTextEdit.text
    property alias wrapMode: theTextEdit.wrapMode
    property alias font: theTextEdit.font
    property alias textFocus: theTextEdit.focus
    border.color: "black"
    border.width: 1
    YACTextEdit
    {
        id: theTextEdit
        anchors.fill: parent
        anchors.leftMargin: parent.radius
        anchors.rightMargin: parent.radius
        anchors.topMargin: parent.radius / 2
        anchors.bottomMargin: parent.radius / 2
    }
}
