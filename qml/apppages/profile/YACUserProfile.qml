import QtQuick 2.15
import "../../items"
import ".."
import "qrc:/EMailPasswordFunctions.js" as EMailPasswordFunctions

AppUserBasePage
{

    id: theProfilePage
    signal logout()
    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Logout")},
        {caption: qsTr("Change Profileimage")}]
    multiMenueButton.onClicked:
    {
        Helper.jsLog("caption: " + caption)
        if (caption == qsTr("Logout"))
        {
            yacApp.appUserLogout()
            theProfilePage.logout()
        }
        if (caption == qsTr("Change Profileimage"))
        {
            yacApp.takePhoto(true, true, originalSizeProfileImage)
        }
    }

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
            YACLineEditWithHeader
            {
                id: theNewPassword
                headerText: qsTr("New Password")
                echoMode: TextInput.Password
            }

            YACButton
            {
                text: qsTr("Save")
                width: parent.width
                function saveProfile(imageFilename)
                {
                    var searchingExactlyAllowed = searchOption.currentIndex == 1
                    var searchingFuzzyAllowed = searchOption.currentIndex == 2
                    yacApp.appUserUpdateProfile(fstname.displayText,
                                                surname.displayText,
                                                visible_name.displayText,
                                                imageFilename,
                                                searchingExactlyAllowed,
                                                searchingFuzzyAllowed,
                                                theNewPassword.text,
                                                function(message)
                                                {
                                                    CPPQMLAppAndConfigurator.goodMessage(qsTr("Profile saved"), null, null);
                                                    closeClicked()
                                                },
                                                function(message){
                                                    CPPQMLAppAndConfigurator.badMessage(qsTr("Could not save Profile, please try again"), null, null)
                                                })
                }

                onClicked:
                {
                    if (theNewPassword.text != "")
                    {
                        if (!EMailPasswordFunctions.checkPassword(Helper, CPPQMLAppAndConfigurator, theNewPassword.text, theNewPassword))
                        {
                            return;
                        }
                    }
                    if (theProfilePage.imageChanged)
                    {
                        theProfilePage.imageChanged = false;
                        originalSizeProfileImage.grabToImage(function(result)
                        {
                            var filename = yacApp.getNewProfileImageFilename();
                            result.saveToFile(filename)
                            saveProfile(filename)
                        }
                        )
                        return;
                    }
                    saveProfile("")
                }
            }
        }
    }
}
