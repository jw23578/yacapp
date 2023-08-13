import QtQuick 2.15
import "../items"
import ".."

Rectangle
{
    id: deployPage
    anchors.fill: parent
    YACText
    {
        visible: Constants.isDesktop
        anchors.left: parent.left
        anchors.top: parent.top
        text: "ServerURL: " + configurator.getYacappServerUrl()
    }

    Column
    {
        id: theColumn
        anchors.centerIn: parent
        spacing: 1
        width: parent.width * 3 / 4
        YACLineEditWithHeader
        {
            headerText: qsTr("LoginEMail")
            width: parent.width
            id: loginEMail
            text: configurator.activeProjectData.deployUser
        }
        YACLineEditWithHeader
        {
            headerText: qsTr("Password")
            width: parent.width
            id: password
            echoMode: TextInput.Password
            text: configurator.activeProjectData.deployPassword
        }
//        YACLineEditWithHeader
//        {
//            headerText: qsTr("www-Basedirectory")
//            width: parent.width
//            id: basedirectory
//            visible: !deployToYACAPPServer
//        }
        YACButton
        {
            text: configurator.activeProjectData.yacappServerLoginToken.length ? qsTr("LoggedIn") : qsTr("Login")
            width: parent.width
            onClicked:
            {
                if (configurator.activeProjectData.yacappServerLoginToken.length)
                {
                    CPPQMLAppAndConfigurator.goodMessage(qsTr("You are already logged in."), null, null)
                    return
                }

                configurator.yacserverLogin(loginEMail.displayText,
                                            password.text,
                                            yacApp.globalConfig.projectID,
                                            function ()
                                            {
                                                CPPQMLAppAndConfigurator.goodMessage(configurator.activeProjectData.yacappServerLoginToken, null)
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
            text: qsTr("Registrieren/Verifizieren")
            width: parent.width
            onClicked: registerVerifyPage.show(loginEMail.displayText)
        }

        YACButton
        {
            width: parent.width
            id: templateAppButton
            text: yacApp.globalConfig.isTemplateApp ? qsTr("APP is offered as Startingpoint for others") : qsTr("APP is not offered as Startingpoint for others")
            onClicked: yacApp.globalConfig.isTemplateApp = !yacApp.globalConfig.isTemplateApp
        }

        YACButton
        {
            text: qsTr("Upload")
            width: parent.width
            onClicked:
            {
                if (configurator.activeProjectData.yacappServerLoginToken == "")
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please login first"), null, null)
                    return
                }
                configurator.deploy(function()
                                    {
                                        CPPQMLAppAndConfigurator.goodMessage(qsTr("Deployment successful"), null, null)
                                    },
                                    function(message)
                                    {
                                        CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                    })
            }
        }
        YACButton
        {
            text: qsTr("Abort")
            onClicked: deployPage.visible = false
            width: parent.width
        }
        YACButton
        {
            text: qsTr("APP-Images")
            onClicked: appImages.visible = true
            width: parent.width
        }
    }
    ProgressPage
    {
        id: pp
    }
    ConfiguratorRegisterVerifyPage
    {
        visible: false
        id: registerVerifyPage
    }
    ConfiguratorAppImagesPage
    {
        id: appImages
        visible: false
    }
}
