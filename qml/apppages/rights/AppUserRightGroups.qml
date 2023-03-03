import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
    ListView
    {
        id: theListview
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: closeButton.top
        anchors.margins: Constants.defaultMargin
        reuseItems: true
        model: RightGroupsModel
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }
        remove: Transition {
            NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
        }
        spacing: 1
        delegate: Rectangle
        {
            color: index % 2 ? "silver" : "lightgrey"
            width: parent.width
            height: 100
            Column
            {
                width: parent.width
                YACText
                {
                    text: rightgroup.name
                }
            }
        }

    }

    YACButton
    {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        text: qsTr("Add Rightgroup")
        onClicked: theLoader.sourceComponent = insertRightGroupComponent
    }
    Component
    {
        id: insertRightGroupComponent
        AppUserInsertRightGroup
        {
            onCloseClicked: theLoader.sourceComponent = null
            onRightgroupSaved: theLoader.sourceComponent = null
        }
    }
    Loader
    {
        anchors.fill: parent
        id: theLoader
    }

}
