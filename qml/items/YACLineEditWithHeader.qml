import QtQuick 2.0

Column
{
    width: parent.width
    property alias headerText: header.text
    property alias text: input.text
    property alias displayText: input.displayText
    Text
    {
        id: header
        width: parent.width
    }
    TextInput
    {
        id: input
        x: height
        width: parent.width - x
    }
}
