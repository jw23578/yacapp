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
        anchors.bottom: buttonRow.top
        anchors.margins: Constants.defaultMargin
        reuseItems: true
        model: SpacesModel
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
                    text: space.name
                }
                YACText
                {
                    text: space.automatic ? qsTr("New Users are automatically added to this Space") : qsTr("No Users will be automatically added")
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    if (space.creater_id != yacApp.appUserConfig.id)
                    {
                        return
                    }

                    yacApp.appUserFetchSpace(space.id,
                                             function(message){
                                                 console.log(yacApp.currentFetchedIds)
                                                 appUserInsertSpace.show(space.id,
                                                                         space.name,
                                                                         space.access_code,
                                                                         space.automatic)
                                             },
                                             function(message){})
                }
            }
            YACText
            {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: qsTr("You are the owner of this Space")
                visible: space.creater_id == yacApp.appUserConfig.id
            }
            YACText
            {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                text: qsTr("Requested, not yet granted")
                visible: space.requested
            }

            YACButton
            {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                visible: space.creater_id != yacApp.appUserConfig.id && !space.requested
                text: space.member ? qsTr("Leave") :
                                     space.denied ? qsTr("Request again, you were denied") :
                                                    qsTr("Request Membership")
                onClicked:
                {
                    if (space.member)
                    {
                        yacApp.notYetImplemented()
                        return
                        yacApp.yesNoQuestion("Really leave this space?", null
                                             , function(){yacApp.notYetImplemented()}
                                             , function(){yacApp.notYetImplemented()})
                        return
                    }
                    if (space.denied)
                    {
                        yacApp.notYetImplemented()
                        return
                    }
                    appUserRequestSpace.show(space)
                }
            }

            YACButton
            {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: qsTr("Delete")
                visible: space.creater_id == yacApp.appUserConfig.id
                onClicked:
                {
                    yacApp.yesNoQuestion(qsTr("Delete Space \"") + space.name + "\"", null,
                                         function()
                                         {
                                             yacApp.appUserDeleteSpace(space.id, function(message){},
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

    leftText: qsTr("Add Space")
    onLeftClicked: appUserInsertSpace.show("", "", "", false)

    AppUserInsertSpace
    {
        id: appUserInsertSpace
        anchors.fill: parent
        visible: false
        onCloseClicked: appUserInsertSpace.visible = false
        onSpaceSaved: appUserInsertSpace.visible = false
    }
    AppUserRequestSpace
    {
        id: appUserRequestSpace
        visible: false
        onCloseClicked: visible = false
    }

}
