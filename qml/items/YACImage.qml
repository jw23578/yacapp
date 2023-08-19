import QtQuick 2.15

Item
{
    property bool showLoadingImage: false
    property alias source: theImage.source
    property alias fillMode: theImage.fillMode
    Image
    {
        id: theImage
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        cache: true
        mipmap: true
        opacity: 0
        Behavior on opacity {
            NumberAnimation {
                duration: 400
            }
        }
        onStatusChanged: {
            if (status == Image.Ready)
            {
                opacity = 1
            }
        }
    }
    AnimatedImage
    {
        function getRandomInt(max)
        {
          return Math.floor(Math.random() * max);
        }

        id: loadingImage
        anchors.centerIn: parent
        width: Math.min(theImage.width, theImage.height, sourceSize.width)
        height: width
        visible: opacity > 0 // parent.status == Image.Loading && showLoadingImage
        source: "qrc:/images/images/loading_animation" + (getRandomInt(6) + 1) + ".webp"
        opacity: theImage.status == Image.Loading && showLoadingImage ? 1 : 0
        Behavior on opacity {
            NumberAnimation {
                duration: 400
            }
        }
    }
}
