import QtQuick 2.15
import QtQuick.Controls 2.15
import "../../items"
import ".."
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions


AppUserBasePage2
{    
    id: theTokenLogin
    function checkLogin()
    {
        if (yacApp.globalConfig.third == "aidoo")
        {
            // aidoo akzeptiert emails und andere logins -> fast keine prüfung notwendig
            if (loginEMail.displayText == "")
            {
                CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a valid E-Mail / Login"), loginEMail, null)
                return false
            }
        }
        else
        {
            if (!EMailPasswordFunctions.checkEMail(Helper, CPPQMLAppAndConfigurator, loginEMail.displayText, loginEMail))
            {
                return false
            }
        }
        return true
    }
    anchors.fill: parent
    signal loginSuccessful()
    SwipeView
    {
        id: theSwipeView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomRectangle.top
        interactive: false
        Item
        {
            id: helloView
            YACButtonWithHeader
            {
                headerText: qsTr("Login or Register first")
                width: parent.width * Constants.defaultWidthFactor
                anchors.centerIn: parent
                text: qsTr("Let's start")
                onClicked: theSwipeView.setCurrentIndex(1)
            }
        }
        Item
        {
            id: theLoginRegisterView
            property int theViewIndex: 1
            YACPageColumn
            {
                spacing: loginEMail.height / 2
                YACLineEdit
                {
                    emptyText.text: qsTr("LoginEMail")
                    id: loginEMail
                    Keys.onTabPressed: password.focus = true
                    Keys.onBacktabPressed: password.focus = true
                    showGoodBadIndicator: true
                    dataIsGood: Helper.loginIsValid(displayText, yacApp.globalConfig.third)
                    emailEdit: true
                }
                YACLineEdit
                {
                    emptyText.text: qsTr("Password")
                    id: password
                    echoMode: TextInput.Password
                    Keys.onTabPressed: loginEMail.focus = true
                    Keys.onBacktabPressed: loginEMail.focus = true
                }



                YACButton
                {
                    width: parent.width
                    text: qsTr("Login")
                    onClicked:
                    {
                        Helper.jsLog("login clicked, checkEMail")
                        if (!theTokenLogin.checkLogin())
                        {
                            return;
                        }
                        if (!EMailPasswordFunctions.checkPassword(Helper, CPPQMLAppAndConfigurator, password.text, password))
                        {
                            return;
                        }
                        CPPQMLAppAndConfigurator.waitMessage(qsTr("Login in progress, please wait."))

                        yacApp.appUserLogin(loginEMail.displayText,
                                            password.text,
                                            function (message)
                                            {
                                                password.text = ""
                                                CPPQMLAppAndConfigurator.hideWaitMessage()
                                                CPPQMLAppAndConfigurator.goodMessage(qsTr("Login successful, have fun!"), null, null)
                                                loginSuccessful();
                                                theSwipeView.setCurrentIndex(0)
                                            },
                                            function (message)
                                            {
                                                CPPQMLAppAndConfigurator.hideWaitMessage()
                                                if (message == "LoginEMail/User not yet verified")
                                                {
                                                    Helper.jsLog("weiter zum verifizieren")
                                                    CPPQMLAppAndConfigurator.badMessage(qsTr("Login is not yet verified"), null, null)
                                                    theSwipeView.setCurrentIndex(theVerifyView.theViewIndex)
                                                    return;
                                                }
                                                CPPQMLAppAndConfigurator.badMessage(qsTr(message), null, null)
                                            }
                                            )

                    }
                }
                YACButton
                {
                    width: parent.width
                    text: qsTr("Register")
                    onClicked:
                    {
                        Helper.jsLog("register clicked, checkEMail")
                        if (!theTokenLogin.checkLogin())
                        {
                            return;
                        }
                        yacApp.appUserRegister(loginEMail.displayText,
                                               password.text,
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
                                                       Helper.jsLog("weiter zum verifizieren")
                                                       CPPQMLAppAndConfigurator.badMessage(qsTr("Login is not yet verified"), null, null)
                                                       theSwipeView.setCurrentIndex(theVerifyView.theViewIndex)
                                                       return
                                                   }
                                                   CPPQMLAppAndConfigurator.badMessage(qsTr(message), null, null)
                                               })
                    }
                }
            }
        }
        Item
        {
            property int theViewIndex: 2
            id: theVerifyView
            YACPageColumn
            {
                spacing: loginEMail.height / 2
                YACText
                {
                    id: verifyText
                    text: qsTr("The Verifycode was sent to you by mail.")
                    width: parent.width
                }
                YACLineEdit
                {
                    id: theVerifyToken
                    emptyText.text: qsTr("Verifytoken")
                    intEdit: true
                }
                YACButton
                {
                    id: theVerifyTokenButton
                    width: parent.width
                    text: qsTr("Verify and Login")
                    onClicked:
                    {
                        if (theVerifyToken.displayText === "")
                        {
                            CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a valid Verifytoken"), theVerifyToken, null)
                            return;
                        }
                        CPPQMLAppAndConfigurator.waitMessage(qsTr("Login in progress, please wait."))
                        yacApp.appUserVerify(loginEMail.displayText,
                                             theVerifyToken.displayText,
                                             function(message) {
                                                 CPPQMLAppAndConfigurator.hideWaitMessage()
                                                 CPPQMLAppAndConfigurator.goodMessage(qsTr("Verification successful, you are logged in. Have fun!"), null, null)
                                                 loginSuccessful();
                                                 theSwipeView.setCurrentIndex(0)

                                             },
                                             function(message) {
                                                 CPPQMLAppAndConfigurator.hideWaitMessage()
                                                 if (message == "wrong token")
                                                 {
                                                     CPPQMLAppAndConfigurator.badMessage(qsTr("This token is incorrect."), token, null)
                                                     return
                                                 }
                                                 CPPQMLAppAndConfigurator.badMessage(message, token, null)
                                             })
                    }
                }
                YACButton
                {
                    id: toPassword
                    width: parent.width
                    text: qsTr("Back to Login/Register")
                    onClicked: theSwipeView.setCurrentIndex(theLoginRegisterView.theViewIndex)
                }

                YACButton
                {
                    width: parent.width
                    text: yacApp.globalConfig.third != "" ? qsTr("Request Passwortreset E-Mail") : qsTr("Request Verifycode")
                    onClicked:
                    {
                        if (yacApp.globalConfig.third != "")
                        {
                            CPPQMLAppAndConfigurator.badMessage("not yet implemented", null, null)
                            return;
                        }

                        yacApp.appUserRequestVerifyToken(loginEMail.displayText,
                                                         function(message)
                                                         {
                                                             CPPQMLAppAndConfigurator.goodMessage(qsTr("Code sended, please check your E-Mails."), null, null)
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
