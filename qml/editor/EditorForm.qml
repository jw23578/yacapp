import QtQuick 2.0
import com.yacapp.parsedconfig 1.0
import "../items"

Rectangle
{
    property ParsedConfig config: null
    color: "silver"
    Column
    {
        width: parent.width
        Row
        {
            width: parent.width
            YACButton
            {
                width: parent.width / 2
                text: "Load Project"
            }
            Item
            {
                width: 1
                height: 1
            }
            YACButton
            {
                width: parent.width / 2 - 1
                text: "Save Project"
            }
        }
        YACProjectText
        {
            width: parent.width
            text: "Project: "
        }

        EditorSection
        {
            sectionCaption: "Header"
            Text
            {
                text: "Title"
            }
            TextInput
            {
                x: height
                width: parent.width
                text: config.header.title
                onDisplayTextChanged: config.header.title = text
            }
            Text
            {
                text: "Height"
            }
            TextInput
            {
                x: height
                width: parent.width
                text: config.header.height
                onDisplayTextChanged: config.header.height = text
            }
        }
        EditorSection
        {
            sectionCaption: "Content"
        }

        EditorSection
        {
            sectionCaption: "Footer"
        }
    }
}
