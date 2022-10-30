import QtQuick 2.0

FocusScope
{
    id: theScope
    property bool showColumn: true
    property alias echoMode: input.echoMode

    property bool doubleEdit: false
    property bool intEdit: false
    property alias headerText: header.text
    property alias text: input.text
    property alias displayText: input.displayText
    height: lineEditWithHeader.height
    width: parent.width
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

        width: parent.width
        YACText
        {
            id: header
            width: parent.width
            visible: theScope.showColumn
        }
        Rectangle
        {
            x: header.height
            width: parent.width - x
            height: input.height + 4
            border.color: "black"
            border.width: 1
            visible: theScope.showColumn
            YACTextInput
            {
                width: parent.width - 4
                x: 2
                y: 2
                id: input
                validator: theScope.doubleEdit ? doubleValidator : theScope.intEdit ? intValidator : null
                clip: true
                focus: true
            }
        }
    }
}
