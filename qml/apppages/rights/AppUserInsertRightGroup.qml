import QtQuick 2.15
import "../../items"
import ".."
import "../.."

AppUserBasePage
{
    multiMenueButton.visible: false
    showCloseButton: true
    id: thePage
    signal rightgroupSaved();
    property string rightGroupID: ""
    function show(rightgroup)
    {
        thePage.rightGroupID = ""
        theName.text = ""
        theAccessCode.text = ""
        theAutomatic.currentIndex = 0
        if (rightgroup != null)
        {
            thePage.rightGroupID = rightgroup.id
            theName.text = rightgroup.name
            theAccessCode.text = rightgroup.access_code
            theAutomatic.currentIndex = rightgroup.automatic ? 1 : 0
            if (thePage.rightGroupID != "")
            {
                theMultiSelectItem.previousSelected.clear()
                for (var i = 0; i < yacApp.currentFetchedIds.length; ++i)
                {
                    theMultiSelectItem.previousSelected.add(yacApp.currentFetchedIds[i])
                }
            }
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
                                               function(message){rightgroupSaved()},
                                               function(message){console.log(message)})
    }
}
