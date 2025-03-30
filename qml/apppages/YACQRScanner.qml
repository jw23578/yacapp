import QtQuick 2.15
import QtMultimedia

Rectangle {

    function decode(preview) {
        imageToDecode.source = preview
        decoder.decodeImageQML(imageToDecode);
    }

}
