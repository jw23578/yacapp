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
    property alias emptyText: inputItem.emptyText
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
        property int theBorder: inputItem.height / 3
        width: parent.width + 2 * theBorder
        height: inputItem.height + 2 * theBorder
        border.color: Constants.dark ? Qt.lighter(color) : Qt.darker(color)
        border.width: 1
        radius: width == height ? height / 2 : height / 4
        visible: theScope.showColumn
        color: Constants.lineEditBackgroundColor
        YACTextInput
        {
            width: parent.width - 2 * parent.theBorder
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            id: inputItem
            validator: theScope.doubleEdit ? doubleValidator : theScope.intEdit ? intValidator : null
            clip: true
            focus: true
            inputMethodHints: theScope.intEdit ? Qt.ImhDigitsOnly :
                                                 theScope.emailEdit ? Qt.ImhEmailCharactersOnly : Qt.ImhNone
        }
    }
}
