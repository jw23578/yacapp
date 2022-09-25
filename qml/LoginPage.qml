import QtQuick 2.15
import "items"
import "qrc:/LoginFunctions.js" as LoginFunctions

Rectangle
{
    anchors.fill: parent
    Column
    {
        anchors.centerIn: parent
        width: parent.width * 3 / 4
        YACLineEditWithHeader
        {
            headerText: qsTr("Login")
            id: login
        }
        YACLineEditWithHeader
        {
            headerText: qsTr("Password")
            id: password
        }
        YACButton
        {
            text: qsTr("Login")
            onClicked: LoginFunctions.login(yacApp, "aidoo", "https://portal.aidoo-online.de/directLogin?mandant=test", login.text, password.text)
        }
    }
}
