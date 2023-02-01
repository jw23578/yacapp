import QtQuick 2.15
import "../items"
import com.yacapp.appuserconfig 1.0

Item
{
    id: appUserProfileIcon
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

    Rectangle
    {
        visible: !Constants.profileOpen
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width / 8
        height: width
        radius: Constants.radius
        color: "cyan"
        MouseArea
        {
            anchors.fill: parent
            onClicked: Constants.profileOpen = !Constants.profileOpen
        }
        YACText
        {
            anchors.centerIn: parent
            text: "open"
        }
    }

    Column
    {
        visible: Constants.profileOpen
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width / 8
        spacing: 1
        Rectangle
        {
            radius: Constants.radius
            width: parent.width
            height: width
            color: "cyan"
            MouseArea
            {
                anchors.fill: parent
                onClicked: profileLoader.sourceComponent = selectLanguageComponent
            }
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("Language")
            }
        }
        Rectangle
        {
            radius: Constants.radius
            width: parent.width
            height: width
            color: "cyan"
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if (yacApp.appUserConfig.loginToken == "")
                    {
                        profileLoader.sourceComponent = tokenLogin
//                        profileLoader.sourceComponent = registerVerifyLoginComponent
                    }
                    else
                    {
                        profileLoader.sourceComponent = profileComponent
                    }
                }
            }
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("Profile")
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
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("Messages")
            }
        }
        Rectangle
        {
            visible: yacApp.globalConfig.appUserWorktimeEnabled
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
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("Worktime")
            }
        }
        Rectangle
        {
            width: parent.width
            height: width
            radius: Constants.radius
            color: "cyan"
            MouseArea
            {
                anchors.fill: parent
                onClicked: Constants.profileOpen = !Constants.profileOpen
            }
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("Close")
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
        id: tokenLogin
        YACUserTokenLogin
        {
            onCloseClicked: profileLoader.sourceComponent = null
        }
    }

    Component
    {
        id: selectLanguageComponent
        YACUserSelectLanguage
        {
            onCloseClicked: profileLoader.sourceComponent = null
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
            onCloseClicked: profileLoader.sourceComponent = null
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
