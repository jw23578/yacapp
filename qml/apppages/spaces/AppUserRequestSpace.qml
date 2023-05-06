import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
    showCloseButton: true
    property var space: null
    function show(theSpace)
    {
        space = theSpace
        visible = true
    }

    property string space_id: ""
    YACPageColumn
    {
        centerVertical: false
        YACText
        {
            id: theName
            text: space.name
        }
        YACLineEditWithHeader
        {
            id: theAccesscode
            headerText: qsTr("Accesscode")
        }
        YACButton
        {
            width: parent.width
            text: qsTr("Request by Code")
            onClicked:
            {
                if (theAccesscode.displayText == "")
                {
                    CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter Accesscode first"), theAccesscode, null)
                    return
                }
                CPPQMLAppAndConfigurator.notYetImplemented()
            }
        }
        YACButton
        {
            width: parent.width
            text: qsTr("Request by Admin")
            onClicked:
            {
                yacApp.appUserRequestSpaceAccess(space.id
                                                 , function(message)
                                                 {
                                                     CPPQMLAppAndConfigurator.goodMessage(qsTr("Space access requested, Space owner is informed"), null, null)
                                                     space.requested = true
                                                     closeClicked()
                                                 }
                                                 , function(message)
                                                 {
                                                     CPPQMLAppAndConfigurator.badMessage(qsTr("Could not request Space-Access, please try again later"), null, null)
                                                 })
            }
        }
    }
}
