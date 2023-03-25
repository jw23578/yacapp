import QtQuick 2.0
import "items"

Rectangle
{
    height: parent.height * headerConfig.heightPerThousand / 1000.0
    color: headerConfig.color
    property var headerConfig: null
    YACText
    {
        id: headerText
        anchors.centerIn: parent
        text: header.headerConfig.title
    }
}
