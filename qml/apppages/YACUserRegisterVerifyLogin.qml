import QtQuick 2.15
import "../items"
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions


FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()

    function goVerify()
    {
        if (login.displayText != "")
        {
            verifyLogin.text = login.displayText
        }

        registerColumn.visible = false
        verifyColumn.visible = true
    }

    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            id: registerColumn
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
            YACButton
            {
                width: parent.width
                text: qsTr("Login")
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
                    yacApp.appUserLogin(login.displayText,
                                        password.text,
                                        function(message) {
                                            yacApp.goodMessage(qsTr(message))
                                        },
                                        function(message) {
                                            yacApp.badMessage(qsTr(message))
                                        })
                }
            }

            YACButton
            {
                width: parent.width
                text: qsTr("Register")
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

                    yacApp.appUserRegister(login.displayText,
                                           password.text,
                                           function() {
                                               yacApp.goodMessage(qsTr("Registered, please verify."))
                                               goVerify()
                                           },
                                           function() {})
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Verify")
                onClicked: goVerify()
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Reset password")
                onClicked:
                {
                    registerColumn.visible = false
                    verifyColumn.visible = false
                    resetPasswordColumn.visible = true
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Back")
            }
        }
        YACPageColumn
        {
            id: resetPasswordColumn
            visible: false
            YACButton
            {
                width: parent.width
                text: qsTr("Update password")
            }
        }

        YACPageColumn
        {
            id: verifyColumn
            visible: false
            YACLineEditWithHeader
            {
                id: verifyLogin
                width: parent.width
                headerText: qsTr("E-Mail")
                color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
            }
            YACLineEditWithHeader
            {
                id: verifyToken
                width: parent.width
                headerText: qsTr("Code")
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Verify")
                onClicked:
                {
                    if (!EMailPasswordFunctions.checkEMail(Helper, yacApp, verifyLogin.displayText, verifyLogin))
                    {
                        return;
                    }
                    if (verifyToken.displayText == "")
                    {
                        yacApp.badMessage(qsTr("Please enter the code you received by E-Mail."), verifyToken, null)
                        return
                    }

                    yacApp.appUserVerify(verifyLogin.displayText,
                                         verifyToken.displayText,
                                         function() {},
                                         function() {})
                }
            }

            YACButton
            {
                width: parent.width
                text: qsTr("Back")
                onClicked:
                {
                    registerColumn.visible = true
                    verifyColumn.visible = false
                }
            }
        }
    }
}
