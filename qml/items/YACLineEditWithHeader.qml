import QtQuick 2.0

Column
{
    DoubleValidator
    {
        id: doubleValidator
    }
    Behavior on height {
        NumberAnimation {
            duration: 200
        }
    }

    property bool showColumn: true

    property bool doubleEdit: false
    width: parent.width
    property alias headerText: header.text
    property alias text: input.text
    property alias displayText: input.displayText
    Text
    {
        id: header
        width: parent.width
        visible: parent.showColumn
    }
    TextInput
    {
        id: input
        x: height
        width: parent.width - x
        validator: parent.doubleEdit ? doubleValidator : null
        visible: parent.showColumn
    }
}
