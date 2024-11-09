import QtQuick 2.15

Item
{
    id: theIntEdit
    height: theRow.height
    property int value: 0
    property int step: 1
    property int minValue: 0
    property int maxValue: 1000
    property bool useInConfigurator: false
    Row
    {
        id: theRow
        height: plusButton.height
        YACButton
        {
            id: plusButton
            useInConfigurator: theIntEdit.useInConfigurator
            width: theIntEdit.width / 3
            text: "-"
            onClicked: theIntEdit.value = Math.max(theIntEdit.minValue, theIntEdit.value - theIntEdit.step)
        }
        YACText
        {
            useInConfigurator: theIntEdit.useInConfigurator
            height: parent.height
            width: theIntEdit.width / 3
            text: value
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        YACButton
        {
            useInConfigurator: theIntEdit.useInConfigurator
            width: theIntEdit.width / 3
            text: "+"
            onClicked: theIntEdit.value = Math.min(theIntEdit.maxValue, theIntEdit.value + theIntEdit.step)
        }
    }

}
