import QtQuick 2.15
import "../../items"
import ".."
import QtQuick.Pdf

AppUserBasePage2
{
    enableBack: true
    property alias source: theDocument.source
    PdfDocument
    {
        id: theDocument
        source: "file:/home/jw78/.cache/jw78/yacapp/6a22caf5-18fe-4d30-8571-e7fcfd931960/Weihnachtskarte 2024.pdf"
    }

    PdfMultiPageView
    {
        id: theView
        anchors.fill: content
        document: theDocument
        clip: true
    }
}
