import QtQuick 2.15
import QZXing 3.3
import QtMultimedia 5.15

Rectangle {

    function decode(preview) {
        imageToDecode.source = preview
        decoder.decodeImageQML(imageToDecode);
    }

    Camera {
        id: camera
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

        flash.mode: Camera.FlashRedEyeReduction
        focus.focusMode: Camera.FocusContinuous
    }
    VideoOutput {
        anchors.centerIn: parent
        width: parent.width
        height: width
        fillMode: VideoOutput.PreserveAspectFit
        autoOrientation: true
        source: camera
        filters: [ zxingFilter ]
    }

    QZXingFilter
    {
        id: zxingFilter
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
