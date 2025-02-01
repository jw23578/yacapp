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
    }
    ListModel
    {
        id: theKeywordModel
    }

    Rectangle
    {
        visible: false
        anchors.fill: parent
        id: theUploadRectangle
        YACLineEditWithHeader
        {
            id: keywordsEdit
            headerText: qsTr("Keywords")
            function appendKeyword(keyword)
            {
                for (var i = 0; i < theKeywordModel.count; ++i)
                {
                    var kw = theKeywordModel.get(i).keyword
                    if (kw == keyword)
                    {
                        return
                    }
                }
                theKeywordModel.append({"keyword": keyword})

            }

            Keys.onReturnPressed:  {
                var s = displayText.trim()
                if (s.length == 0)
                {
                    return
                }
                appendKeyword(s)
                text = ""
            }
            Keys.onEnterPressed:  {
                var s = displayText.trim()
                if (s.length == 0)
                {
                    return
                }
                appendKeyword(s)
                text = ""
            }

            onDisplayTextChanged: {
                var s = displayText
                if (s.toLowerCase() != s)
                {
                    text = s.toLowerCase()
                    return
                }

                if (s.trim().length <= 1)
                {
                    return
                }
                var delimiter = ' ,;';
                if (delimiter.indexOf(s[s.length - 1]) == -1)
                {
                    return
                }
                var keyword = s.substring(0, s.length - 1).trim()
                text = ""
                appendKeyword(keyword)
            }
        }
        AppPageListView
        {
            id: keywordsView
            anchors.top: keywordsEdit.bottom
            model: theKeywordModel
            delegate: YACTextDeleteable
            {
                text: keyword
                onDeleteClicked: theKeywordModel.remove(index)
            }
            height: contentHeight
        }

        AppPageListView
        {
            id: theUploadView
            model: theUploadModel
            anchors.top: keywordsView.bottom
            anchors.bottom: bottomButtons.top
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
                var keywords = []
                for (var i = 0; i < theKeywordModel.count; ++i)
                {
                    keywords.push(theKeywordModel.get(i).keyword)
                }

                for (var i = 0; i < theUploadModel.count; ++i)
                {
                    yacApp.appUserPostDocument(theUploadModel.get(i).fileUrl,
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
