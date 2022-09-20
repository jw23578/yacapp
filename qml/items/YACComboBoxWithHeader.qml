import QtQuick 2.15
import QtQuick.Controls 2.15

Column
{
    width: parent.width
    property alias headerText: header.text
    property alias model: input.model
    property alias currentText: input.currentText
    property alias currentIndex: input.currentIndex
    function find(needle)
    {
        for (var i = 0; i < input.model.length; ++i)
        {
            console.log(input.model[i])
            if (input.model[i] == needle)
            {
                return i;
            }
        }
        return -1;
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
        x: height
        width: parent.width - x
        onActivated: parent.activated(textAt(index))
    }
}
