import QtQuick 2.15
import "../../items"

Rectangle
{
    MouseArea
    {
        anchors.fill: parent
    }
    property var profile: null
    anchors.fill: parent
    signal closeClicked()
    id: messagePage
    Rectangle
    {
        id: profileHeader
        height: 50
        width: parent.width

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

    Component
    {
        id: messageDelegate

        Column
        {
            function updateFunction(widthToSet)
            {
                if (widthToSet <= message.qmlWidth)
                {
                    return;
                }

                message.qmlWidth = widthToSet;
                if (theColumn.model.nextSameTime)
                {
                    theListview.itemAtIndex(theColumn.model.index - 1).item.updateFunction(message.qmlWidth)
                }
            }
            id: theColumn
            width: messagePage.width - 2 * x
            rotation: 180
            x: Constants.defaultMargin
            //x: -20 //  (messagePage.width - width) / 2
            property var model: null
            property var message: model.message

            Text
            {
                height: visible ? contentHeight : 0
                anchors.horizontalCenter: parent.horizontalCenter
                visible: theColumn.model.daySwitch
                text: Helper.formatDate(message.sent)
            }
            YACRoundedRectangle
            {
                id: messageRectangle
                radius: Constants.radius
                radiusTopLeft: other && !theColumn.model.prevSameTime && theListview.count
                radiusBottomRight: other && !theColumn.model.nextSameTime && theListview.count
                radiusTopRight: !other && !theColumn.model.prevSameTime && theListview.count
                radiusBottomLeft: !other && !theColumn.model.nextSameTime && theListview.count
                color: other ? Qt.darker("lightgrey", 1.3) : "lightgrey"
                property bool other: message.senderId != yacApp.appUserConfig.id
                property int nettoWidth: Math.max(contentText.contentWidth, messageDateTime.contentWidth, theColumn.model.message.qmlWidth, theColumn.model.nextQMLWidth, theColumn.model.prevQMLWidth)
                width: nettoWidth + radius
                height: messageColumn.height
                x: other ? 0 : theColumn.width - width
                Column
                {
                    id: messageColumn
                    width: parent.width
                    Item
                    {
                        width: 1
                        height: messageRectangle.radius / 2
                    }

                    Item
                    {
                        id: theMessageItem
                        width: theColumn.width * 0.7
                        height: contentText.contentHeight
                        x: messageRectangle.radius / 2
                        YACText
                        {
                            id: contentText
                            text: message.content.trim()
                            width: parent.width
                            x: contentWidth < messageDateTime.contentWidth ? messageRectangle.nettoWidth - contentWidth : 0
                            onContentWidthChanged:
                            {
                                if (theColumn.message == null)
                                {
                                    return
                                }
                                if (theColumn.model.message.qmlWidth >= contentWidth)
                                {
                                    return
                                }
                                contentWidth = Math.max(theColumn.model.prevQMLWidth, theColumn.model.nextQMLWidth, contentWidth)
                                theColumn.model.message.qmlWidth = contentWidth
                                if (theColumn.model.nextSameTime)
                                {
                                    theListview.itemAtIndex(theColumn.model.index - 1).item.updateFunction(contentWidth)
                                }
                            }
                        }
                    }
                    Item
                    {
                        width: 1
                        height: messageRectangle.radius / 4
                    }
                    YACText
                    {
                        visible: !theColumn.model.nextSameTime && theListview.count
                        font.pixelSize: contentText.font.pixelSize * Constants.smallerTextFactor
                        id: messageDateTime
                        text: Helper.formatTime(message.sent)
                        x: messageRectangle.other ? theMessageItem.x : messageRectangle.width - contentWidth - messageRectangle.radius / 2
//                        x: messageRectangle.other ? messageRectangle.radius / 4 : messageColumn.width - width - messageRectangle.radius / 4
                    }
                    Item
                    {
                        width: 1
                        height: messageRectangle.radius / 4
                    }
                }
            }
        }
    }


    ListView
    {
        id: theListview
        rotation: 180
        clip: true
        anchors.top: profileHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: theTextEdit.top
        reuseItems: true
        model: MessagesModel
        spacing: 1
        delegate: Loader
        {
            sourceComponent: messageDelegate // message.senderID = "" ? myMessageDelegate : otherMessageDelegate
            onLoaded:
            {
                item.model = model
            }
        }
    }
    YACButton
    {
        id: closeButton
        anchors.top: profileHeader.bottom
        text: qsTr("Close")
        onClicked: closeClicked()
    }
    YACTextEditWithBackground
    {
        id: theTextEdit
        focus: false
        anchors.left: parent.left
        anchors.right: sendButton.left
        anchors.leftMargin: Constants.defaultMargin
        anchors.bottom: parent.bottom
        wrapMode: Text.WordWrap
        font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
        textFocus: false
    }
    YACButton
    {
        id: sendButton
        text: ">>"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked:
        {
            var toSend = theTextEdit.text + theTextEdit.preeditText
            if (toSend == "")
            {
                return
            }
            yacApp.sendMessage(profile.id, toSend)
            theTextEdit.text = ""
        }
    }
}
