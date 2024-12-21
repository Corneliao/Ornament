import QtQuick

Rectangle {
    id: button

    property var ico_source: ""

    signal buttonClicked

    color: "transparent"
    height: 25
    radius: 5
    width: 25

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onClicked: {
            button.buttonClicked();
        }
        onEntered: {
            //button.color = "#e77272";
            button.color = Qt.rgba(0 / 255, 0 / 255, 0 / 255, 20 / 255);
            cursorShape = Qt.PointingHandCursor;
        }
        onExited: {
            button.color = "transparent";
        }
    }
    Image {
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        height: parent.width - 5
        smooth: true
        source: button.ico_source
        width: parent.width - 5
    }
}

