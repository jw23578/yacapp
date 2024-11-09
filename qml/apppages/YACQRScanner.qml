import QtQuick 2.15
import QZXing 3.3
import QtMultimedia

Rectangle {

    function decode(preview) {
        imageToDecode.source = preview
        decoder.decodeImageQML(imageToDecode);
    }

    CaptureSession
    {
        imageCapture {
            onImageCaptured: {
                originalSizeImage.source = preview
                photoPreview.visible = true
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
        camera: Camera {
            id: camera

            exposureMode: Camera.ExposurePortrait
            exposureCompensation: -1
            focusMode: Camera.FocusContinuous
            flashMode: Camera.FlashRedEyeReduction

        }
        videoOutput: theVideoOutput
    }

    VideoOutput {
        id: theVideoOutput
        anchors.centerIn: parent
        width: parent.width
        height: width
        fillMode: VideoOutput.PreserveAspectFit
    }



    QZXingFilter
    {
        id: zxingFilter
        videoSink: theVideoOutput.videoSink()
        decoder {
            enabledDecoders: QZXing.DecoderFormat_QR_CODE

            /////////////
            //optional
            tryHarderType: QZXing.TryHarderBehaviour_ThoroughScanning | QZXing.TryHarderBehaviour_Rotate

            imageSourceFilter: QZXing.SourceFilter_ImageNormal //| QZXing.SourceFilter_ImageInverted
            /////////////
            onTagFound: Helper.jsLog("Barcode data: " + tag)
        }


//        onDecodingStarted: Helper.jsLog("Decoding of image started...")


//        onDecodingFinished: Helper.jsLog("Decoding finished " + (succeeded==true ? "successfully" :    "unsuccessfully") )
    }
}
