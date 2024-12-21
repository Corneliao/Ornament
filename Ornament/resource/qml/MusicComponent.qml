import QtQuick
import Qt5Compat.GraphicalEffects
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.ImageLoader
Item {
    id: root

    property alias m_height: root.height
    property alias m_width: root.width

    antialiasing: true

    RadiusMaoboliEffect {
        id: effect_image

        anchors.fill: parent
        image_source: music_image_source.imageUrl
    }
    ColumnLayout {
        anchors.fill: parent
        antialiasing: true

        Item {
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.preferredHeight: 60
            Layout.rightMargin: 10
            Layout.topMargin: 10

            RowLayout {
                anchors.fill: parent
                antialiasing: true

                Rectangle {
                    id: music_image

                    Layout.preferredHeight: 60
                    Layout.preferredWidth: 60
                    color: "transparent"

                    ImageLoader {
                        id: music_image_source

                        height: 60
                        radius: 20
                        imageWidth:60
                        imageHeight:60
                        imageUrl: "D:/Doga/WallPaper/_20241120222209.jpg"
                        width: 60
                        windowDpi:global.WindowDpi
                        Component.onCompleted: {
                            effect_image.image_source = imageUrl;
                        }
                    }
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: music_image.height

                    Loader {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 10
                    }
                    Text {
                        antialiasing: true
                        color: "white"
                        font.bold: true
                        font.pixelSize: 15
                        horizontalAlignment: Text.AlignHCenter
                        text: "开不了口"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        antialiasing: true
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignLeft
                        text: "周杰伦"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Loader {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 10
                    }
                }
                Loader {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 10
                }
            }
        }
        Loader {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.preferredHeight: time_.height
            Layout.rightMargin: 20
            antialiasing: true

            Text {
                id: time_

                color: "white"
                horizontalAlignment: Text.AlignHCenter
                text: "1:00"
                verticalAlignment: Text.AlignVCenter
            }
            Loader {
                Layout.fillWidth: true
                Layout.preferredHeight: 10
            }
            Text {
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                text: "1:00"
                verticalAlignment: Text.AlignVCenter
            }
        }
        Slider {
            id: slider

            Layout.alignment: Qt.AlignHCenter
            Layout.preferredHeight: 10
            Layout.preferredWidth: parent.width - 20
            Material.accent: "white"
            antialiasing: true
            value: 0.5
        }
        Loader {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Layout.bottomMargin: 10
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            antialiasing: true

            Image {
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                antialiasing: true
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                source: "qrc:/resource/ico/SiPlayPreviousFill.png"

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.PointingHandCursor
                }
            }
            Loader {
                Layout.fillWidth: true
                Layout.preferredWidth: 10
            }
            Image {
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                antialiasing: true
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                source: "qrc:/resource/ico/SolarPlayBold.png"

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.PointingHandCursor
                }
            }
            Loader {
                Layout.fillWidth: true
                Layout.preferredWidth: 10
            }
            Image {
                Layout.preferredHeight: 20
                Layout.preferredWidth: 20
                antialiasing: true
                fillMode: Image.PreserveAspectFit
                mipmap: true
                smooth: true
                source: "qrc:/resource/ico/SiPlayNextFill.png"

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: cursorShape = Qt.PointingHandCursor
                }
            }
        }
    }
}
