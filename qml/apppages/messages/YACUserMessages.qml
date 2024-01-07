import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage2
{
    enableBack: true
    property var profile: null
    id: messagePage
    property color otherColor: Qt.darker("lightgrey", 1.3)
    property color myColor: "lightgrey"
    YACRoundedRectangle
    {
        id: profileHeader
        height: 50
        width: parent.width * 3 / 4
        radiusTopLeft: true
        radiusBottomRight: true
        color: messagePage.otherColor
        radius: Constants.radius

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
        MouseArea
        {
            anchors.fill: parent
            onClicked: yacApp.fetchMessageUpdates()
        }
    }

    Component
    {
        id: messageDelegate

        Item
        {
            id: theMessageItem
            width: theColumn.width
            height: theColumn.height
            property var model: null
            property var message: model.message
            property var parsedMessage: JSON.parse(message.content)
            property var messageText: parsedMessage.text
            property bool other: message.senderId != yacApp.appUserConfig.id

            Column
            {
                function updateFunction(widthToSet)
                {
                    if (widthToSet <= theMessageItem.message.qmlWidth)
                    {
                        return;
                    }

                    theMessageItem.message.qmlWidth = widthToSet;
                    if (theMessageItem.model.nextSameTime)
                    {
                        theListview.itemAtIndex(theMessageItem.model.index - 1).item.updateFunction(theMessageItem.message.qmlWidth)
                    }
                }
                id: theColumn
                width: messagePage.width - 2 * x
                rotation: 180
                x: Constants.defaultMargin
                //x: -20 //  (messagePage.width - width) / 2

                Text
                {
                    height: visible ? contentHeight : 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: theMessageItem.model.daySwitch
                    text: Helper.formatDate(theMessageItem.message.sent)
                }
                Repeater
                {
                    model: parsedMessage.images.length
                    id: imagesRepeater
                    property int imagesRectWidth: parent.width * 3 / 4
                    YACRoundedRectangle
                    {
                        id: theImageRectangle
                        property var tracker: yacApp.tracker(theMessageItem.parsedMessage.images[0].imageid)
                        radius: Constants.radius
                        radiusTopLeft: other && !theMessageItem.model.prevSameTime && theListview.count
                        radiusTopRight: !other && !theMessageItem.model.prevSameTime && theListview.count
                        color: other ? messagePage.otherColor : messagePage.myColor
                        width: imagesRepeater.imagesRectWidth
                        height: tracker.active ? 100 : imagesRepeater.imagesRectWidth / theMessageItem.parsedMessage.images[0].width * theMessageItem.parsedMessage.images[0].height
                        x: theMessageItem.other ? 0 : theColumn.width - width
                        YACRoundedImage
                        {
                            id: theImage
                            radius: Constants.radius
                            circled: false
                            anchors.centerIn: parent
                            width: parent.width - 2
                            height: parent.height - 2
                            source: "image://async/message/" + theMessageItem.parsedMessage.images[0].imageid
                            Rectangle
                            {
                                visible: theImageRectangle.tracker.active
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                                height: 1
                                color: "red"
                                Rectangle
                                {
                                    color: "white"
                                    width: parent.width * theImageRectangle.tracker.percentState / 100
                                    height: 1
                                }
                            }
                        }
                    }
                }

                YACRoundedRectangle
                {
                    id: messageRectangle
                    radius: Constants.radius
                    radiusTopLeft: theMessageItem.other && !theMessageItem.model.prevSameTime && theListview.count && imagesRepeater.count == 0
                    radiusTopRight: !theMessageItem.other && !theMessageItem.model.prevSameTime && theListview.count && imagesRepeater.count == 0
                    radiusBottomRight: theMessageItem.other && !theMessageItem.model.nextSameTime && theListview.count
                    radiusBottomLeft: !theMessageItem.other && !theMessageItem.model.nextSameTime && theListview.count
                    color: theMessageItem.other ? messagePage.otherColor : messagePage.myColor
                    property int nettoWidth: Math.max(contentText.contentWidth
                                                      , messageDateTime.contentWidth
                                                      , theMessageItem.model.message.qmlWidth
                                                      , theMessageItem.model.nextQMLWidth
                                                      , theMessageItem.model.prevQMLWidth
                                                      , imagesRepeater.count > 0 ? imagesRepeater.imagesRectWidth - radius : 0)
                    width: nettoWidth + radius
                    height: messageColumn.height
                    x: theMessageItem.other ? 0 : theColumn.width - width
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
                            id: theInnerMessageItem
                            width: theColumn.width * 0.7
                            height: contentText.contentHeight
                            x: messageRectangle.radius / 2
                            YACText
                            {
                                id: contentText
                                text: messageText
                                width: parent.width
                                property double theWidth: 0
                                x: theWidth < messageDateTime.contentWidth ? messageRectangle.nettoWidth - contentWidth : 0
                                function setWidth()
                                {
                                    if (theMessageItem.message == null)
                                    {
                                        return
                                    }
                                    if (theMessageItem.model.message.qmlWidth >= contentWidth)
                                    {
                                        return
                                    }
                                    theWidth = Math.max(theMessageItem.model.prevQMLWidth, theMessageItem.model.nextQMLWidth, contentWidth, messageDateTime.contentWidth)
                                    theMessageItem.model.message.qmlWidth = theWidth
                                    if (theMessageItem.model.nextSameTime)
                                    {
                                        theListview.itemAtIndex(theMessageItem.model.index - 1).item.updateFunction(theWidth)
                                    }
                                }
                                Connections
                                {
                                    target: theListview
                                    function onCountChanged(count)
                                    {
                                        contentText.setWidth()
                                    }
                                }

                                onContentWidthChanged:
                                {
                                    setWidth()
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
                            visible: !theMessageItem.model.nextSameTime && theListview.count
                            font.pixelSize: contentText.font.pixelSize * Constants.smallerTextFactor
                            id: messageDateTime
                            text: Helper.formatTime(message.sent)
                            x: messageRectangle.other ? theInnerMessageItem.x : messageRectangle.width - contentWidth - messageRectangle.radius / 2
                            //                        x: messageRectangle.other ? messageRectangle.radius / 4 : messageColumn.width - width - messageRectangle.radius / 4
                        }
                        Item
                        {
                            width: 1
                            height: messageRectangle.radius / 4
                        }
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onPressAndHold:
                        {
                            if (theMessageItem.other)
                            {
                                return
                            }
                            deleteItem.visible = true
                        }
                        onClicked: deleteItem.visible = false
                    }
                }
            }
            Item
            {
                id: deleteItem
                visible: false
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: theColumn.width * 0.1
                height: width
                Image
                {
                    mipmap: true
                    anchors.fill: parent
                    source: "qrc:/images/images/delete.svg"
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        yacApp.deleteMessage(theMessageItem.message.id)
                        deleteItem.visible = false
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
        anchors.bottom: bottomRectangle.top
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

    Image {
        id: realSizeImage
        visible: false
        width: sourceSize.width
        height: sourceSize.height
        onSourceChanged: imageToSend.source = source
    }

    Image
    {
        id: imageToSend
        visible: source != ""
        fillMode: Image.PreserveAspectFit
        mipmap: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Constants.radius / 4
        anchors.bottom: theTextEdit.top
    }

    YACTextEditWithBackground
    {
        id: theTextEdit
        radius: Constants.radius / 2
        border.width: 0
        focus: true
        anchors.left: backImage.right
        anchors.right: cameraButton.left
        anchors.margins: Constants.radius / 4
        anchors.bottom: bottomRectangle.bottom
        wrapMode: Text.WordWrap
        font.pixelSize: Constants.defaultFontPixelSize * Constants.x4largerTextFactor
        textFocus: false
        Behavior on height {
            NumberAnimation {
                duration: Constants.fastAnimationDuration
            }
        }
        onHeightChanged: bottomRectangle.height = theTextEdit.height + Constants.radius / 2 + (imageToSend.visible ? imageToSend.height + Constants.radius / 4 : 0)
    }



    YACImage
    {
        id: cameraButton
        anchors.right: sendButton.left
        height: heightInfoTextEdit.height
        width: theTextEdit.text == "" ? height : 0
        Behavior on width
        {

            NumberAnimation
            {
                duration: Constants.fastAnimationDuration
            }
        }

        anchors.margins: Constants.radius / 4
        anchors.bottom: parent.bottom
        source: "qrc:/images/images/camera.svg"
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                yacApp.goTakePhoto(false, false, realSizeImage)
            }
        }
    }

    YACImage
    {
        id: sendButton
        anchors.right: parent.right
        height: heightInfoTextEdit.height
        width: height
        anchors.margins: Constants.radius / 4
        anchors.bottom: parent.bottom
        source: "qrc:/images/images/send-arrow.svg"
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                var toSend = theTextEdit.text + theTextEdit.preeditText
                if (toSend == "" && imageToSend.source == "")
                {
                    return
                }
                var images = []
                var imagesWidths = []
                var imagesHeights = []
                if (realSizeImage.source != "")
                {

                    realSizeImage.grabToImage(function(result) {
                        var filename = yacApp.getCacheImageFilename()
                        console.log(filename)
                        if (result.saveToFile(filename))
                        {
                            images.push(yacApp.storeMessageImage(filename, realSizeImage.width, realSizeImage.height));
                            imagesWidths.push(realSizeImage.width)
                            imagesHeights.push(realSizeImage.height)
                            yacApp.sendMessage(profile.id, toSend, images, imagesWidths, imagesHeights)
                            imageToSend.source = ""
                            theTextEdit.text = ""
                            realSizeImage.source = ""
                        }
                    });
                }
                else
                {
                    yacApp.sendMessage(profile.id, toSend, images, imagesWidths, imagesHeights)
                    imageToSend.source = ""
                    theTextEdit.text = ""
                }
            }
        }
    }
}
