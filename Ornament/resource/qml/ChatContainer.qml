import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml.Models
import Qt.ImageLoader

Item {
    /**
     * @brief 创建用户聊天窗口
     * @param userAccount
     * @param userName
     * @param imageData
     */
    function createFriendItem(userAccount,userName,imageData) {
        list_model.append({
            "imageSource":imageData,
            "userName":userName,
            "userAccount":userAccount,
            "userMessage":"我发送了一条消息"
        })

        chat_window.createObject(stack_layout,{
            "Layout.fillWidth":true,
            "Layout.fillHeight":true,
            "userAccount":userAccount,
            "userName":userName,
            "userHead":imageData
        })
    }

    RowLayout {
        id: row_layout

        anchors.fill: parent

        ListView {
            id: chat_listview

            Layout.fillHeight: true
            Layout.preferredWidth: 230
            clip: true
            model: list_model
            spacing: 10

            currentIndex:-1

            delegate: Rectangle {
                id: warrper

                color: "transparent"
                height: 60
                radius: 10
                width: ListView.view.width

                ImageLoader {
                    id: user_head

                    anchors.left: warrper.left
                    anchors.leftMargin: 5
                    anchors.verticalCenter: warrper.verticalCenter
                    height: warrper.height - 10
                    width:warrper.height - 10
                    imageWidth: warrper.height - 10
                    windowDpi: global.windowDpi
                    imageByteArray:imageSource
                    imageHeight: warrper.height - 10
                    isRoundImage:true
                    isFromData:true

                }

                Label {
                    id: user_name

                    anchors.left: user_head.right
                    anchors.leftMargin: 8
                    anchors.top: user_head.top
                    anchors.topMargin: 8
                    //color: "#a5b3cb"
                    font.bold: true
                    font.pixelSize: 13
                    horizontalAlignment: Text.AlignHCenter
                    text: userName
                    verticalAlignment: Text.AlignVCenter
                }
                Label {
                    anchors.bottom: warrper.bottom
                    anchors.bottomMargin: 8
                    anchors.left: user_name.left
                    color: "#7b8290"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    text: userMessage
                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        if(warrper.ListView.view.currentIndex === index)
                            return;
                        warrper.ListView.view.currentIndex = index;
                        stack_layout.currentIndex = index + 1;
                        var item = stack_layout.children[index + 1];
                        if (item !== undefined)
                            item.startAnimation();
                    }
                }
            }
            highlight: Rectangle {
                //color: Qt.rgba(0 / 255, 0 / 255, 0 / 255, 10 / 255)
                color: "white"
                radius: 10
            }
        }
        Loader {
            Layout.fillHeight: true
            Layout.preferredWidth: 15
        }
        StackLayout {
            id: stack_layout

            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true

            ChatDefaultContainer {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
    ListModel {
        id: list_model

    }
    Component {
        id: chat_window

        Item {
            id: window_

            property string bg_color
            property string name
            property string userAccount
            property string userName
            property alias  userHead :chatwindow_userhead.imageByteArray
            function startAnimation() {
                animation_.start();
            }

            ParallelAnimation {
                id: animation_

                YAnimator {
                    duration: 500
                    easing.type: Easing.InOutExpo
                    from: window_.height
                    target: window_
                    to: 0
                }
                PropertyAnimation {
                    duration: 500
                    from: 0
                    properties: "opacity"
                    target: window_
                    to: 1
                }
            }
            ColumnLayout {
                anchors.fill:parent

                Rectangle {
                    id: rect_

                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    color :window_.bg_color.length <=0?"transparent":window_.bg_color
                    RowLayout {
                        width:parent.width
                        height:45
                        anchors.top:parent.top
                        ImageLoader {
                            id:chatwindow_userhead
                            width:40
                            height:40
                            imageWidth:40
                            imageHeight:40
                            isFromData:true
                            isRoundImage:true
                            windowDpi:global.windowDpi
                        }
                        Label  {
                            text:window_.userName
                            horizontalAlignment:Text.AlignHCenter
                            verticalAlignment:Text.AlignVCenter
                            font.pixelSize:14
                        }
                        Loader {
                            Layout.fillHeight:true
                            Layout.fillWidth:true
                        }
                    }
                }
            }
        }
    }
}

