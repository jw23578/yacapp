import QtQuick 2.15
import "items"

Rectangle
{
    id: deployPage
    anchors.fill: parent
    Column
    {
        anchors.centerIn: parent
        spacing: 1
        width: parent.width * 3 / 4
        YACLineEditWithHeader
        {
            headerText: qsTr("Host")
            width: parent.width
            id: host
        }
        YACLineEditWithHeader
        {
            headerText: qsTr("User")
            width: parent.width
            id: user
        }
        YACLineEditWithHeader
        {
            headerText: qsTr("Password")
            width: parent.width
            id: password
            echoMode: TextInput.Password
        }
        YACLineEditWithHeader
        {
            headerText: qsTr("www-Basedirectory")
            width: parent.width
            id: basedirectory
        }
        YACButton
        {
            text: qsTr("Upload")
            width: parent.width
            onClicked:
            {
                configurator.deploy(yacApp.globalConfig.projectID, host.displayText, user.displayText, password.displayText, basedirectory.displayText)
            }
        }
        YACButton
        {
            text: qsTr("Abbrechen")
            onClicked: deployPage.visible = false
            width: parent.width
        }
    }
    ProgressPage
    {
        id: pp
    }
}
