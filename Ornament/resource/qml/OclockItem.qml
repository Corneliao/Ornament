import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import Qt5Compat.GraphicalEffects
import QtQuick.Controls

Rectangle {
    property alias imageSource: image_1.source
    property alias periodText: period_text.text
    property alias title: oclock_title.text

    color: "white"
    radius: 15

    // layer.enabled: true
    // radius: 15
    //
    // layer.effect: InnerShadow {
    //     color: "gray"
    //     radius: 15
    //     samples: 8
    //     spread: 0.1
    // }

    RowLayout {
        anchors.fill: parent

        RoundImage {
            id: image_1

            Layout.leftMargin: 5
            Layout.preferredHeight: 45
            Layout.preferredWidth: 45
            radius: 15
            source: "file:///D:\\Doga\\WallPaper\\_20241120222209.jpg"
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Loader {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    id: oclock_title

                    font.pixelSize: 11
                    renderType: Text.NativeRendering
                    text: "Camera 1"
                }
                Loader {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 10
                }
                Rectangle {
                    Layout.preferredHeight: 18
                    Layout.preferredWidth: 38
                    Layout.rightMargin: 10
                    color: "#eaeaea"
                    //color: "red"
                    radius: 15

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: false
                        font.pixelSize: 10
                        text: "on"
                    }
                    Text {
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        font.bold: false
                        font.pixelSize: 10
                        text: "off"
                    }
                    Rectangle {
                        id: slider

                        anchors.verticalCenter: parent.verticalCenter
                        antialiasing: true
                        color: "#4b4b4b"
                        height: parent.height
                        radius: parent.height / 2
                        width: parent.height

                        ColorAnimation on color {
                            id: color_animation

                            duration: 200
                        }

                        XAnimator {
                            id: x_animation

                            duration: 200
                            target: slider
                            to: slider.width
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onClicked: {
                                if (slider.x === 0) {
                                    x_animation.to = slider.width;
                                    color_animation.to = "#d5b6f9";
                                    x_animation.start();
                                    color_animation.start();
                                } else {
                                    x_animation.to = 0;
                                    color_animation.to = "#4b4b4b";
                                    x_animation.start();
                                    color_animation.start();
                                }
                            }
                            onEntered: {
                                cursorShape = Qt.PointingHandCursor;
                            }
                        }
                    }
                }
            }
            RowLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Image {
                    Layout.preferredHeight: 15
                    Layout.preferredWidth: 15
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    source: "/resource/ico/SolarClockCircleOutline.png"
                }
                Label {
                    id: period_text

                    font.pixelSize: 10
                    renderType: Text.NativeRendering
                    text: "Camera 1"
                }
            }
            Loader {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
