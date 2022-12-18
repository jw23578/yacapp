import QtQuick 2.15
import "../items"
import com.yacapp.appuserconfig 1.0

Item
{
    visible: !Constants.topLevelFormActive
    Column
    {
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width / 8
        spacing: 1
        Rectangle
        {
            radius: Constants.radius
            width: parent.width
            height: width
            color: "blue"
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if (yacApp.appUserConfig.loginToken == "")
                    {
                        profileLoader.sourceComponent = registerVerifyLoginComponent
                    }
                    else
                    {
                        profileLoader.sourceComponent = profileComponent
                    }
                }
            }
        }
        Rectangle
        {
            visible: yacApp.globalConfig.appUserMessagesEnabled
            radius: Constants.radius
            width: parent.width
            height: width
            color: "yellow"
            MouseArea
            {
                anchors.fill: parent
                onClicked: profileLoader.sourceComponent = messagesComponent
            }
        }
        Rectangle
        {
            visible: true // yacApp.globalConfig.appUserMessagesEnabled
            radius: Constants.radius
            width: parent.width
            height: width
            color: "orange"
            MouseArea
            {
                anchors.fill: parent
                onClicked: profileLoader.sourceComponent = worktimeComponent
            }
        }    }
    Connections
    {
        target: yacApp.appUserConfig
        function onLoginTokenChanged()
        {
            if (yacApp.appUserConfig.loginToken.length > 0 && profileLoader.sourceComponent != null)
            {
                profileLoader.sourceComponent = profileComponent
            }
        }
    }
    Component
    {
        id: messagesComponent
        YACUserMessages
        {

        }
    }
    Component
    {
        id: worktimeComponent
        YACUserWorktimeState
        {

        }
    }

    Component
    {
        id: profileComponent
        YACUserProfile
        {
            onCloseClicked: profileLoader.sourceComponent = null
        }
    }

    Component
    {
        id: registerVerifyLoginComponent
        YACUserRegisterVerifyLogin
        {
            onCloseClicked: profileLoader.sourceComponent = null
        }
    }

    Loader
    {
        id: profileLoader
        anchors.fill: parent
    }
}
