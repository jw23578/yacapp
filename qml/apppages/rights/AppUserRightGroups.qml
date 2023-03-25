import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage
{
    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Rightgroup")}]
    multiMenueButton.onClicked:
    {
        console.log("caption: " + caption)
        if (caption == qsTr("Add Rightgroup"))
        {
            appUserInsertRightGroup.show("", "", false)
        }
    }
    multiMenueButton.hide: appUserInsertRightGroup.visible
    ListView
    {
        id: theListview
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: buttonRow.top
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
                YACText
                {
                    text: rightgroup.automatic ? qsTr("New Users are automatically added to this Group") : qsTr("No Users will be automatically added")
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    yacApp.appUserFetchRightGroup(rightgroup.id,
                                                  function(message){
                                                      console.log(yacApp.currentFetchedIds)
                                                      appUserInsertRightGroup.show(rightgroup.id,
                                                                                   rightgroup.name,
                                                                                   rightgroup.automatic)
                                                  },
                                                  function(message){})
                }
            }
            YACButton
            {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: qsTr("Delete")
                onClicked:
                {
                    yacApp.yesNoQuestion(qsTr("Delete Rightgroup \"") + rightgroup.name + "\"", null,
                                         function()
                                         {
                                             yacApp.appUserDeleteRightGroup(rightgroup.id, function(message){},
                                             function(message)
                                             {
                                                 yacApp.badMessage(message, null, null)
                                             })
                                         },
                                         function()
                                         {
                                         })

                }
            }

        }

    }

    AppUserInsertRightGroup
    {
        id: appUserInsertRightGroup
        anchors.fill: parent
        visible: false
        onCloseClicked: appUserInsertRightGroup.visible = false
        onRightgroupSaved: appUserInsertRightGroup.visible = false
    }


}
