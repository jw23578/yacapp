import QtQuick 2.15
import "items"
import "qrc:/LoginFunctions.js" as LoginFunctions

FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    Rectangle
    {
        anchors.fill: parent
        MouseArea
        {
            anchors.fill: parent
        }
        Column
        {
            anchors.centerIn: parent
            width: parent.width * 3 / 4
            YACLineEditWithHeader
            {
                headerText: qsTr("Login")
                id: login
                width: parent.width
                focus: true
                KeyNavigation.tab: password
            }
            Row
            {
                width: parent.width
                height: password.height
                YACLineEditWithHeader
                {
                    headerText: qsTr("Password")
                    id: password
                    echoMode: viewButton.showPassword ? TextInput.Normal : TextInput.Password
                    width: parent.width - viewButton.width
                    KeyNavigation.tab: login
                }
                YACButton
                {
                    id: viewButton
                    property bool showPassword: false
                    height: password.height
                    width: height
                    text: showPassword ? qsTr("h") : qsTr("v")
                    onClicked: showPassword = !showPassword
                }
            }
            YACButton
            {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Login")
                onClicked:
                {
                    if (login.displayText == "")
                    {
                        yacApp.badMessage(qsTr("Please enter your Login first"), login, null)
                        return
                    }
                    if (password.text == "")
                    {
                        yacApp.badMessage(qsTr("Please enter your Password first"), password, null)
                        return
                    }
                    LoginFunctions.login(yacApp, "aidoo", "https://portal.aidoo-online.de/directLogin?mandant=test", login.displayText, password.text)
                }
            }
        }
    }
}

