import QtQuick 2.15
import "../items"

Rectangle
{
    anchors.fill: parent
    signal profileSelected(string id)
    signal closeClicked()
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
        anchors.bottom: closeButton.top
        model: SearchProfilesModel
        delegate: Rectangle
        {
            width: listView.width
            height: 50
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
                    yacApp.addProfileToKnownProfiles(profile.id)
                    profileSelected(profile.id)
                }
            }
        }

    }
    YACButton
    {
        id: closeButton
        anchors.bottom: parent.bottom
        width: parent.width * Constants.defaultWidthFactor
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Close")
        onClicked: closeClicked()
    }
}
