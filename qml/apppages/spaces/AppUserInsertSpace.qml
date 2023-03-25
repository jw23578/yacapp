import QtQuick 2.15
import "../../items"
import ".."
import "../.."

AppUserBasePage
{
    id: thePage
    showCloseButton: true
    signal spaceSaved();
    property string id: ""
    function show(id, name, access_code, automatic)
    {
        thePage.id = id
        theName.text = name
        theAccesscode.text = access_code
        theAutomatic.currentIndex = automatic ? 1 : 0
        thePage.visible = true
    }

    leftText: qsTr("Save")
    onLeftClicked:
    {
        if (theName.displayText == "")
        {
            yacApp.badMessage(qsTr("Please insert Name first"), theName, null)
            return
        }

        yacApp.appUserInsertOrUpdateSpace(thePage.id,
                                          theName.displayText,
                                          theAccesscode.displayText,
                                          theAutomatic.currentIndex == 1,
                                          function(message){spaceSaved()},
                                          function(message){console.log(message)})
    }
    YACPageColumn
    {
        centerVertical: false
        YACLineEditWithHeader
        {
            id: theName
            headerText: qsTr("Name")
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

        //        MultiSelectItem
        //        {
        //            id: theMultiSelectItem
        //            previousSelected: new Set(["1", "2", "3"])
        //            width: parent.width
        //            height: thePage.height - theName.height * 2
        //            model: thePage.visible ? AllRightsModel : null
        //            innerDelegateComponent: theInner
        //            Component
        //            {
        //                id: theInner
        //                Rectangle
        //                {
        //                    property var dataObject: null
        //                    YACText
        //                    {
        //                        anchors.centerIn: parent
        //                        text: dataObject.caption
        //                    }
        //                }
        //            }
        //        }
    }
}
