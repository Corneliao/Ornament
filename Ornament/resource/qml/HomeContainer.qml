import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import Qt.ImageLoader 1.0

Item {
    id: home_container

    property bool isHide: false

    //显示窗口
    function showHomeContainer() {
        if (home_container.visible === false || home_container.isHide) {
            home_container.visible = true;
            scaleAndopacityAnimation.from = 0;
            scaleAndopacityAnimation.to = 1;
            xAnimation.from = home_container.width;
            xAnimation.to = 0;
            paraAnimation.start();
        }
    }

    height: parent.height
    width: parent.width

    //窗口收起动画
    ParallelAnimation {
        id: paraAnimation

        onFinished: {
            if (home_container.opacity <1) {
                home_container.isHide = true;
                home_container.visible = false;
            } else {
                home_container.isHide = false;
            }
        }

        NumberAnimation {
            id: scaleAndopacityAnimation

            duration: 700
            properties: "scale,opacity"
            target: home_container
            to: 0
        }
        XAnimator {
            id: xAnimation

            duration: 700
            easing.type: Easing.InOutExpo
            target: home_container
            to: home_container.width
        }
    }
    MouseArea {
        anchors.fill: parent

        onClicked: forceActiveFocus()
    }
    Item {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        ColumnLayout {
            anchors.fill: parent

            //  top
            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 60

                RowLayout {
                    anchors.fill: parent

                    Rectangle {
                        Layout.preferredHeight: 40
                        Layout.preferredWidth: 40
                        color: "transparent"
                        radius: 5

                        Image {
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            height: parent.width - 20
                            smooth: true
                            source: "qrc:/resource/ico/ArrowBack.png"
                            width: parent.width - 20
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;
                                parent.color = Qt.rgba(0 / 255, 0 / 255, 0 / 255, 25 / 255);
                            }
                            onExited: {
                                parent.color = "transparent";
                            }
                            onPressed: {
                                scaleAndopacityAnimation.from = 1;
                                scaleAndopacityAnimation.to = 0;
                                xAnimation.from = 0;
                                xAnimation.to = home_container.width;
                                paraAnimation.start();
                            }
                        }
                    }
                    Loader {
                        Layout.preferredHeight: 10
                        Layout.preferredWidth: 10
                    }

                    ImageLoader {
                        id: home_image

                        Layout.preferredHeight: 50
                        Layout.preferredWidth: 50
                        imageHeight: 50
                        imageUrl: "D:/Doga/WallPaper/_20241201135729.jpg"
                        imageWidth: 50
                        radius: 15
                        windowDpi: global.WindowDpi
                    }
                    Loader {
                        Layout.preferredHeight: 10
                        Layout.preferredWidth: 10
                    }
                    //image right
                    Item {
                        Layout.fillWidth: true
                        Layout.preferredHeight: home_image.height

                        ColumnLayout {
                            anchors.fill: parent

                            Loader {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }
                            Text {
                                font.bold: true
                                font.pixelSize: 15
                                renderType: Text.NativeRendering
                                text: "Doga"
                            }
                            RowLayout {
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                Image {
                                    Layout.preferredHeight: 20
                                    Layout.preferredWidth: 20
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    source: "qrc:/resource/ico/online.png"
                                }
                                Text {
                                    font.bold: false
                                    font.pixelSize: 11
                                    renderType: Text.NativeRendering
                                    text: "永远相信美好的事情即将发生"
                                }
                            }
                            Loader {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }
                        }
                    }
                    Loader {
                        Layout.fillWidth: true
                        Layout.preferredHeight: parent.height
                    }
                    //search
                    Rectangle {
                        Layout.preferredHeight: parent.height - 20
                        Layout.preferredWidth: 200
                        color: "#e5e7e9"
                        radius: 20

                        Image {
                            id: search_ico

                            anchors.left: parent.left
                            anchors.leftMargin: 5
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            height: 30
                            source: "qrc:/resource/ico/audio_ico.png"
                            width: 30
                        }
                        TextField {
                            id: textfield_

                            anchors.left: search_ico.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: search_ico.verticalCenter
                            clip: true
                            font.pixelSize: 12
                            height: 30
                            placeholderText: "Search"
                            verticalAlignment: Text.AlignVCenter
                            width: 120

                            background: Rectangle {
                                color: "transparent"
                            }
                        }
                    }
                    Loader {
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 20
                    }
                    //all tool
                    Rectangle {
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 35
                        color: "white"
                        radius: 35 / 2

                        Image {
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            height: parent.width - 15
                            source: "qrc:/resource/ico/ProiconsApps.png"
                            width: parent.width - 15
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;
                            }
                        }
                    }
                    Loader {
                        Layout.preferredHeight: 10
                        Layout.preferredWidth: 15
                    }
                    //notification
                    Rectangle {
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 35
                        color: "white"
                        radius: 35 / 2

                        Image {
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            height: parent.width - 15
                            source: "qrc:/resource/ico/MageNotificationBell.png"
                            width: parent.width - 15
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;
                            }
                        }
                        Rectangle {
                            anchors.right: parent.right
                            anchors.rightMargin: 5
                            anchors.top: parent.top
                            anchors.topMargin: 7
                            color: "#d5b6f9"
                            height: 5
                            radius: 5 / 2
                            width: 5
                        }
                    }
                }
            }
            //-----------title_end

            //client_area
            // Loader {
            //     id: localer
            //
            //     Layout.fillHeight: true
            //     Layout.preferredWidth: parent.width
            // }
            // client_area
            Item {
                Layout.fillHeight: true
                Layout.margins: 20
                Layout.preferredWidth: 250

                MusicComponent {
                    anchors.right: parent.right
                    m_height: 170
                    m_width: parent.width
                }
            }
        }
    }
}