import QtQuick 2.15
import "../items"
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions

FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            visible: true
            YACLineEditWithHeader
            {
                id: login
                width: parent.width
                headerText: qsTr("E-Mail")
                color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
            }
            YACLineEditWithHeader
            {
                id: password
                width: parent.width
                echoMode: TextInput.Password
                headerText: qsTr("Password")
                color: Helper.passwordOk(text) ? Constants.goodColor : Constants.badColor
            }
            YACLineEditWithHeader
            {
                id: updatePasswordToken
                width: parent.width
                headerText: qsTr("Code")
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Update password")
                onClicked:
                {
                    if (!EMailPasswordFunctions.checkEMail(Helper, yacApp, login.displayText, login))
                    {
                        return;
                    }
                    if (!EMailPasswordFunctions.checkPassword(Helper, yacApp, password.text, password))
                    {
                        return;
                    }
                    yacApp.appUserUpdatePassword(login.displayText,
                                                 password.text,
                                                 updatePasswordToken.displayText,
                                                 function(message) {
                                                     yacApp.goodMessage(qsTr(message), null, null)
                                                 },
                                                 function(message) {
                                                     yacApp.badMessage(qsTr(message), null, null)
                                                 })
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Back")
            }
        }
    }

}
