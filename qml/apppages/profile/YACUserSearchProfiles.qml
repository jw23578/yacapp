import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage2
{
    enableBack: true
    signal profileSelected(string id)
    YACLineEditWithHeader
    {
        id: needleText
        headerText: qsTr("Search for")
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * Constants.defaultWidthFactor
        property string lastDisplayText: ""
        onDisplayTextChanged:
        {
            if (lastDisplayText == displayText)
            {
                return
            }
            lastDisplayText = displayText

            let limit = 0
            let offset = 0
            yacApp.appUserSearchProfiles(displayText,
                                               limit,
                                               offset,
                                         function(message){},
                                         function(message){})
        }
    }
    ListView
    {
        id: listView
        clip: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: needleText.bottom
        anchors.bottom: bottomRectangle.top
        model: SearchProfilesModel
        delegate: Rectangle
        {
            width: listView.width
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
                text: profile.visibleName
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    profileSelected(profile.id)
                }
            }
        }

    }
}
