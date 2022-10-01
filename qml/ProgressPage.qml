import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle
{
    visible: false
    function show()
    {
        visible = true
    }
    function tick(to1, pos1, to2, pos2)
    {
        pb1.to = to1
        pb1.position = pos1
        pb2.to = to2
        pb2.position = pos2
    }

    anchors.fill: parent
    Column
    {
        width: parent.width * 3 / 4
        anchors.centerIn: parent
        ProgressBar
        {
            id: pb1
            width: parent.width
        }
        ProgressBar
        {
            id: pb2
            width: parent.width
            visible: to > 0
        }
    }
}
