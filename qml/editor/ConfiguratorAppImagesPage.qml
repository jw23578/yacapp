import QtQuick 2.15
import "../items"

Rectangle
{
    id: appImages
    anchors.fill: parent
    visible: false
    ListView
    {
        model: yacApp.globalConfig.appImages()
        id: imagesListView
        width: parent.width / 2 + 2
        height: parent.height
        property int imageWidth: width - 2
        property int imageHeight: imageWidth * 100 / 266
        spacing: 1
        add: Transition {
            NumberAnimation
            {
                properties: "opacity"
                from: 0
                to: 1
            }
        }
        remove: Transition {
            NumberAnimation
            {
                properties: "opacity"
                from: 1
                to: 0
            }
        }
        displaced: Transition {
            NumberAnimation
            {
                properties: "y"
            }
        }

        delegate: Rectangle
        {
            width: imagesListView.imageWidth + 2
            height: imagesListView.imageHeight + 2
            border.color: "black"
            border.width: 1
            YACImage
            {
                anchors.centerIn: parent
                width: imagesListView.imageWidth
                height: imagesListView.imageHeight
                source: image.fileUrl
            }
            YACButton
            {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: qsTr("Delete")
                onClicked: yacApp.globalConfig.appImages().deleteById(image.id)
            }
        }
    }
    Column
    {
        anchors.left: imagesListView.right
        anchors.right: parent.right
        YACButton
        {
            text: qsTr("Add Image")
            onClicked: configurator.addImageSignal(function(fileUrls)
            {
                for (var i = 0; i < fileUrls.length; ++i)
                {
                    configurator.addImageFile(fileUrls[i])
                }

            })
        }
        YACButton
        {
            text: qsTr("Close")
            onClicked:
            {
                yacApp.saveCurrentProject()
                appImages.visible = false
            }
        }
    }

}
