import QtQuick 2.0

YACText
{
    signal clicked()
    MouseArea
    {
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
