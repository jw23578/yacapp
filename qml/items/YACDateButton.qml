import QtQuick 2.15

YACButtonWithHeader
{
    property date theDate: new Date()
    text: Helper.formatDateShort(theDate)
    onClicked: CPPQMLAppAndConfigurator.pickDate(theDate,
                                                 function(newDate){theDate = newDate})
}
