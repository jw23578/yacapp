import QtQuick 2.15

Item
{
    width: parent.width
    height: keywordsEdit.height + keywordsView.height

    function getKeywords()
    {
        var keywords = []
        for (var i = 0; i < theKeywordModel.count; ++i)
        {
            keywords.push(theKeywordModel.get(i).keyword)
        }
        return keywords;
    }

    ListModel
    {
        id: theKeywordModel
    }
    YACLineEditWithHeader
    {
        id: keywordsEdit
        headerText: qsTr("Keywords")
        focus: true
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

}
