import QtQuick 2.15
import "../items"
import ".."

ConfiguratorPage
{

    Column
    {
        id: theVerifyColumn

        opacity: 0
        enabled: opacity === 1
        Behavior on opacity {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }

        anchors.centerIn: contentItem
        anchors.verticalCenterOffset: parent.height / 2
        spacing: loginEMail.height / 2
        width: parent.width * 3 / 4
        Behavior on anchors.verticalCenterOffset {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }
        function close()
        {
            theVerifyToken.text = ""
            opacity = 0
            anchors.verticalCenterOffset = parent.height / 2
        }
        function open()
        {
            opacity = 1
            anchors.verticalCenterOffset = 0
            theVerifyToken.focus = true
        }

        YACText
        {
            id: theVerifyText
            width: parent.width
        }
        YACLineEdit
        {
            emptyText.text: qsTr("Verifytoken")
            width: parent.width
            id: theVerifyToken
        }
        YACButton
        {
            text: qsTr("Verify")
            width: parent.width
            onClicked:
            {
                if (theVerifyToken.displayText === "")
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a valid Verifytoken"), theVerifyToken, null)
                    return;
                }

                configurator.verifyCreator(loginEMail.displayText,
                                           theVerifyToken.displayText,
                                           function (message)
                                           {
                                               CPPQMLAppAndConfigurator.goodMessage(message, null, null)
                                               theVerifyColumn.close()
                                           },
                                           function (message)
                                           {
                                               CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                           }
                                           )
            }
        }
        YACButton
        {
            text: qsTr("Request new Verifycode")
            width: parent.width
            onClicked: configurator.requestVerifyTokenCreator(loginEMail.displayText,
                                                              function (message)
                                                              {
                                                                  CPPQMLAppAndConfigurator.goodMessage(message, null, null)
                                                              },
                                                              function (message)
                                                              {
                                                                  CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                                              }
                                                              )
        }
        YACButton
        {
            text: qsTr("Back")
            width: parent.width
            onClicked: theVerifyColumn.close()
        }
    }

    Column
    {
        id: theLoginRegisterColumn
        clip: true
        opacity: 1 - theVerifyColumn.opacity
        enabled: opacity === 1
        anchors.centerIn: contentItem
        spacing: loginEMail.height / 2
        width: parent.width * 3 / 4
        anchors.verticalCenterOffset: parent.height / 2 - theVerifyColumn.anchors.verticalCenterOffset
        YACLineEdit
        {
            emptyText.text: qsTr("LoginEMail")
            width: parent.width
            id: loginEMail
            text: configurator.deployUser
            Keys.onTabPressed: password.focus = true
            Keys.onBacktabPressed: password.focus = true
        }
        YACLineEdit
        {
            emptyText.text: qsTr("Password")
            width: parent.width
            id: password
            echoMode: TextInput.Password
            Keys.onTabPressed: loginEMail.focus = true
            Keys.onBacktabPressed: loginEMail.focus = true
        }
        YACButton
        {
            text: qsTr("Login")
            width: parent.width
            onClicked:
            {
                if (!Helper.emailIsValid(loginEMail.displayText))
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a valid email"), loginEMail, null)
                    return
                }
                if (password.text === "")
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a valid password"), password, null)
                    return;
                }

                configurator.yacserverLogin(loginEMail.displayText,
                                            password.text,
                                            function (message)
                                            {
                                                //                                                CPPQMLAppAndConfigurator.goodMessage(configurator.activeProjectData.yacappServerLoginToken, null)
                                                password.text = ""
                                            },
                                            function (message)
                                            {
                                                CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                            }
                                            )
            }
        }
        YACButton
        {
            text: qsTr("Register")
            width: parent.width
            onClicked:
            {
                configurator.yacserverRegister(loginEMail.displayText,
                                               password.text,
                                               function (message)
                                               {
                                                   //                                                CPPQMLAppAndConfigurator.goodMessage(configurator.activeProjectData.yacappServerLoginToken, null)
                                                   theVerifyColumn.open()
                                                   theVerifyText.text = qsTr("The Verifycode was sent to you by mail.")
                                               },
                                               function (message)
                                               {
                                                   if (message === "LoginEMail already exists.")
                                                   {
                                                       theVerifyColumn.open()
                                                       theVerifyText.text = qsTr("Login-E-Mail already registered, please verify or Login..")
                                                       return
                                                   }

                                                   CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                               }
                                               )
            }
        }
    }
    Component.onCompleted: loginEMail.focus = true
}
