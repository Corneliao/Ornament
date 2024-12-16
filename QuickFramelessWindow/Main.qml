import QtQuick
import QtQuick.Controls
import Qt.QuickFramelessWindow 1.0
import QtQuick.Controls
import QtQuick.Layouts

FramelessWindow {
    id: main_window

    minimumHeight: 750
    minimumWidth: 1075
    visible: true

    Component.onCompleted: {
        main_window.x = (Screen.desktopAvailableWidth - main_window.width) / 2;
        main_window.y = (Screen.desktopAvailableHeight - main_window.height) / 2;
        main_window.setTitleBar(title_layout); //设置标题栏
    }

    ColumnLayout {
        id: main_content_layout

        function adjustResizeConentMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin;
            anchors.topMargin = top_margin;
            anchors.rightMargin = right_margin;
            anchors.bottomMargin = bottom_margin;
        }

        anchors.fill: parent
        objectName: "main_content_layout"

        Rectangle {
            id: title_bar

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "yellow"
            objectName: "title_bar"

            RowLayout {
                id: title_layout

                anchors.fill: parent
                anchors.margins: 5
                objectName: "title_layout"

                Button {
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 80
                    objectName: "close"

                    onClicked: {
                        main_window.close();
                    }

                    Text {
                        anchors.centerIn: parent
                        text: "close"
                    }
                }
                Button {
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredHeight: 30
                    Layout.preferredWidth: 80
                    objectName: "max"

                    onClicked: {
                        if (main_window.visibility === Window.Maximized) {
                            main_window.showNormal();
                        } else {
                            main_window.showMaximized();
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        text: "max"
                    }
                }
            }
        }
        Rectangle {
            Layout.preferredHeight: 40
            Layout.preferredWidth: 100

            Text {
                anchors.centerIn: parent
                text: "min"
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    main_window.showMinimized();
                }
            }
        }
    }
}
