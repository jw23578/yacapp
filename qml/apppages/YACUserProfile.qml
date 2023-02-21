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
            spacing: parent.height / 50
            Item
            {
                width: parent.width
                height: width / 2
                Rectangle
                {
                    id: profileImageRect
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
                        source: "image://async/profileImage/" + yacApp.appUserConfig.profileImageId
                    }
                    Image
                    {
                        id: originalSizeProfileImage
                        visible: false
                        onSourceChanged: {
                            theProfilePage.imageChanged = true
                            theProfileImage.source = source
                        }
                    }
                }
                YACButton
                {
                    anchors.left: profileImageRect.right
                    anchors.bottom: profileImageRect.bottom
                    text: qsTr("+/-")
                    onClicked: yacApp.takePhoto(true, true, originalSizeProfileImage)
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
            YACComboBoxWithHeader
            {
                id: searchOption
                headerText: qsTr("Search Option")
                model: [qsTr("not searchable"),
                    qsTr("only exact searchable"),
                    qsTr("fuzzy searchable")]
                Component.onCompleted:
                {
                    if (yacApp.appUserConfig.searchingExactlyAllowed)
                    {
                        currentIndex = 1
                    }
                    if (yacApp.appUserConfig.searchingFuzzyAllowed)
                    {
                        currentIndex = 2
                    }
                }
            }

            YACButton
            {
                text: qsTr("Save")
                width: parent.width
                onClicked:
                {
                    var searchingExactlyAllowed = searchOption.currentIndex == 1
                    var searchingFuzzyAllowed = searchOption.currentIndex == 2
                    if (theProfilePage.imageChanged)
                    {
                        theProfilePage.imageChanged = false;
                        console.log("image changed")
                        originalSizeProfileImage.grabToImage(function(result)
                        {
                            console.log("grabbed")
                            var filename = yacApp.getNewProfileImageFilename();
                            console.log(filename)
                            console.log("hello")
                            result.saveToFile(filename)
                            yacApp.appUserUpdateProfile(fstname.displayText,
                                                        surname.displayText,
                                                        visible_name.displayText,
                                                        filename,
                                                        searchingExactlyAllowed,
                                                        searchingFuzzyAllowed,
                                                        function(message)
                                                        {
                                                            yacApp.goodMessage(qsTr("Profile saved"), null, null);
                                                            closeClicked()
                                                        },
                                                        function(message){
                                                            yacApp.badMessage(qsTr("Could not save Profile, please try again"), null, null)
                                                        })
                        }
                        )
                        return;
                    }
                    yacApp.appUserUpdateProfile(fstname.displayText,
                                                surname.displayText,
                                                visible_name.displayText,
                                                "",
                                                searchingExactlyAllowed,
                                                searchingFuzzyAllowed,
                                                function(message)
                                                {
                                                    yacApp.goodMessage(qsTr("Profile saved"), null, null);
                                                    closeClicked()
                                                },
                                                function(message){
                                                    yacApp.badMessage(qsTr("Could not save Profile, please try again"), null, null)
                                                })
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
