import QtQuick 2.0

Rectangle
{
    height: theTextEdit.contentHeight
    property alias text: theTextEdit.text
    property alias wrapMode: theTextEdit.wrapMode
    YACTextEdit
    {
        id: theTextEdit
        anchors.fill: parent
    }
}
