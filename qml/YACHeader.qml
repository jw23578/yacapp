import QtQuick 2.0

Rectangle
{
    height: spot.y + spot.height / 2// parent.height * headerConfig.height
    color: headerConfig.color
    property var headerConfig: null
    Text
    {
        id: headerText
        anchors.centerIn: parent
        text: header.headerConfig.title
    }
    Rectangle
    {
        visible: Constants.isConfigurator
        id: spot
        width: 15
        height: width
        color: "red"
        x: parent.width / 2 - width / 2
        y: parent.parent.height * headerConfig.height - height / 2
        //            onYChanged: parent.height = y + height / 2

        MouseArea {
            anchors.fill: parent
            drag.target: spot
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.threshold: 0
            drag.smoothed: true
            //                drag.maximumX: container.width - rect.width
        }
    }
}
