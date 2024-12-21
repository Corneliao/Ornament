import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml.Models
import Qt.ImageLoader

Item {
    Component.onCompleted: {
        list_model.append({
            "image_source": "D:/Doga/WallPaper/b_7ee3bc45e31b578427bd985ae04f0ab9.png",
            "username": "Doga",
            "usermessage": "我发送了一条消息"
        });
        list_model.append({
            "image_source": "D:/Doga/WallPaper/b_7ee3bc45e31b578427bd985ae04f0ab9.png",
            "username": "Doga",
            "usermessage": "我发送了一条消息"
        });
        chat_window.createObject(stack_layout, {
            "Layout.ferredHeight": true,
            "Layout.ferredWidth": true,

        });
        chat_window.createObject(stack_layout, {
            "Layout.ferredHeight": true,
            "Layout.ferredWidth": true,

        });
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
                    imageUrl: image_source
                    imageWidth: warrper.height - 10
                    radius: 20
                    windowDpi: global.WindowDpi
                    imageHeight: warrper.height - 10
                    isRoundImage:true
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
                    text: username
                    verticalAlignment: Text.AlignVCenter
                }
                Label {
                    anchors.bottom: warrper.bottom
                    anchors.bottomMargin: 8
                    anchors.left: user_name.left
                    color: "#7b8290"
                    font.pixelSize: 12
                    horizontalAlignment: Text.AlignHCenter
                    text: usermessage
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        warrper.ListView.view.currentIndex = index;
                        stack_layout.currentIndex = index + 1;
                        var item = stack_layout.children[index + 1];
                        if (item !== null)
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
            Rectangle {
                id: rect_

                anchors.fill: parent
                color :window_.bg_color.length <=0?"transparent":window_.bg_color
                radius: 10
            }
        }
    }
}

