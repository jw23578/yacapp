import QtQuick 2.15
import QtWebView 1.15
import com.yacapp.contentitem 1.0
import "qrc:/MenueFunctions.js" as MenueFunctions
import "qrc:/qml/items"


Item
{
    id: theContentDelegate
    property var stackView: null
    property var theMenue: null
    property int formHeight: 0
    property int formWidth: 0
    property string contentType: ""
    property bool gridType: contentType == "grid"
    property bool columnType: contentType == "column"
    property bool rowType: contentType == "row"
    property ContentItem itemConfig: null
    width: columnType && !gridType ? parent.width : formWidth * itemConfig.widthPerThousand / 1000.0
    height: rowType && !gridType ? parent.height : formHeight * itemConfig.heightPerThousand / 1000.0

    Component
    {
        id: tileComponent
        YACRectangle
        {
            anchors.fill: parent
            color: itemConfig.color
            shadowEnabled: false
            YACText
            {
                anchors.centerIn: parent
                text: itemConfig.text
            }
        }
    }
    Component
    {
        id: flipableComponent
        Flipable
        {
            id: flipable
            anchors.fill: parent
            front: RectangleORSuperForm
            {
                anchors.fill: parent
                filename: itemConfig.flipableFilename
                color: itemConfig.color
                text: itemConfig.text
                stackView: theContentDelegate.stackView
                theMenue: theContentDelegate.theMenue
            }

            back: RectangleORSuperForm
            {
                anchors.fill: parent
                filename: itemConfig.flipableFilename2
                color: itemConfig.color2
                text: itemConfig.text2
                stackView: theContentDelegate.stackView
                theMenue: theContentDelegate.theMenue
            }
            transform: Rotation
            {
                id: rotation
                origin.x: flipable.width/2
                origin.y: flipable.height/2
                axis.x: 0; axis.y: 1; axis.z: 0     // set axis.y to 1 to rotate around y-axis
                angle: 0    // the default angle
                Behavior on angle
                {
                    NumberAnimation
                    {
                        duration: 400
                    }
                }
            }
            Timer
            {
                running: true
                interval: 3000
                repeat: true
                onTriggered: rotation.angle += 180 // rotation.angle == 0 ? 180 : 0
            }
        }
    }

    Component
    {
        id: slidertileComponent
        Item
        {
            id: slidertileItem
            clip: true
            anchors.fill: parent
            Row
            {
                id: slidertileRow
                height: slidertileItem.height
                Behavior on x
                {
                    NumberAnimation
                    {
                        duration: 200
                    }
                }

                RectangleORSuperForm
                {
                    width: slidertileItem.width
                    height: slidertileItem.height
                    filename: itemConfig.flipableFilename
                    color: itemConfig.color
                    text: itemConfig.text
                    stackView: theContentDelegate.stackView
                    theMenue: theContentDelegate.theMenue
                }
                RectangleORSuperForm
                {
                    width: slidertileItem.width
                    height: slidertileItem.height
                    filename: itemConfig.flipableFilename2
                    color: itemConfig.color2
                    text: itemConfig.text2
                    stackView: theContentDelegate.stackView
                    theMenue: theContentDelegate.theMenue
                }
            }
            Timer
            {
                running: true
                interval: 3000
                repeat: true
                onTriggered: slidertileRow.x = slidertileRow.x == 0 ? -slidertileItem.width : 0
            }
        }
    }

    Component
    {
        id: imageComponent
        Image
        {
            property string fms: itemConfig.fillModeString
            source: itemConfig.url
            fillMode: fms == "Image.Stretch" ?
                          Image.Stretch :
                          fms == "Image.PreserveAspectFit" ?
                              Image.PreserveAspectFit :
                              fms == "Image.PreserveAspectCrop" ?
                                  Image.PreserveAspectCrop :
                                  fms == "Image.Tile" ?
                                      Image.Tile :
                                      fms == "Image.TileVertically" ?
                                          Image.TileVertically :
                                          fms == "Image.TileHorizontally" ?
                                              Image.TileHorizontally :
                                              Image.Pad
            cache: true
        }
    }
    Component
    {
        id: animatedimageComponent
        AnimatedImage
        {
            source: itemConfig.url
            fillMode: Image.Stretch
            cache: true
        }
    }
    Component
    {
        id: webviewComponent
        WebView
        {
            anchors.fill: parent
            url: itemConfig.url
            visible: !theMenue.menueOpen && !Constants.topLevelFormActive && !Constants.profileOpen && !Constants.superMenueOpen && !Constants.superMenueClicked
        }
    }

    function getComponent(type)
    {
        switch (type)
        {
        case "image": return imageComponent
        case "animatedimage": return animatedimageComponent
        case "webview": return webviewComponent
        case "tile": return tileComponent
        case "slidertile": return slidertileComponent
        case "flipable": return flipableComponent
        }
        return null
    }

    Loader
    {
        id: contentLoader
        anchors.fill: parent
        anchors.leftMargin: parent.width * itemConfig.leftMarginPerThousand / 1000.0
        anchors.rightMargin: parent.width * itemConfig.rightMarginPerThousand / 1000.0
        anchors.topMargin: parent.height * itemConfig.topMarginPerThousand / 1000.0
        anchors.bottomMargin: parent.height * itemConfig.bottomMarginPerThousand / 1000.0
        sourceComponent: getComponent(itemConfig.type)
    }

    Loader
    {
        active: itemConfig.type == "file"
        asynchronous: true
        anchors.fill: parent
        source: "SuperForm.qml"
        visible: active
        onLoaded:
        {
            item.stackView = theSuperForm.stackView
            item.config = yacApp.getConfig(itemConfig.filename)
            item.theMenue = theSuperForm.theMenue
        }
    }
    Loader
    {
        id: mouseAreaLoader
        anchors.fill: parent
        sourceComponent: itemConfig.target != "" ? mouseAreaComponent : null
    }
    Component
    {
        id: mouseAreaComponent
        MouseArea
        {
            anchors.fill: parent
            onClicked: MenueFunctions.openTarget(yacApp, stackView, itemConfig.target, theMenue)
        }
    }

}
