import QtQuick 2.15
import "items"
import "../HttpFunctions.js" as Http

YACRectangle
{
    color: "orange"
    visible: yacApp.globalConfig.projectID == ""
    anchors.fill: parent

    ListModel
    {
        id: appModel
        ListElement
        {
            projectID: "123"
            projectName: "Test Test"
            projectLogo: "https://www.hc-salzland-06.de/pages/wp-content/uploads/sites/3/2017/12/crest-soccer.png"
        }
        ListElement
        {
            projectID: "123"
            projectName: "Test Test"
            projectLogo: "https://www.hc-salzland-06.de/pages/wp-content/uploads/sites/3/2017/12/crest-soccer.png"
        }
        ListElement
        {
            projectID: "123"
            projectName: "Test Test"
            projectLogo: "https://www.hc-salzland-06.de/pages/wp-content/uploads/sites/3/2017/12/crest-soccer.png"
        }
        ListElement
        {
            projectID: "123"
            projectName: "Test Test"
            projectLogo: "https://www.hc-salzland-06.de/pages/wp-content/uploads/sites/3/2017/12/crest-soccer.png"
        }
    }

    ListView
    {
        id: appView
        clip: true
        width: parent.width * 2 / 4
        height: parent.height * 3 / 4
        anchors.centerIn: parent
        model: appModel
        spacing: radiusSpacing / 2
        property double radiusSpacing: width / 12
        delegate: YACRectangle
        {
            color: "green"
            width: appView.width
            height: width
            radiusTopLeft: index == 0 ? appView.radiusSpacing : 0
            radiusTopRight: radiusTopLeft
            radiusBottomLeft: index == appModel.count - 1 ? appView.radiusSpacing : 0
            radiusBottomRight: radiusBottomLeft
            YACText
            {
                id: nameText
                text: projectName
                anchors.top: parent.top
                anchors.topMargin: appView.spacing
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
            }
            YACImage
            {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: nameText.bottom
                anchors.bottom: parent.bottom
                width: height
                source: projectLogo
            }
        }
    }
}
