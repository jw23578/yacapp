import QtQuick 2.15
import "../../items"
import ".."
import "../.."

AppUserBasePage
{
    id: thePage
    showCloseButton: true
    signal spaceSaved();
    property string spaceID: ""
    function show(space)
    {
        if (space != null)
        {
            thePage.spaceID = space.id
            theName.text = space.name
            theAccesscode.text = space.access_code
            theAutomatic.currentIndex = space.automatic ? 1 : 0
            theRequestAllowed.currentIndex = space.request_allowed ? 1 : 0
        }
        else
        {
            thePage.spaceID = ""
            theName.text = ""
            theAccesscode.text = ""
            theAutomatic.currentIndex = 0
            theRequestAllowed.currentIndex = 0
        }

        thePage.visible = true
    }

    leftText: qsTr("Save")
    onLeftClicked:
    {
        if (theName.displayText == "")
        {
            CPPQMLAppAndConfigurator.badMessage(qsTr("Please insert Name first"), theName, null)
            return
        }

        yacApp.appUserInsertOrUpdateSpace(thePage.spaceID,
                                          theName.displayText,
                                          theAccesscode.displayText,
                                          theAutomatic.currentIndex == 1,
                                          theRequestAllowed.currentIndex == 1,
                                          function(message){
                                              spaceSaved()
                                          },
                                          function(message){Helper.jsLog(message)})
    }
    YACPageColumn
    {
        centerVertical: false
        YACLineEditWithHeader
        {
            id: theName
            headerText: qsTr("Space-Name")
        }
        YACLineEditWithHeader
        {
            id: theAccesscode
            headerText: qsTr("Accesscode")
        }

        YACComboBoxWithHeader
        {
            id: theAutomatic
            headerText: qsTr("Automatism")
            model: [qsTr("Do not add new Users automatically"), qsTr("Add new User automatically to this Space")]
        }
        YACComboBoxWithHeader
        {
            id: theRequestAllowed
            headerText: qsTr("Request allowed")
            model: [qsTr("Access-Request by others not allowed"), qsTr("Access-Request by others allowed")]
        }
    }
}
