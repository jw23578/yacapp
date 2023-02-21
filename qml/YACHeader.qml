import QtQuick 2.0

Rectangle
{
    height: parent.height * headerConfig.heightPerThousand / 1000.0
    color: headerConfig.color
    property var headerConfig: null
    Text
    {
        id: headerText
        anchors.centerIn: parent
        text: header.headerConfig.title
    }
}
