import QtQuick 2.15
import "../items"

Rectangle
{
    anchors.fill: parent

    property string currentProfileId: ""

    YACButton
    {
        id: addButton
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * Constants.defaultWidthFactor
        text: qsTr("Add Contact")
        onClicked: theLoader.sourceComponent = searchProfiles
    }

    ListView
    {
        id: listView
        clip: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: addButton.bottom
        anchors.bottom: parent.bottom
        model: KnownProfilesModel
        delegate: Rectangle
        {
            width: listView.width
            height: 50
            Text
            {
                anchors.centerIn: parent
                text: profile.visibleName + " " + profile.unreadMessages
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    currentProfileId = profile.id
                    yacApp.loadMessages(profile.id)
                    theLoader.sourceComponent = messages
                }
            }
        }

    }
    Loader
    {
        id: theLoader
        anchors.fill: parent
        onLoaded:
        {
            if (sourceComponent == messages)
            {
                item.profileId = currentProfileId
            }
        }
    }
    Component
    {
        id: searchProfiles
        YACUserSearchProfiles
        {
            onProfileSelected:
            {
                console.log("id: " + id)
                theLoader.sourceComponent = null
            }
        }
    }
    Component
    {
        id: messages
        YACUserMessages
        {
            onCloseClicked:
            {
                theLoader.sourceComponent = null
            }
        }
    }
}
