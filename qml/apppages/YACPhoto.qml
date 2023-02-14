import QtQuick 2.15
import QtMultimedia 5.15
import QtGraphicalEffects 1.15

import "../items"

Rectangle
{
    id: photoPage
    signal abortClicked()
    signal okClicked(var image)
    property bool squared: false
    property bool circled: true
    anchors.fill: parent
    Camera {
        id: camera

        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

        flash.mode: Camera.FlashRedEyeReduction

        imageCapture {
            onImageCaptured: {
                originalSizeImage.source = preview
                photoPreview.visible = true
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }

    Component
    {
        id: circleMask
        OpacityMask
        {
            maskSource: Item
            {
                width: videoOutput.width
                height: videoOutput.height
                Rectangle
                {
                    anchors.centerIn: parent
                    width: videoOutput.width
                    height: videoOutput.height
                    radius: Math.min(width, height) / 2
                }
            }
        }
    }

    VideoOutput
    {
        property alias squared: photoPage.squared
        property alias circled: photoPage.circled
        property double sizeFactor: 0.6
        id: videoOutput
        source: camera
        anchors.centerIn: parent
        fillMode: (squared || circled) ? Image.PreserveAspectCrop : Image.PreserveAspectFit
        width: ((squared || circled) ? Math.min(photoPage.width, photoPage.height) : photoPage.width) * sizeFactor
        height: ((squared || circled) ? Math.min(photoPage.width, photoPage.height) : photoPage.width) * sizeFactor
        focus : visible // to receive focus and capture key events when visible
        layer.enabled: circled
        layer.effect: circleMask
        autoOrientation: true
    }

    Image
    {
        visible: false
        anchors.fill: videoOutput
        id: photoPreview
        fillMode: videoOutput.fillMode
        layer.enabled: videoOutput.layer.enabled
        layer.effect: circleMask
        autoTransform: true
    }
    Image
    {
        property alias squared: photoPage.squared
        property alias circled: photoPage.circled
        id: originalSizeImage
        visible: false
        fillMode: (squared || circled) ? Image.PreserveAspectCrop : Image.PreserveAspectFit
        width: (squared || circled) ? Math.min(sourceSize.width, sourceSize.height) : sourceSize.width
        height: (squared || circled) ? Math.min(sourceSize.width, sourceSize.height) : sourceSize.height
    }
    YACButton
    {
        visible: !Constants.isDesktop
        anchors.bottom: takeButton.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Flip")
        onClicked:
        {
            if (camera.position == Camera.BackFace)
            {
                camera.position = Camera.FrontFace
            }
            else
            {
                camera.position = Camera.BackFace
            }
        }
    }

    YACButton
    {
        id: takeButton
        width: parent.width * Constants.defaultWidthFactor
        anchors.bottom: buttonRow.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: photoPreview.visible ? qsTr("Retry") : qsTr("Take Photo")
        onClicked:
        {
            if (photoPreview.visible)
            {
                photoPreview.visible = false
                return
            }

            onClicked: camera.imageCapture.capture();
        }
    }

    YACTwoButtonRow
    {
        id: buttonRow
        width: parent.width * Constants.defaultWidthFactor
        leftText: qsTr("Ok")
        onLeftClicked:
        {
            if (!photoPreview.visible)
            {
                yacApp.badMessage(qsTr("Please take Photo first"), null, null);
                return
            }

            originalSizeImage.grabToImage(function(result)
            {
                originalSizeImage.source = result.url
                okClicked(originalSizeImage)
            }
            )
        }
        rightText: qsTr("Abort")
        onRightClicked: abortClicked()
    }
}
