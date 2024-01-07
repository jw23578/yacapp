import QtQuick 2.15
import "../../items"
import ".."
import "../messages"

AppUserBasePage2
{
    id: yacUserKnownProfiles
    property var currentProfile: null

    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Contact")},
        {caption: qsTr("Fetch")},
        {caption: qsTr("delete all messages")}]
    multiMenueButton.onClicked:
    {
        Helper.jsLog("caption: " + caption)
        if (caption == qsTr("delete all messages"))
        {
            yacApp.deleteAllMyMessages()
        }
        if (caption == qsTr("Add Contact"))
        {
            theLoader.theComponent = searchProfiles
            theLoader.open()
        }
        if (caption == qsTr("Fetch"))
        {
            yacApp.fetchMessageUpdates()
        }
    }
    multiMenueButton.hide: theLoader.isOpen

    ListView
    {
        id: listView
        clip: true
        anchors.fill: content
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
                        if (profile.public_key_base64 == "")
                        {
                            CPPQMLAppAndConfigurator.badMessage(qsTr("Receiver has no public Key to encrypt message, please try again later.", null, null))
                            return;
                        }

                        currentProfile = profile
                        yacApp.loadMessages(profile.id)
                        theLoader.theComponent = messages
                        theLoader.open()
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

    Component
    {
        id: searchProfiles
        YACUserSearchProfiles
        {
            onProfileSelected:
            {
                yacApp.addProfileToKnownProfiles(id)
                Helper.jsLog("id: " + id)
                theLoader.close()
            }
            onCloseClicked: {
                theLoader.close()
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
                theLoader.close()
            }
        }
    }
    BasePageLoader
    {
        z: 1
        id: theLoader
        onLoaded:
        {
            if (sourceComponent == messages)
            {
                item.profile = currentProfile
            }
        }
    }
}
