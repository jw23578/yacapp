import QtQuick 2.15
import "../items"

FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    signal closeClicked()
    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            YACLineEditWithHeader
            {
                headerText: qsTr("Name")
                text: yacApp.appUserConfig.fstname
            }
            YACLineEditWithHeader
            {
                headerText: qsTr("Surname")
                text: yacApp.appUserConfig.surname
            }
            YACLineEditWithHeader
            {
                headerText: qsTr("Visiblename")
                text: yacApp.appUserConfig.visibleName
            }
            YACButton
            {
                text: qsTr("Save")
                width: parent.width
            }
            YACButton
            {
                text: qsTr("Close")
                onClicked: closeClicked()
                width: parent.width
            }
            YACButton
            {
                text: qsTr("Logout")
                onClicked:
                {
                    yacApp.appUserLogout()
                    closeClicked()
                }
                width: parent.width
            }
        }
    }
}
