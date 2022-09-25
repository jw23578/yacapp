import QtQuick 2.0

Column
{
    id: lineEditWithHeader
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
    YACText
    {
        id: header
        width: parent.width
        visible: parent.showColumn
    }
    Rectangle
    {
        x: header.height
        width: parent.width - x
        height: input.height + 4
        border.color: "black"
        border.width: 1
        visible: lineEditWithHeader.showColumn
        YACTextInput
        {
            width: parent.width - 4
            x: 2
            y: 2
            id: input
            validator: lineEditWithHeader.doubleEdit ? doubleValidator : lineEditWithHeader.intEdit ? intValidator : null
        }
    }
}
