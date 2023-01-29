import QtQuick 2.15
import "../items"
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions


FocusScope
{
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    signal closeClicked()

    function goRegisterVerify()
    {
        if (login.displayText != "")
        {
            registerVerifyLogin.text = login.displayText
        }
        if (password.text != "")
        {
            registerPassword.text = password.text
        }

        loginColumn.visible = false
        registerVerifyColumn.visible = true
    }

    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            id: loginColumn
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
                                            yacApp.goodMessage(qsTr("Login successful, have fun!"), null, null)
                                            closeClicked()
                                        },
                                        function(message) {
                                            yacApp.badMessage(qsTr(message), null, null)
                                        })
                }
            }

            YACButton
            {
                width: parent.width
                text: qsTr("Register / Verify")
                onClicked: goRegisterVerify()
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Reset password")
                onClicked:
                {
                    loginColumn.visible = false
                    registerVerifyColumn.visible = false
                    resetPasswordColumn.visible = true
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Close")
                onClicked: closeClicked()
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
            id: registerVerifyColumn
            visible: false
            YACLineEditWithHeader
            {
                id: registerVerifyLogin
                width: parent.width
                headerText: qsTr("E-Mail")
                color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
            }
            YACLineEditWithHeader
            {
                id: registerPassword
                width: parent.width
                echoMode: TextInput.Password
                headerText: qsTr("Password")
                color: Helper.passwordOk(text) ? Constants.goodColor : Constants.badColor
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
                text: qsTr("Register")
                onClicked:
                {
                    if (!EMailPasswordFunctions.checkEMail(Helper, yacApp, registerVerifyLogin.displayText, registerVerifyLogin))
                    {
                        return;
                    }
                    if (!EMailPasswordFunctions.checkPassword(Helper, yacApp, registerPassword.text, registerPassword))
                    {
                        return;
                    }

                    yacApp.appUserRegister(registerVerifyLogin.displayText,
                                           registerPassword.text,
                                           function(message) {
                                               yacApp.goodMessage(qsTr("Registration successful, please verify with the Code you received per E-Mail."), verifyToken, null)
                                           },
                                           function(message) {
                                               yacApp.badMessage(qsTr(message), null, null)
                                           })
                }
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Verify")
                onClicked:
                {
                    if (!EMailPasswordFunctions.checkEMail(Helper, yacApp, registerVerifyLogin.displayText, registerVerifyLogin))
                    {
                        return;
                    }
                    if (verifyToken.displayText == "")
                    {
                        yacApp.badMessage(qsTr("Please enter the code you received by E-Mail."), verifyToken, null)
                        return
                    }

                    yacApp.appUserVerify(registerVerifyLogin.displayText,
                                         verifyToken.displayText,
                                         function(message) {
                                             yacApp.goodMessage(qsTr("Verification successful, you are logged in. Have fun!"), null, null)
                                         },
                                         function(message) {
                                             yacApp.badMessage(message, verifyToken, null)
                                         })
                }
            }

            YACButton
            {
                width: parent.width
                text: qsTr("Back")
                onClicked:
                {
                    loginColumn.visible = true
                    registerVerifyColumn.visible = false
                }
            }
        }
    }
}
