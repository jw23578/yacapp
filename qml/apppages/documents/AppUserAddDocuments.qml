import QtQuick 2.15
import "../../items"
import ".."
import QtQuick.Dialogs

AppUserBasePage2
{
    id: theAppUserAddDocuments
    enableBack: true
    ListModel
    {
        id: theUploadModel
        ListElement
        {
            fileUrl: ""
            fileNameWithExtension: ""
        }
    }

    FileDialog
    {
        id: theFileDialog
        fileMode: FileDialog.OpenFiles
        onAccepted:
        {
            theUploadModel.clear()
            for (var i = 0; i < selectedFiles.length; ++i)
            {
                theUploadModel.append({"fileNameWithExtension": Helper.extractFileNameWithExtension(selectedFiles[i]),
                                        "fileUrl": selectedFiles[i].toString()})
            }
            theUploadRectangle.visible = true
        }
    }
    AppPageColumn
    {
        YACButton
        {
            text: qsTr("Select Files (any type)")
            onClicked: {
                theFileDialog.nameFilters = []
                theFileDialog.open()
            }
            width: theAppUserAddDocuments.width - 2 * Constants.defaultMargin
        }
        YACButton
        {
            text: qsTr("Select PDFs")
            onClicked: {
                theFileDialog.nameFilters = ["PDFs (*.pdf)"]
                theFileDialog.open()
            }
            width: parent.width
        }
    }

    Rectangle
    {
        visible: false
        anchors.fill: parent
        id: theUploadRectangle
        AppPageListView
        {
            id: theUploadView
            model: theUploadModel
            anchors.bottom: bottomButtons.top
            delegate:
                Rectangle
            {
                width: theUploadRectangle.width
                height: theFileUrl.height
                YACText
                {
                    id: theFileUrl
                    text: fileNameWithExtension
                }
            }
        }

        YACTwoButtonRow
        {
            id: bottomButtons
            leftText: qsTr("Upload")
            rightText: qsTr("Abort")
            onRightClicked: theUploadRectangle.visible = false
            onLeftClicked: {
                console.log("hello")
                for (var i = 0; i < theUploadModel.count; ++i)
                {
                    console.log(i + ": " + theUploadModel.get(i).fileUrl)
                    yacApp.appUserPostDocument(theUploadModel.get(i).fileUrl,
                                                     function(message)
                                                     {
                                                        CPPQMLAppAndConfigurator.goodMessage(message, null)
                                                     },
                                                     function(message)
                                                     {
                                                         CPPQMLAppAndConfigurator.badMessage(message, null)
                                                     }
                                                     )

                }
            }
        }
    }
}
