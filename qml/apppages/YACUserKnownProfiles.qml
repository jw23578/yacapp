import QtQuick 2.15
import "../items"

Rectangle
{
    anchors.fill: parent

    signal closeClicked()

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
        anchors.bottom: closeButton.top
        model: KnownProfilesModel
        delegate: Row
        {
            height: 50
            Rectangle
            {
                width: listView.width - height
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
            YACButton
            {
                height: parent.height
                width: height
                text: "-"
                onClicked: yacApp.yesNoQuestion(qsTr("Delete this Contact?"), null,
                                                function() {
                                                    yacApp.removeProfileFromKnownProfiles(profile.id)
                                                },
                    function(){})
            }
        }
        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 300 }
        }
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 300 }
        }
    }
    YACButton
    {
        id: closeButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: qsTr("Close")
        onClicked: closeClicked()
    }
    YACButton
    {
        id: fetchButton
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: qsTr("Fetch")
        onClicked: yacApp.fetchMessageUpdates()
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
