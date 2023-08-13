import QtQuick 2.15
import QtGraphicalEffects 1.15
import ".."
import "../../items"

AppUserBasePage
{
    id: theNewsPage
    signal profileClicked()
    YACImage
    {
        id: topImage
        width: parent.width
        height: parent.height * Constants.newsPageConfig.topImageHeightFactor
        source: "https://picsum.photos/" + Math.trunc(Constants.appTotalWidth) + "/" + Math.trunc(Constants.appTotalHeight * (1 - Constants.mainMenueConfig.smallElemHeightFactor)  * Constants.newsPageConfig.topImageHeightFactor)
        Component.onCompleted: {
            Helper.jsLog(width)
            Helper.jsLog(height)
            Helper.jsLog(source)
        }
    }
    Column
    {
        width: parent.width * Constants.newsPageConfig.topRightItemWidthFactor
        anchors.top: parent.top
        anchors.right: parent.right
        Rectangle
        {
            width: parent.width
            height: theNewsPage.height * Constants.newsPageConfig.topRightItemHeightFactor
            color: "#9E9A9A"
            YACImage
            {
                id: noProfilePic
                visible: !yacApp.appUserConfig.loggedIn || yacApp.appUserConfig.profileImageId == ""
                anchors.centerIn: parent
                source: "qrc:/images/images/person_icon.svg"
            }
            YACRoundedImage
            {
                width: height
                height: parent.height * 2 / 3
                anchors.centerIn: parent
                visible: !noProfilePic.visible
                source: "image://async/profileImage/" + yacApp.appUserConfig.profileImageId
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked: theNewsPage.profileClicked()
            }
        }
        Rectangle
        {
            width: parent.width
            height: theNewsPage.height * Constants.newsPageConfig.topRightItemHeightFactor
            color: Constants.newsPageConfig.alertColor
        }
        Rectangle
        {
            width: parent.width
            height: theNewsPage.height * Constants.newsPageConfig.topRightItemHeightFactor
            color: SpaceRequestsModel.count ? "red" : Constants.newsPageConfig.messagesColor
        }
    }
    Component
    {
        id: spaceRequestDelegateComponent
        Item
        {
            property var news: null
            anchors.fill: parent
            Column
            {
                width: parent.width
                YACText
                {
                    text: news.caption
                }
                Row
                {
                    YACRoundedImage
                    {
                        height: theText.height
                        width: height
                        source: "image://async/profileImage/" + yacApp.getProfileImageId(news.spaceRequesterId)
                    }

                    YACText
                    {
                        id: theText
                        text: "by " + yacApp.getProfileVisibleName(news.spaceRequesterId)
                    }
                }
            }

        }
    }
    Component
    {
        id: dummyDelegateComponent
        YACText
        {
            property int index: o
            anchors.centerIn: parent
            text: "News #" + index
        }
    }

    ListView
    {
        id: theListview
        clip: true
        anchors.top: topImage.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        model: NewsModel
        property double itemSpacing: theNewsPage.height * Constants.newsPageConfig.contentItemSpacing
        delegate: Item
        {
            width: theNewsPage.width
            height: theNewsPage.height * Constants.newsPageConfig.contentItemHeightFactor + 2 * theListview.itemSpacing
            Rectangle
            {
                id: theRectangle
                anchors.centerIn: parent
                height: parent.height - theListview.itemSpacing
                width: theNewsPage.width * Constants.newsPageConfig.contentItemWidthFactor
                Loader
                {
                    anchors.fill: parent
                    property int specialModelIndex: index < SpaceRequestsModel.count ? index : index - SpaceRequestsModel.count
                    sourceComponent: news.type == "spaceRequest" ? spaceRequestDelegateComponent : dummyDelegateComponent
                    onLoaded: item.news = news
                }
            }
            YACDropShadow
            {
                source: theRectangle
                anchors.fill: theRectangle
            }
        }
    }
}
