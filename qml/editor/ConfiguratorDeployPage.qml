import QtQuick 2.15
import "../items"
import ".."

Rectangle
{
    id: deployPage
    anchors.fill: parent
    Column
    {
        id: theColumn
        anchors.centerIn: parent
        spacing: 1
        width: parent.width * 3 / 4
//        YACLineEditWithHeader
//        {
//            headerText: qsTr("Host")
//            width: parent.width
//            id: host
//            visible: !deployToYACAPPServer
//        }
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
                    yacApp.goodMessage(qsTr("You are already logged in."))
                    return
                }

                configurator.yacserverLogin(loginEMail.displayText, password.text, yacApp.globalConfig.projectID)
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
            text: qsTr("Upload")
            width: parent.width
            onClicked:
            {
                if (configurator.activeProjectData.yacappServerLoginToken == "")
                {
                    yacApp.badMessage(qsTr("Please login first"))
                    return
                }
                configurator.deployToYACAPPServer(yacApp.globalProjectConfigFilename)
                return;
                configurator.defaultDeploy(yacApp.globalProjectConfigFilename, host.displayText, loginEMail.displayText, password.text);
//                configurator.deploy(yacApp.globalConfig.projectID, host.displayText, user.displayText, password.text, basedirectory.displayText)
            }
        }
        YACButton
        {
            text: qsTr("Abbrechen")
            onClicked: deployPage.visible = false
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

    onVisibleChanged:
    {
        if (visible)
        {
            configurator.setProjectData(yacApp.globalConfig.projectID)
            //host.text = configurator.getProjectData(yacApp.globalConfig.projectID).deployUrl
//            loginEMail.text = configurator.getProjectData(yacApp.globalConfig.projectID).deployUser
//            password.text = configurator.getProjectData(yacApp.globalConfig.projectID).deployPassword
            //basedirectory.text = configurator.getProjectData(yacApp.globalConfig.projectID).deployBaseDirectory
        }
    }
    Connections
    {
        target: configurator
        function onLoginSuccessful()
        {
            yacApp.goodMessage(configurator.activeProjectData.yacappServerLoginToken)
        }
        function onLoginNotSuccessful(message)
        {
            yacApp.badMessage(message)
        }
        function onDeployToYACAPPServerSuccessful()
        {
            yacApp.goodMessage(qsTr("Deployment successful"))
        }
        function onDeployToYACAPPServerNotSuccessful(message)
        {
            yacApp.badMessage(message)
        }
    }
}