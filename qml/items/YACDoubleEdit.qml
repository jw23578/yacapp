import QtQuick 2.15

Item
{
    id: theDoubleEdit
    height: theRow.height
    property int precision: 2
    property double value: 0
    property double step: 1
    property double minValue: 0
    property double maxValue: 1000
    Row
    {
        id: theRow
        YACButton
        {
            width: theDoubleEdit.width / 3
            text: "-"
            onClicked: theDoubleEdit.value = Math.max(theDoubleEdit.minValue, theDoubleEdit.value - theDoubleEdit.step)
        }
        YACText
        {
            width: theDoubleEdit.width / 3
            text: Number.parseFloat(theDoubleEdit.value).toFixed(theDoubleEdit.precision);
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        YACButton
        {
            width: theDoubleEdit.width / 3
            text: "+"
            onClicked: theDoubleEdit.value = Math.min(theDoubleEdit.maxValue, theDoubleEdit.value + theDoubleEdit.step)
        }
    }

}
