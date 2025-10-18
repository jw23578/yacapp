import QtQuick 2.15
import "../../items"
import ".."
import "../profile"

AppUserBasePage
{
    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Rightgroup")}]
    multiMenueButton.onClicked:
    {
        Helper.jsLog("caption: " + caption)
        if (caption == qsTr("Add Rightgroup"))
        {
            appUserInsertRightGroup.show(null)
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
            height: theColumn.height
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                    yacApp.appUserFetchRightGroupRights(rightgroup.id,
                                                        function(message)
                                                        {
                                                            appUserInsertRightGroup.show(rightgroup)
                                                        }, function(message)
                                                        {
                                                            CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                                        })
            }
            Column
            {
                id: theColumn
                width: parent.width
                YACText
                {
                    text: rightgroup.name
                }
                YACText
                {
                    text: rightgroup.automatic ? qsTr("New Users are automatically added to this Group") : qsTr("No Users will be automatically added")
                }
                YACText
                {
                    text: qsTr("You are the owner of this Rightgroup")
                    visible: rightgroup.creater_id == yacApp.appUserConfig.id
                }
                Row
                {
                    YACButton
                    {
                        z: 1
                        text: qsTr("Edit Member")
                        onClicked:
                        {
                            yacApp.appUserFetchRightGroupMember(rightgroup.id,
                                                                function(message)
                                                                {
                                                                    theAppUserProfileList.right_group_id = rightgroup.id
                                                                    theAppUserProfileList.show()

                                                                },
                                                                function(message)
                                                                {
                                                                }
                                                                )
                        }

                    }
                    YACButton
                    {
                        text: qsTr("Delete")
                        onClicked:
                        {
                            Helper.jsLog("hello hjsdfhsdlkjfhlkjsahlka")
                            CPPQMLAppAndConfigurator.yesNoQuestion(qsTr("Delete Rightgroup \"") + rightgroup.name + "\"", null,
                                                 function()
                                                 {
                                                     yacApp.appUserDeleteRightGroup(rightgroup.id, function(message){},
                                                     function(message)
                                                     {
                                                         CPPQMLAppAndConfigurator.badMessage(message, null, null)
                                                     })
                                                 },
                                                 function()
                                                 {
                                                 })

                        }
                    }
                }
            }
        }

    }


    AppUserProfileList
    {
        z: 1
        property string right_group_id: ""
        id: theAppUserProfileList
        visible: false
        emptyText.text: qsTr("No Member yet")
        onCloseClicked:
        {
            for (var i = 0; i < newSelected.length; ++i)
            {
                yacApp.appUserInsertOrUpdateRightGroup2AppUser("",
                                                               right_group_id,
                                                               newSelected[i],
                                                               new Date(),
                                                               new Date(),
                                                               yacApp.appUserConfig.id,
                                                               Constants.timePointPostgreSqlNull,
                                                               "",
                                                               function(message){},
                                                               function(message){})
            }

            visible = false
        }
    }

    AppUserInsertRightGroup
    {
        z: 1
        id: appUserInsertRightGroup
        anchors.fill: parent
        visible: false
        onCloseClicked: appUserInsertRightGroup.visible = false
        onRightgroupSaved: appUserInsertRightGroup.visible = false
    }


}
