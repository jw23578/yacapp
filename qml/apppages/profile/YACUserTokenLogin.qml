import QtQuick 2.15
import QtQuick.Controls 2.15
import "../../items"
import ".."
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions


AppUserBasePage
{
    id: theTokenLogin
    anchors.fill: parent
    showCloseButton: true
    signal loginSuccessful()
    SwipeView
    {
        id: theSwipeView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
        Rectangle
        {
            id: helloView
            YACButton
            {
                width: parent.width * Constants.defaultWidthFactor
                anchors.centerIn: parent
                text: qsTr("Let's start")
                onClicked: theSwipeView.setCurrentIndex(1)
            }
        }
        Rectangle
        {
            id: emailView
            YACPageColumn
            {
                YACText
                {
                    text: qsTr("Please enter your E-Mail")
                }
                YACLineEdit
                {
                    id: login
                    color: Helper.emailIsValid(displayText) ? Constants.goodColor : Constants.badColor
                    text: yacApp.appUserConfig.loginEMail
                }
                YACButton
                {
                    width: parent.width
                    text: qsTr("Next")
                    onClicked:
                    {
                        Helper.jsLog("next clicked, checkEMail")
                        if (!EMailPasswordFunctions.checkEMail(Helper, CPPQMLAppAndConfigurator, login.displayText, login))
                        {
                            return;
                        }
                        Helper.jsLog("next clicked, email ok")
                        yacApp.appUserRegister(login.displayText,
                                               "",
                                               function(message)
                                               {
                                                   if (message == "appuser registered, please verify")
                                                   {
                                                       Helper.jsLog("weiter zum token")
                                                       tokenOrPasswort.byToken = true
                                                   }
                                                   theSwipeView.setCurrentIndex(2)
                                               },
                                               function(message) {
                                                   Helper.jsLog(message)
                                                   if (message == "LoginEMail already exists.")
                                                   {
                                                       Helper.jsLog("weiter zum passwort")
                                                       tokenOrPasswort.byToken = false
                                                       theSwipeView.setCurrentIndex(2)
                                                       return
                                                   }
                                                   CPPQMLAppAndConfigurator.badMessage(qsTr(message), null, null)
                                               })
                    }
                }
            }
        }
        Rectangle
        {
            id: tokenOrPasswort
            property bool byToken: true
            YACPageColumn
            {
                YACText
                {
                    visible: tokenOrPasswort.byToken
                    id: tokenText
                    text: qsTr("Please enter the code you received by E-Mail.")
                }
                YACLineEdit
                {
                    id: token
                    visible: tokenOrPasswort.byToken
                    intEdit: true
                }
                YACButton
                {
                    id: tokenButton
                    width: parent.width
                    visible: tokenOrPasswort.byToken
                    text: qsTr("Verify and Login")
                    onClicked:
                    {
                        if (token.displayText.length != 4)
                        {
                            CPPQMLAppAndConfigurator.goodMessage(qsTr("Please enter the Code you received by E-Mail"), token, null)
                        }
                        yacApp.appUserVerify(login.displayText,
                                             token.displayText,
                                             function(message) {
                                                 token.text = ""
                                                 if (tokenButton.text == qsTr("Login"))
                                                 {
                                                     CPPQMLAppAndConfigurator.goodMessage(qsTr("You are logged in. Have fun!"), null, null)
                                                 } else
                                                 {
                                                    CPPQMLAppAndConfigurator.goodMessage(qsTr("Verification successful, you are logged in. Have fun!"), null, null)
                                                 }
                                                 loginSuccessful();
                                                 theSwipeView.setCurrentIndex(0)

                                             },
                                             function(message) {
                                                 CPPQMLAppAndConfigurator.badMessage(message, verifyToken, null)
                                             })
                    }
                }
                YACButton
                {
                    id: toPassword
                    width: parent.width
                    visible: tokenOrPasswort.byToken && tokenButton.text == qsTr("Login")
                    text: qsTr("Back to Password")
                    onClicked: tokenOrPasswort.byToken = false
                }

                YACText
                {
                    visible: !tokenOrPasswort.byToken
                    text: qsTr("Please enter your Passwort.")
                }
                YACLineEdit
                {
                    id: password
                    visible: !tokenOrPasswort.byToken
                    echoMode: TextInput.Password
                }
                YACButton
                {
                    width: parent.width
                    visible: !tokenOrPasswort.byToken
                    text: qsTr("Login")
                    onClicked:
                    {
                        if (!EMailPasswordFunctions.checkEMail(Helper, CPPQMLAppAndConfigurator, login.displayText, login))
                        {
                            return;
                        }
                        if (!EMailPasswordFunctions.checkPassword(Helper, CPPQMLAppAndConfigurator, password.text, password))
                        {
                            return;
                        }
                        yacApp.appUserLogin(login.displayText,
                                            password.text,
                                            function(message) {
                                                password.text = ""
                                                CPPQMLAppAndConfigurator.goodMessage(qsTr("Login successful, have fun!"), null, null)
                                                loginSuccessful();
                                                theSwipeView.setCurrentIndex(0)
                                            },
                                            function(message) {
                                                CPPQMLAppAndConfigurator.badMessage(qsTr(message), null, null)
                                            })
                    }
                }
                YACText
                {
                    visible: !tokenOrPasswort.byToken
                    text: qsTr("Forgot your Password?")
                }
                YACButton
                {
                    width: parent.width
                    visible: !tokenOrPasswort.byToken
                    text: qsTr("Request Logintoken")
                    onClicked:
                    {
                        yacApp.appUserRequestVerifyToken(login.displayText,
                                                         function(message)
                                                         {
                                                             CPPQMLAppAndConfigurator.goodMessage(qsTr("Token sended, please check your E-Mails."), null, null)
                                                             tokenButton.text = qsTr("Login")
                                                             tokenOrPasswort.byToken = true
                                                         },
                                                         function(message)
                                                         {
                                                             CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                                         })
                    }
                }
            }
        }
    }
}
