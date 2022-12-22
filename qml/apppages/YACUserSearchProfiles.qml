import QtQuick 2.15
import "../items"

Rectangle
{
    anchors.fill: parent
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
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: needleText.bottom
        anchors.bottom: parent.bottom
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
                onClicked: yacApp.addProfileToKnownProfiles(profile.id)
            }
        }

    }
}
