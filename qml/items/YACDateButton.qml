import QtQuick 2.15

YACButtonWithHeader
{
    property date theDate: new Date()
    signal dateChanged;
    text: Helper.formatDateShort(theDate)
    onClicked: CPPQMLAppAndConfigurator.pickDate(theDate,
                                                 function(newDate){theDate = newDate;
                                                 dateChanged()})
}
