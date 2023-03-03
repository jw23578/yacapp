import QtQuick 2.15
import "../../items"
import ".."
import "../.."

AppUserBasePage
{
    id: thePage
    signal rightgroupSaved();
    YACPageColumn
    {
        YACLineEditWithHeader
        {
            id: theName
            headerText: qsTr("Name")
        }
        MultiSelectItem
        {
            previousSelected: new Set(["1", "2", "3"])
            width: parent.width
            height: thePage.height - theName.height * 2
            model: AllRightsModel
            innerDelegateComponent: theInner
            Component
            {
                id: theInner
                Rectangle
                {
                    property var dataObject: null
                    YACText
                    {
                        anchors.centerIn: parent
                        text: dataObject.caption
                    }
                }
            }
        }
    }
    YACButton
    {
        text: qsTr("Save")
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        onClicked:
        {
            yacApp.appUserInsertRightGroup(theName.displayText,
                                           function(message){rightgroupSaved()},
                                           function(message){console.log(message)})
        }
    }
}
