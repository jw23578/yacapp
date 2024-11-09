import QtQuick
import QtMultimedia
import QtQuick.Effects

import "../items"

Rectangle
{
    id: photoPage
    signal abortClicked()
    signal okClicked(var image)
    property bool squared: false
    property bool circled: true
    property alias theOriginalSizeImage: originalSizeImage
    anchors.fill: parent
    CaptureSession
    {
        imageCapture: ImageCapture {
            id: theImageCapture
            onImageCaptured: function(requestId, preview) {
                photoPreview.visible = true
            }
        }
        camera: Camera {
            active: true
            id: camera

            // exposureMode: Camera.ExposurePortrait
            // exposureCompensation: -1
            focusMode: Camera.FocusModeAuto
            flashMode: Camera.FlashAuto

        }
        videoOutput: theVideoOutput
    }

    VideoOutput
    {
        property alias squared: photoPage.squared
        property alias circled: photoPage.circled
        property double sizeFactor: 0.6
        id: theVideoOutput
        visible: false
        anchors.centerIn: parent
        fillMode: (squared || circled) ? Image.PreserveAspectCrop : Image.PreserveAspectFit
        width: ((squared || circled) ? Math.min(photoPage.width, photoPage.height) : photoPage.width) * sizeFactor
        height: ((squared || circled) ? Math.min(photoPage.width, photoPage.height) : photoPage.width) * sizeFactor
    }
    Rectangle
    {
        id: theMask
        layer.enabled: true
        width: theVideoOutput.width
        height: theVideoOutput.height
        radius: Math.min(width, height) / 2
        color: "black"
        visible: false
    }
    MultiEffect
    {
        source: theVideoOutput
        anchors.fill: theVideoOutput
        maskEnabled: photoPage.circled
        maskSource: theMask
    }

    Image
    {
        visible: false
        anchors.fill: theVideoOutput
        id: photoPreviewImage
        source: theImageCapture.preview
        fillMode: theVideoOutput.fillMode
        autoTransform: true
    }
    MultiEffect
    {
        visible: false
        id: photoPreview
        source: photoPreviewImage
        anchors.fill: photoPreviewImage
        maskEnabled: photoPage.circled
        maskSource: theMask
    }
    Image
    {
        property alias squared: photoPage.squared
        property alias circled: photoPage.circled
        anchors.horizontalCenter: parent.horizontalCenter
        id: originalSizeImage
        source: theImageCapture.preview
        visible: true
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

            onClicked: theImageCapture.capture();
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
                CPPQMLAppAndConfigurator.badMessage(qsTr("Please take Photo first"), null, null);
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
