import QtQuick 2.15
import "../items"
import com.yacapp.appuserconfig 1.0

Item
{
    visible: !Constants.topLevelFormActive
    function fetchMessages()
    {
        console.log("fetch messages")
        yacApp.fetchMessageUpdates()
    }

    Component.onCompleted:
    {
        if (Constants.isDesktop)
        {
            fetchMessages()
        }
    }

    Timer
    {
        running: Constants.isDesktop
        interval: 10000
        onTriggered: fetchMessages()
        repeat: true
    }

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
                onClicked:
                {
                    if (!yacApp.appUserConfig.loggedIn)
                    {
                        yacApp.badMessage(qsTr("Please login first."), null, null);
                        return
                    }
                    profileLoader.sourceComponent = knownProfilesComponent
                }
            }
        }
        Rectangle
        {
            visible: true
            radius: Constants.radius
            width: parent.width
            height: width
            color: "orange"
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if (!yacApp.appUserConfig.loggedIn)
                    {
                        yacApp.badMessage(qsTr("Please login first."), null, null);
                        return
                    }
                    profileLoader.sourceComponent = worktimeComponent
                }
            }
        }
    }
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
        id: knownProfilesComponent
        YACUserKnownProfiles
        {
            onCloseClicked: profileLoader.sourceComponent = null
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
