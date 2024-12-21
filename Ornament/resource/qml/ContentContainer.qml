import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: chat_container

    signal setStacklayoutCurrentIndex(int index)

    function setFeatureCurrentIndex() {
        feature_view.currentIndex = 1;
        swipe_view.currentIndex = 0;
    }

    height: parent.height
    width: parent.width

    Item {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        RowLayout {
            anchors.fill: parent
            spacing: 15

            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: 150

                ListView {
                    id: feature_view

                    anchors.fill: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    clip: true
                    model: list_model
                    spacing: 10

                    delegate: Rectangle {
                        id: warrper

                        color: "transparent"
                        height: 40
                        radius: 10
                        width: ListView.view.width

                        Item {
                            anchors.fill: warrper
                            anchors.leftMargin: 10

                            RowLayout {
                                anchors.fill: parent
                                spacing: 15

                                Image {
                                    Layout.preferredHeight: parent.height - 20
                                    Layout.preferredWidth: parent.height - 20
                                    antialiasing: true
                                    fillMode: Image.PreserveAspectFit
                                    mipmap: true
                                    smooth: true
                                    source: warrper.ListView.isCurrentItem ? ico_path_sel : ico_path_unsel
                                }
                                Text {
                                    Layout.alignment: Qt.AlignCenter
                                    color: warrper.ListView.isCurrentItem ? "white" : "black"
                                    font.bold: warrper.ListView.isCurrentItem ? false : true
                                    horizontalAlignment: Text.AlignHCenter
                                    text: feature_text
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Loader {
                                    Layout.fillHeight: true
                                    Layout.fillWidth: true
                                }
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                feature_view.currentIndex = index;
                                if (index === 0)
                                    chat_container.setStacklayoutCurrentIndex(index);
                                else
                                    swipe_view.currentIndex = index - 1;
                            }
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;

                                if (feature_view.currentIndex !== index)
                                    warrper.color = Qt.rgba(0 / 255, 0 / 255, 0 / 255, 10 / 255);
                            }
                            onExited: {
                                warrper.color = "transparent";
                            }
                        }
                    }
                    highlight: Rectangle {
                        color: "#407aff"
                        radius: 10
                    }
                }
                ListModel {
                    id: list_model

                    // ico_path: ""
                    ListElement {
                        feature_text: "主页"
                        ico_path_sel: "qrc:/resource/ico/home_ico_white.png"
                        ico_path_unsel: "qrc:/resource/ico/home_ico_black.png"
                    }
                    ListElement {
                        feature_text: "消息"
                        ico_path_sel: "qrc:/resource/ico/MageMessageDots_sel.png"
                        ico_path_unsel: "qrc:/resource/ico/MageMessageDots_unsel.png"
                    }
                    ListElement {
                        feature_text: "好友"
                        ico_path_sel: "qrc:/resource/ico/friend_ico_white.png"
                        ico_path_unsel: "qrc:/resource/ico/friend_ico_black.png"
                    }
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                SwipeView {
                    id: swipe_view

                    anchors.fill: parent
                    clip: true
                    currentIndex: 0
                    interactive: false
                    orientation: Qt.Vertical
                    spacing: 20

                    ChatContainer {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                    FriendContainer {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}

