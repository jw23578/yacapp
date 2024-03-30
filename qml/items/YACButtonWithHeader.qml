import QtQuick 2.0

FocusScope
{
    id: theScope
    property bool showColumn: true

    property alias headerText: header.text
    property alias text: button.text
    property alias emptyText: button.emptyText
    height: buttonEditWithHeader.height
    property alias buttonHeight: button.height
    width: parent.width
    signal clicked()
    Column
    {
        id: buttonEditWithHeader

        Behavior on height {
            NumberAnimation {
                duration: 200
            }
        }
        width: parent.width
        YACText
        {
            id: header
            x: button.captionStartX
            width: parent.width
            visible: theScope.showColumn
        }
        Item
        {
            width: parent.width - x
            height: button.height
            visible: theScope.showColumn
            YACButton
            {
                id: button
                width: parent.width
                clip: true
                focus: true
                onClicked: theScope.clicked()
            }
        }
    }
}
