import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQml.Models
import Qt.ImageLoader
import QtQml
Item {
    /**
     * @brief 创建用户聊天窗口
     * @param userAccount
     * @param userName
     * @param imageData
     */
    function createFriendItem(userAccount,userName,imageData) {
        // list_model.append({
        //     "imageSource":imageData,
        //     "userName":userName,
        //     "userAccount":userAccount,
        //     "userMessage":"我发送了一条消息"
        // })
        //
        //
        //
        // var object = chat_window.createObject(stack_layout,{
        //     "Layout.fillWidth":true,
        //     "Layout.fillHeight":true,
        //     "userAccount":userAccount,
        //     "userName":userName,
        //     "userHead":imageData
        // })
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
            property string  userHead // :chatwindow_userhead.imageByteArray
            property string message_temptemp;

            function startAnimation() {
                animation_.start();
            }

            function addUserRecvTextMessage() {
                message_model.append({
                    "userMessage":"我发送了一条消息",
                    "itemDirection":"recv"
                })
            }

            function addMySendTextMessage(text) {
                message_model.append({
                    "userMessage":text,
                    "itemDirection":"send"
                })
                scrollEnd.start()
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
                RowLayout {
                    Layout.fillWidth:true
                    Layout.preferredHeight:50
                    Layout.alignment:Qt.AlignTop

                    Label  {
                        text:window_.userName
                        horizontalAlignment:Text.AlignHCenter
                        verticalAlignment:Text.AlignVCenter
                        font.pixelSize:17
                    }

                    Loader {
                        Layout.preferredHeight:10
                        Layout.fillWidth:true
                    }
                }
                ListView {
                    signal adjustMessageItemSize()
                    model:message_model
                    Layout.fillWidth:true
                    Layout.fillHeight:true
                    clip:true
                    id:message_list
                    spacing:10
                    antialiasing:true
                    smooth:true
                    // verticalLayoutDirection: ListView.BottomToTop
                    delegate:ChatMessageDelegate {
                        id:message_delegate
                        message:userMessage
                        direction:itemDirection
                        userHeadData:userHead
                        Component.onCompleted: {
                            message_delegate.ListView.view.adjustMessageItemSize.connect(message_delegate.wrapText)
                        }

                    }
                    onWidthChanged: {
                        message_list.adjustMessageItemSize()
                    }
                }

                ScrollView {
                    Layout.preferredHeight:130
                    Layout.fillWidth:true
                    TextArea {
                        id:textarea_
                        width:parent.width
                        height:130
                        focus:true
                        placeholderText:"Enter Your Mesasge..."
                        smooth:true
                        antialiasing:true
                        wrapMode: TextArea.WrapAnywhere
                        font.pixelSize:14
                        Keys.onReturnPressed:event=> {
                            if(textarea_.text.length  <=0)
                                return;
                            if(event.modifiers && Qt.ControlModifier) {
                                text += '\n'
                                textarea_.cursorPosition = textarea_.length
                            }
                            else {
                                window_.addMySendTextMessage(textarea_.text)
                                textarea_.clear();
                            }
                        }
                        background:Rectangle {
                            color:"transparent"
                            anchors.fill:parent
                        }

                    }

                }
                RowLayout {
                    Layout.fillWidth:true
                    Layout.preferredHeight:30
                    Layout.bottomMargin:10
                    Loader {
                        Layout.fillWidth:true
                        Layout.preferredHeight:1
                    }
                    Rectangle {
                        radius:7
                        color:"#58C7B5"
                        Layout.preferredHeight:parent.height
                        Layout.preferredWidth:80
                        Label {
                            text:"发送"
                            horizontalAlignment:Text.AlignHCenter
                            verticalAlignment:Text.AlignVCenter
                            color:"white"
                            font.pixelSize:14
                            anchors.centerIn:parent
                        }
                    }
                }
            }

            ListModel {
                id:message_model
            }
            Timer {
                id:scrollEnd
                interval:10
                onTriggered:
                    message_list.positionViewAtIndex(message_list.count - 1, ListView.Beginning)
            }
        }
    }
}

