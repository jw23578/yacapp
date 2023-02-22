import QtQuick 2.15
import QtQuick.Controls 2.15
import "qrc:/ModelFunctions.js" as ModelFunctions

Column
{
    width: parent.width
    property string initialText: ""
    property alias headerText: header.text
    property alias model: input.model
    property alias currentText: input.currentText
    property alias currentIndex: input.currentIndex
    property alias comboBox: input
    function find(needle)
    {
        return ModelFunctions.find(input.model, needle)
    }

    signal activated(string text)

    Text
    {
        id: header
        width: parent.width
    }
    ComboBox
    {
        id: input
        x: header.height
        width: parent.width - x
        currentIndex: initialText != "" ? find(initialText) : 0
        onActivated: parent.activated(textAt(index))
    }
}
