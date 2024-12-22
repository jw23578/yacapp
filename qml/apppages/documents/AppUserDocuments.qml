import QtQuick 2.15
import "../../items"
import ".."

AppUserBasePage2
{
    id: documentsPage

    multiMenueButton.visible: true
    multiMenueButton.model: [{caption: qsTr("Add Documents")},
        {caption: qsTr("Delete Document")}]
    multiMenueButton.onClicked: function(caption)
    {
        Helper.jsLog("caption: " + caption)
        if (caption == qsTr("Add Documents"))
        {
            theLoader.sourceComponent = addDocumentsComponent
        }
        if (caption == qsTr("Delete Document"))
        {
        }
    }
    multiMenueButton.hide: theLoader.status == Loader.Ready

    Component
    {
        id: addDocumentsComponent
        AppUserAddDocuments
        {
            onCloseClicked: theLoader.sourceComponent = null
        }
    }
    AppPageListView
    {
        id: theListview
        anchors.bottom: bottomRectangle.top
        model: DocumentsModel
        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }
        remove: Transition {
            NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
        }
        delegate: Rectangle
        {
            color: index % 2 ? "silver" : "lightgrey"
            width: parent.width
            height: 100
            Column
            {
                YACText
                {
                    text: document.document_name
                }
                YACText
                {
                    text: Helper.formatDateLong(document.created_datetime) + " um " + Helper.formatTime(document.created_datetime)
                }
            }
            Item
            {
                id: deleteItem
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: parent.height / 2
                height: width
                Image
                {
                    mipmap: true
                    anchors.fill: parent
                    source: "qrc:/images/images/delete.svg"
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        CPPQMLAppAndConfigurator.yesNoQuestion("Löschen?", null, function() {
                            yacApp.appUserDeleteDocument(document.id,
                                                         function(message)
                                                         {
                                                             CPPQMLAppAndConfigurator.badMessage(qsTr("could not delete document, please try again later. ") + message, null, null)
                                                         }
                                                         )
                        }, null)
                    }
                }
            }

        }

    }

    Loader
    {
        anchors.fill: parent
        id: theLoader
    }
}
