import QtQuick 2.15
import "../items"

FocusScope
{
    id: theProfilePage
    anchors.fill: parent
    focus: true
    Component.onCompleted: forceActiveFocus()
    signal closeClicked()
    property bool imageChanged: false
    Rectangle
    {
        anchors.fill: parent
        YACPageColumn
        {
            Item
            {
                width: parent.width
                height: width / 2
                Rectangle
                {
                    radius: width / 2
                    border.width: 1
                    border.color: "black"
                    height: parent.height
                    width: height
                    anchors.centerIn: parent
                    YACRoundedImage
                    {
                        id: theProfileImage
                        anchors.fill: parent
                        mipmap: true
                        autoTransform: true
                        onSourceChanged: theProfilePage.imageChanged = true
                    }
                }
            }

            YACLineEditWithHeader
            {
                id: fstname
                headerText: qsTr("Name")
                text: yacApp.appUserConfig.fstname
            }
            YACLineEditWithHeader
            {
                id: surname
                headerText: qsTr("Surname")
                text: yacApp.appUserConfig.surname
            }
            YACLineEditWithHeader
            {
                id: visible_name
                headerText: qsTr("Visiblename")
                text: yacApp.appUserConfig.visibleName
            }
            YACButton
            {
                width: parent.width
                text: qsTr("Take Profilephoto")
                onClicked: yacApp.takePhoto(true, true, theProfileImage)
            }

            YACButton
            {
                text: qsTr("Save")
                width: parent.width
                onClicked:
                {
                    if (theProfilePage.imageChanged)
                    {
                        console.log("image changed")
                        return
                    }
                    var searching_exactly_allowed = true
                    var searching_fuzzy_allowed = true
                    yacApp.appUserUpdateProfile(fstname.displayText,
                                                surname.displayText,
                                                visible_name.displayText,
                                                "",
                                                searching_exactly_allowed,
                                                searching_fuzzy_allowed,
                                                function(message){},
                                                function(message){})
                }
            }
            YACButton
            {
                text: qsTr("Close")
                onClicked: closeClicked()
                width: parent.width
            }
            YACButton
            {
                text: qsTr("Logout")
                onClicked:
                {
                    yacApp.appUserLogout()
                    closeClicked()
                }
                width: parent.width
            }
        }
    }
}
