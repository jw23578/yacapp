import QtQuick 2.0

FocusScope
{
    id: theScope
    property bool showColumn: true

    property alias headerText: header.text
    property alias text: button.text
    property alias emptyText: button.emptyText
    height: buttonEditWithHeader.height
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
        Rectangle
        {
            width: parent.width - x
            height: button.height
//            border.color: "black"
//            border.width: 1
            visible: theScope.showColumn
            YACButton
            {
                id: button
                width: parent.width - 4
                x: 2
                y: 2
                clip: true
                focus: true
                onClicked: theScope.clicked()
            }
        }
    }
}
