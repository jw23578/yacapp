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
    Component
    {
        id: appUserDocumentsViewPdf
        AppUserDocumentsViewPdf
        {
            onCloseClicked: theViewDocumentLoader.sourceComponent = null
        }
    }
    Component
    {
        id: appUserDocumentsViewImage
        AppUserDocumentsViewImage
        {
            onCloseClicked: theViewDocumentLoader.sourceComponent = null
        }
    }
    YACLineEditWithHeader
    {
        id: needle
        headerText: qsTr("Suche (mit Leerzeichen trennen)")
        width: theListview.width
        anchors.horizontalCenter: parent.horizontalCenter
        onDisplayTextChanged: yacApp.appUserFetchDocuments(needle.text,
                                                           0,
                                                           10,
                                                           function(message) {},
                                                           function(message) {})
    }
    YACText
    {
        id: countLabel
        anchors.top: needle.bottom
        width: theListview.width
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Anzahl: ") + theListview.count
    }

    AppPageListView
    {
        id: theListview
        anchors.top: countLabel.bottom
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
                    text: document.document_type + " " + document.document_name
                }
                YACText
                {
                    text: Helper.formatDateLong(document.created_datetime) + " um " + Helper.formatTime(document.created_datetime)
                }
                YACText
                {
                    text: Helper.formatFileSize(document.document_size)
                }
                YACText
                {
                    text: Helper.decodeBase64(document.transfer_comma_separated_catchphrases_base64)
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: {
                    if (document.document_type != "pdf" && document.document_type != "png")
                    {
                        CPPQMLAppAndConfigurator.badMessage(document.document_type + qsTr(" is currently not supported."), null, null)
                        return;
                    }
                    onClicked: yacApp.appUserFetchDocument(document.id,
                                                           document.document_name,
                                                           document.document_type,
                                                           function(message)
                                                           {
                                                               theViewDocumentLoader.fileName = "file:" + message
                                                               if (document.document_type == "pdf")
                                                               {
                                                                    theViewDocumentLoader.sourceComponent = appUserDocumentsViewPdf
                                                               }
                                                               if (document.document_type == "png")
                                                               {
                                                                    theViewDocumentLoader.sourceComponent = appUserDocumentsViewImage
                                                               }


                                                           },
                                                           function(message)
                                                           {
                                                               CPPQMLAppAndConfigurator.badMessage(qsTr("could not download document, please try again later. ") + message, null, null)
                                                           }
                                                           )

                }
            }

            Item
            {
                id: downloadItem
                anchors.right: deleteItem.left
                anchors.verticalCenter: parent.verticalCenter
                width: parent.height / 2
                height: width
                Image
                {
                    mipmap: true
                    anchors.fill: parent
                    source: "qrc:/images/images/download.svg"
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: yacApp.appUserFetchDocument(document.id,
                                                           document.document_name,
                                                           document.document_type,
                                                           function(message)
                                                           {
                                                               Helper.jsLog(message);
                                                           },
                                                           function(message)
                                                           {
                                                               CPPQMLAppAndConfigurator.badMessage(qsTr("could not download document, please try again later. ") + message, null, null)
                                                           }
                                                           )
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
        z: 2
        id: theViewDocumentLoader
        anchors.fill: parent
        property string fileName: ""
        onLoaded: item.source = fileName
    }

    Loader
    {
        z: 2
        anchors.fill: parent
        id: theLoader
    }
}
