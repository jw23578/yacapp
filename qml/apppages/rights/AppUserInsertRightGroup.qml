import QtQuick 2.15
import "../../items"
import ".."
import "../.."

AppUserBasePage
{
    showCloseButton: true
    id: thePage
    signal rightgroupSaved();
    property string id: ""
    function show(id, name, automatic)
    {
        thePage.id = id
        theName.text = name
        theAutomatic.currentIndex = automatic ? 1 : 0
        if (id != "")
        {
            theMultiSelectItem.previousSelected.clear()
            for (var i = 0; i < yacApp.currentFetchedIds.length; ++i)
            {
                theMultiSelectItem.previousSelected.add(yacApp.currentFetchedIds[i])
            }
        }
        thePage.visible = true
    }

    YACPageColumn
    {
        id: theColumn
        centerVertical: false
        YACLineEditWithHeader
        {
            id: theName
            headerText: qsTr("RightGroup-Name")
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
        yacApp.appUserInsertOrUpdateRightGroup(thePage.id,
                                                          theName.displayText,
                                                          theAutomatic.currentIndex == 1,
                                                          function(message){rightgroupSaved()},
                                                          function(message){console.log(message)})
    }
}
