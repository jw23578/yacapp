import QtQuick 2.15
import "../../items"
import ".."
import "../messages"

AppUserBasePage
{
    property var currentProfile: null

    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Contact")},
        {caption: qsTr("Fetch")}]
    multiMenueButton.onClicked:
    {
        console.log("caption: " + caption)
        if (caption == qsTr("Add Contact"))
        {
            theLoader.sourceComponent = searchProfiles
        }
        if (caption == qsTr("Fetch"))
        {
            yacApp.fetchMessageUpdates()
        }
    }
    multiMenueButton.hide: theLoader.sourceComponent != null

    ListView
    {
        id: listView
        clip: true
        anchors.fill: parent
        model: KnownProfilesModel
        delegate: Row
        {
            height: 50
            Rectangle
            {
                width: listView.width - height
                height: 50
                YACRoundedImage
                {
                    height: parent.height
                    width: height
                    source: "image://async/profileImage/" + profile.profileImageId
                }

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
                        currentProfile = profile
                        yacApp.loadMessages(profile.id)
                        theLoader.sourceComponent = messages
                    }
                }
                Component.onCompleted:
                {
                    yacApp.fetchProfileAndUpsertKnownProfiles(profile.id)
                }
            }
            YACButton
            {
                height: parent.height
                width: height
                text: "-"
                onClicked: CPPQMLAppAndConfigurator.yesNoQuestion(qsTr("Delete this Contact?"), null,
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

    Loader
    {
        id: theLoader
        anchors.fill: parent
        onLoaded:
        {
            if (sourceComponent == messages)
            {
                item.profile = currentProfile
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
                yacApp.addProfileToKnownProfiles(id)
                console.log("id: " + id)
                theLoader.sourceComponent = null
            }
            onCloseClicked: theLoader.sourceComponent = null
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
