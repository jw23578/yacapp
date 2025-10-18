import QtQuick 2.15
import "../../items"
import ".."
import "../messages"

AppUserBasePage
{
    id: theAppUserProfileList

    function show()
    {
        lastId = ""
        undoAction = ""
        newSelected = []
        newRemoved = []
        visible = true
    }

    property string undoAddString: qsTr("Undo add")
    property string lastId: ""
    property string undoAction: ""

    property var newSelected: []
    function add(id)
    {
        if (newSelected.indexOf(id) != -1)
        {
            CPPQMLAppAndConfigurator.badMessage(qsTr("Already selected"), null, null)
            return
        }
        var index = newRemoved.indexOf(id)
        if (index == -1)
        {
            newSelected.push(id)
        }
        else
        {
            newSelected.slice(index, 1)
        }
        yacApp.addProfileToSelectedProfiles(id)
        theLoader.sourceComponent = null
        Helper.jsLog("newSelected: " + newSelected)
        Helper.jsLog("newRemoved: " + newRemoved)

        undoAction = undoAddString
        lastId = id;
    }

    property var newRemoved: []

    function remove(id)
    {
        if (newRemoved.indexOf(id) != -1)
        {
            // already removed, should never happen
            return
        }
        var index = newSelected.indexOf(id)
        if (index == -1)
        {
            newRemoved.push(id)
        }
        else
        {
            newSelected.splice(index, 1)
        }
        undoButton.visible = true
        yacApp.removeFromSelectedProfiles(id)
        Helper.jsLog("newSelected: " + newSelected)
        Helper.jsLog("newRemoved: " + newRemoved)

        undoAction = qsTr("Undo Remove")
        lastId = id;
    }

    property alias emptyText: theEmptyText
    onVisibleChanged: visible ? multiMenueButton.moveIn() : multiMenueButton.moveOut()
    showCloseButton: true
    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Profile")}]
    multiMenueButton.onClicked:
    {
        Helper.jsLog("caption: " + caption)
        if (caption == qsTr("Add Profile"))
        {
            theLoader.sourceComponent = searchProfiles
        }
    }

    ListView
    {
        id: listView
        clip: true
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: SelectedProfilesModel
        delegate: Row
        {
            height: 50
            Rectangle
            {
                width: listView.width - height
                height: 50
                YACRoundedImage
                {
                    height: parent.height
                    width: height
                    source: "image://async/profileImage/" + profile.profileImageId
                }

                Text
                {
                    anchors.centerIn: parent
                    text: profile.visibleName
                }
            }
            YACButton
            {
                height: parent.height
                width: height
                text: "-"
                onClicked: remove(profile.id)
            }
        }
        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 300 }
        }
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 300 }
        }
    }
    YACText
    {
        id: theEmptyText
        anchors.centerIn: parent
        visible: listView.count == 0
    }

    Loader
    {
        anchors.fill: parent
        z: 1
        id: theLoader
    }

    Component
    {
        id: searchProfiles
        YACUserSearchProfiles
        {
            onProfileSelected: add(id)
            onCloseClicked: theLoader.sourceComponent = null
        }
    }

    YACButton
    {
        id: undoButton
        visible: undoAction != ""
        text: undoAction
        onClicked:
        {
            if (undoAction == undoAddString)
            {
                remove(lastId)
            }
            else
            {
                add(lastId)
            }
        }
        anchors.bottom: buttonRow.top
        anchors.right: parent.right
    }
}
