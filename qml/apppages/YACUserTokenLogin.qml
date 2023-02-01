import QtQuick 2.15
import QtQuick.Controls 2.15
import "../items"
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions


Rectangle
{
    id: theTokenLogin
    anchors.fill: parent
    signal closeClicked()
    SwipeView
    {
        id: theSwipeView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: closeButton.top
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
                }
                YACButton
                {
                    width: parent.width
                    text: qsTr("Next")
                    onClicked:
                    {
                        if (!EMailPasswordFunctions.checkEMail(Helper, yacApp, login.displayText, login))
                        {
                            return;
                        }
                        yacApp.appUserRegister(login.displayText,
                                               "",
                                               function(message)
                                               {
                                                   if (message == "appuser registered, please verify")
                                                   {
                                                       console.log("weiter zum token")
                                                       tokenOrPasswort.byToken = true
                                                   }
                                                   theSwipeView.setCurrentIndex(2)
                                               },
                                               function(message) {
                                                   console.log(message)
                                                   if (message == "LoginEMail already exists.")
                                                   {
                                                       console.log("weiter zum passwort")
                                                       tokenOrPasswort.byToken = false
                                                       theSwipeView.setCurrentIndex(2)
                                                       return
                                                   }
                                                   yacApp.badMessage(qsTr(message), null, null)
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
                            yacApp.goodMessage(qsTr("Please enter the Code you received by E-Mail"), token, null)
                        }
                        yacApp.appUserVerify(login.displayText,
                                             token.displayText,
                                             function(message) {
                                                 console.log(message)
                                                 if (tokenButton.text == qsTr("Login"))
                                                 {
                                                     yacApp.goodMessage(qsTr("You are logged in. Have fun!"), null, null)
                                                     return
                                                 }
                                                 yacApp.goodMessage(qsTr("Verification successful, you are logged in. Have fun!"), null, null)
                                             },
                                             function(message) {
                                                 yacApp.badMessage(message, verifyToken, null)
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
                }
                YACButton
                {
                    width: parent.width
                    visible: !tokenOrPasswort.byToken
                    text: qsTr("Login")
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
                        tokenButton.text = qsTr("Login")
                        tokenOrPasswort.byToken = true
                    }
                }
            }
        }
    }
    YACButton
    {
        id: closeButton
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * Constants.defaultWidthFactor
        text: qsTr("Close")
        onClicked: theTokenLogin.closeClicked()
    }
}
