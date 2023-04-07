import QtQuick 2.15
import "../../items"
import ".."
import "../.."
import "../profile"

AppUserBasePage
{
    multiMenueButton.visible: false
    showCloseButton: true
    id: thePage
    signal rightgroupSaved();
    property string rightGroupID: ""
    function show(rightgroup)
    {
        if (rightgroup != null)
        {
            thePage.rightGroupID = rightgroup.id
            theName.text = rightgroup.name
            theAccessCode.text = rightgroup.access_code
            theAutomatic.currentIndex = rightgroup.automatic ? 1 : 0
            theRequestAllowed.currentIndex = rightgroup.request_allowed ? 1 : 0
            theVisibleForNonMembers.currentIndex = rightgroup.visible_for_non_members ? 1 : 0
            if (thePage.rightGroupID != "")
            {
                theMultiSelectItem.previousSelected.clear()
                for (var i = 0; i < yacApp.currentFetchedIds.length; ++i)
                {
                    theMultiSelectItem.previousSelected.add(yacApp.currentFetchedIds[i])
                }
            }
        }
        else
        {
            thePage.rightGroupID = ""
            theName.text = ""
            theAccessCode.text = ""
            theAutomatic.currentIndex = 0
            theRequestAllowed.currentIndex = 0
            theVisibleForNonMembers.currentIndex = 0
        }

        thePage.visible = true
    }

    YACPageColumn
    {
        id: theColumn
        centerVertical: true
        YACLineEditWithHeader
        {
            id: theName
            headerText: qsTr("RightGroup-Name")
        }
        YACLineEditWithHeader
        {
            id: theAccessCode
            headerText: qsTr("Accesscode")
        }

        YACComboBoxWithHeader
        {
            id: theAutomatic
            headerText: qsTr("Automatism")
            model: [qsTr("Do not add new Users automatically"), qsTr("Add new User automatically to this Rightgroup")]
        }
        YACComboBoxWithHeader
        {
            id: theRequestAllowed
            headerText: qsTr("Request allowed")
            model: [qsTr("Access-Request by others not allowed"), qsTr("Access-Request by others allowed")]
        }
        YACComboBoxWithHeader
        {
            id: theVisibleForNonMembers
            headerText: qsTr("Visibility")
            model: [qsTr("Not visible for non-members"), qsTr("Visible for non-members")]
        }
    }
    MultiSelectItem
    {
        id: theMultiSelectItem
        width: theColumn.width
        anchors.top: theColumn.bottom
        anchors.bottom: buttonRow.top
        anchors.horizontalCenter: parent.horizontalCenter
        model: thePage.visible ? AllRightsModel : null
        innerDelegateComponent: theInner
        Component
        {
            id: theInner
            Rectangle
            {
                property var dataObject: null
                YACText
                {
                    anchors.fill: parent
                    anchors.margins: Constants.defaultMargin
                    text: dataObject.caption
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
    leftText: qsTr("Save")
    onLeftClicked:
    {
        if (theName.displayText == "")
        {
            yacApp.badMessage(qsTr("Please insert Name first"), theName, null)
            return
        }
        yacApp.appUserInsertOrUpdateRightGroup(thePage.rightGroupID,
                                               theName.displayText,
                                               theAutomatic.currentIndex == 1,
                                               theAccessCode.text,
                                               theRequestAllowed.currentIndex == 1,
                                               theVisibleForNonMembers.currentIndex == 1,
                                               function(message){rightgroupSaved()},
                                               function(message){console.log(message)})
    }
}
