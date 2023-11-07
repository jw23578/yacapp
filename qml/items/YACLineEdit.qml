import QtQuick 2.15

FocusScope
{
    id: theScope
    property bool showColumn: true
    property alias echoMode: inputItem.echoMode
    property alias color: theRectangle.color
    property bool doubleEdit: false
    property bool intEdit: false
    property bool emailEdit: false
    property alias text: inputItem.text
    property alias input: inputItem
    property alias displayText: inputItem.displayText
    height: theRectangle.height
    width: parent.width
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
    Rectangle
    {
        id: theRectangle
        width: parent.width
        height: inputItem.height + 4
        border.color: "black"
        border.width: 1
        visible: theScope.showColumn
        YACTextInput
        {
            width: parent.width - 4
            x: 2
            y: 2
            id: inputItem
            validator: theScope.doubleEdit ? doubleValidator : theScope.intEdit ? intValidator : null
            clip: true
            focus: true
            inputMethodHints: theScope.intEdit ? Qt.ImhDigitsOnly :
                                                 theScope.emailEdit ? Qt.ImhEmailCharactersOnly : Qt.ImhNone
        }
    }
}
