import QtQuick 2.15
import "../items"

Rectangle
{
    property string profileId: ""
    anchors.fill: parent
    signal closeClicked()
    id: messagePage

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
            width: messagePage.width - 40 // - messagePage.width / 30
            rotation: 180
            x: 20
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
                radiusTopLeft: other && !theColumn.model.prevSameTime
                radiusBottomRight: other && !theColumn.model.nextSameTime
                radiusTopRight: !other && !theColumn.model.prevSameTime
                radiusBottomLeft: !other && !theColumn.model.nextSameTime
                color: "lightgrey"
                property bool other: message.senderId != ""
                width: Math.max(contentText.contentWidth, messageDateTime.contentWidth, theColumn.model.message.qmlWidth, theColumn.model.nextQMLWidth, theColumn.model.prevQMLWidth) + radius
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
                        width: theColumn.width * 0.7
                        height: contentText.contentHeight
                        x: messageRectangle.radius / 2
                        YACText
                        {
                            id: contentText
                            text: message.content.trim()
                            width: parent.width
//                            x: (contentWidth < messageDateTime.contentWidth ? messageColumn.width - contentWidth - messageRectangle.radius / 2 : messageRectangle.radius / 2)
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
                        visible: !theColumn.model.nextSameTime
                        font.pixelSize: contentText.font.pixelSize * Constants.smallerTextFactor
                        id: messageDateTime
                        text: Helper.formatTime(message.sent)
                        x: messageRectangle.other ? messageRectangle.radius / 4 : messageColumn.width - width - messageRectangle.radius / 4
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
        anchors.top: parent.top
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
        anchors.top: parent.top
        text: qsTr("Close")
        onClicked: closeClicked()
    }
    YACTextEditWithBackground
    {
        id: theTextEdit
        anchors.left: parent.left
        anchors.right: sendButton.left
        anchors.bottom: parent.bottom
        wrapMode: Text.WordWrap
    }
    YACButton
    {
        id: sendButton
        text: ">>"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked:
        {
            if (theTextEdit.text == "")
            {
                return
            }
            yacApp.sendMessage(profileId, theTextEdit.text)
            theTextEdit.text = ""
        }
    }
}
