import QtQuick 2.15

YACButtonWithHeader
{
    property date theDate: new Date()
    text: Helper.formatDate(theDate)
    onClicked: yacApp.pickDate(theDate,
                               function(newDate){theDate = newDate})
}
