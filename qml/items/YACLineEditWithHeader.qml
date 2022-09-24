import QtQuick 2.0

Column
{
    DoubleValidator
    {
        id: doubleValidator
    }
    IntValidator
    {
        id: intValidator
    }

    Behavior on height {
        NumberAnimation {
            duration: 200
        }
    }

    property bool showColumn: true

    property bool doubleEdit: false
    property bool intEdit: false
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
        validator: parent.doubleEdit ? doubleValidator : parent.intEdit ? intValidator : null
        visible: parent.showColumn
    }
}
