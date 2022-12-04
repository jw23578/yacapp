import QtQuick 2.15
import "../items"
import com.yacapp.appuserconfig 1.0

Item
{
    property bool opened: false
    visible: !Constants.topLevelFormActive
    Rectangle
    {
        radius: Constants.radius
        anchors.top: parent.top
        anchors.right: parent.right
        width: parent.width / 8
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
                    opened = true
                }
                else
                {
                    console.log("profile")
                }
            }
        }
    }
    Connections
    {
        target: yacApp.appUserConfig
        function onLoginTokenChanged()
        {
            if (yacApp.appUserConfig.loginToken.length > 0 && opened)
            {
                profileLoader.sourceComponent = profileComponent
            }
        }
    }
    Component
    {
        id: profileComponent
        Rectangle
        {
            anchors.fill: parent
            color: "blue"
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
