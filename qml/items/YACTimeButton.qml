import QtQuick 2.15

YACButtonWithHeader
{
    property date theTime: new Date()
    text: Helper.formatTime(theTime)
    onClicked: CPPQMLAppAndConfigurator.pickTime(theTime,
                                                 function(newTime){theTime = newTime})
}
