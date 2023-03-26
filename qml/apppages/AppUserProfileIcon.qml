import QtQuick 2.15
import "../items"
import "rights"
import "spaces"
import "../editor"
import com.yacapp.appuserconfig 1.0

Item
{
    id: appUserProfileIcon
    visible: !Constants.topLevelFormActive
    function fetchMessages()
    {
        return
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
        anchors.top: parent.verticalCenter
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
                onClicked: profileLoader.sourceComponent = designEditorComponent
            }
            YACText
            {
                anchors.centerIn: parent
                text: qsTr("Design")
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
        id: designEditorComponent
        ConfiguratorDesignEditor
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





    Loader
    {
        id: profileLoader
        anchors.fill: parent
    }
}
