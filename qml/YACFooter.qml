import QtQuick 2.0

Rectangle
{
    property int minimumHeight: 0
    property var footerConfig: null
    height: Math.max(minimumHeight, parent.height * footerConfig.heightPerThousand / 1000.0)
    width: parent.width
    color: footerConfig.color
}
