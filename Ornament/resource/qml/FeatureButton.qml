import QtQuick

Item {
    height: 30
    width: 30

    Rectangle {
        id: rect_

        anchors.fill: parent
        color: "transparent"
        radius: 5

        Image {
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            height: parent.width - 10
            smooth: true
            source: "qrc:/resource/ico/IconoirHomeSimpleDoor.png"
            width: parent.width - 10
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onEntered: {
                rect_.color = Qt.rgba(0 / 225, 0 / 255, 0 / 255, 30 / 255);
                cursorShape = Qt.PointingHandCursor;
            }
            onExited: {
                rect_.color = "transparent";
            }
        }
    }
}