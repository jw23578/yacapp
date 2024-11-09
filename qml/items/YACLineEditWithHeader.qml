import QtQuick 2.15

FocusScope
{
    id: theScope
    function intValue()
    {
        if (text == "")
        {
            return 0
        }
        return parseInt(text)
    }

    property double doubleValue: 0

    onDisplayTextChanged:
    {
        if (doubleEdit)
        {
            doubleValue = parseFloat(displayText) || 0
        }
    }

    property bool showColumn: true
    property alias echoMode: inputItem.echoMode
    property alias color: theRectangle.color
    property bool doubleEdit: false
    property bool intEdit: false
    property alias headerText: header.text
    property alias text: inputItem.text
    property alias input: inputItem
    property alias displayText: inputItem.displayText
    property bool useInConfigurator: false
    height: lineEditWithHeader.height
    width: parent.width
    Column
    {
        id: lineEditWithHeader
        DoubleValidator
        {
            id: doubleValidator
            decimals: 3
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
            useInConfigurator: theScope.useInConfigurator
        }
        Rectangle
        {
            id: theRectangle
            x: header.height
            width: parent.width - x
            height: inputItem.height + 4
            border.color: "black"
            border.width: 1
            visible: theScope.showColumn
            YACTextInput
            {
                useInConfigurator: theScope.useInConfigurator
                width: parent.width - 4
                x: 2
                y: 2
                id: inputItem
                validator: theScope.doubleEdit ? doubleValidator : theScope.intEdit ? intValidator : null
                clip: true
                focus: true
            }
        }
    }
}
