import QtQuick 2.0
import com.yacapp.parsedconfig 1.0

Rectangle
{
    property ParsedConfig config: null
    color: "silver"
    Column
    {
        width: parent.width
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
