import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage2
{
    id: documentsPage

    Component
    {
        id: insertDocumentsComponent
        Item
        {

        }

        // AppUserInsertAppointment
        // {
        //     onCloseClicked: insertAppointmentsLoader.sourceComponent = null
        //     onAppointmentSaved: insertAppointmentsLoader.sourceComponent = null
        // }
    }
    ListView
    {
        id: theListview
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomRectangle.top
        anchors.margins: Constants.defaultMargin
        reuseItems: true
        model: 5
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }
        remove: Transition {
            NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
        }
        spacing: 1
        delegate: Rectangle
        {
            color: index % 2 ? "silver" : "lightgrey"
            width: parent.width
            height: 100
        }

    }

    Loader
    {
        anchors.fill: parent
        id: insertDocumentsLoader
    }
}
