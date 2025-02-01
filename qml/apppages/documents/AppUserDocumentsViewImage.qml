import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage2
{
    enableBack: true
    property alias source: theImage.source
    YACImage
    {
        id: theImage
        anchors.fill: content
    }
}
