import QtQuick 2.15

Item
{
    signal pleaseUpdate
    property int updateTrigger: yacApp.updateTrigger
    onUpdateTriggerChanged: pleaseUpdate()
}
