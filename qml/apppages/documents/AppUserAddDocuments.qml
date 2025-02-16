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
            fileSize: 0
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
                                          "fileUrl": selectedFiles[i].toString(),
                                          "fileSize": Helper.fileSize(selectedFiles[i])})
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
        YACButton
        {
            text: qsTr("Take Picture")
            onClicked: yacApp.goTakePhoto(true, true, originalSizeImage)
            width: parent.width
        }
    }
    Image
    {
        id: originalSizeImage
        visible: false
        onSourceChanged: {
            theUploadRectangle.visible = true
            visibleImage.source = source
        }
    }

    Rectangle
    {
        visible: false
        anchors.fill: parent
        id: theUploadRectangle
        YACKeywordsEditor
        {
            id: theKeywordsEditor
        }
        YACLineEditWithHeader
        {
            id: theFilename
            headerText: qsTr("Filename (without extension)")
            anchors.top: theKeywordsEditor.bottom
            visible: visibleImage.visible
        }

        YACImage
        {
            width: parent.width
            anchors.top: theFilename.bottom
            anchors.bottom: bottomButtons.top
            id: visibleImage
            visible: false
            onSourceChanged: {
                visible = true
            }
        }

        AppPageListView
        {
            id: theUploadView
            model: theUploadModel
            anchors.top: theKeywordsEditor.bottom
            anchors.bottom: bottomButtons.top
            visible: !visibleImage.visible
            delegate:
                Rectangle
            {
                width: theUploadRectangle.width
                height: theColumn.height
                Column
                {
                    id: theColumn
                    YACText
                    {
                        id: theFileUrl
                        text: fileNameWithExtension
                    }
                    YACText
                    {
                        text: Helper.formatFileSize(fileSize)
                    }
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
                var keywords = theKeywordsEditor.getKeywords()
                if (visibleImage.visible)
                {
                    var niceFilename = theFilename.displayText
                    if (niceFilename == "")
                    {
                        CPPQMLAppAndConfigurator.badMessage(qsTr("Please enter a filename"), theFilename, null)
                        return
                    }

                    originalSizeImage.grabToImage(function(result)
                    {
                        var filename = yacApp.getCacheImageFilename();
                        result.saveToFile(filename)
                        yacApp.appUserPostDocument("file:" + filename,
                                                   niceFilename,
                                                   keywords,
                                                   function(message)
                                                   {
                                                       CPPQMLAppAndConfigurator.goodMessage(message, null, null)
                                                   },
                                                   function(message)
                                                   {
                                                       CPPQMLAppAndConfigurator.badMessage(message, null. null)
                                                   }
                                                   )

                        closePage()
                    }
                    )
                }
                else
                {
                    for (var i = 0; i < theUploadModel.count; ++i)
                    {
                        yacApp.appUserPostDocument(theUploadModel.get(i).fileUrl,
                                                   "",
                                                   keywords,
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
                    closePage()
                }
            }
        }
    }
}
