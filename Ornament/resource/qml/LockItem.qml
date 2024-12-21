import QtQuick
import Qt5Compat.GraphicalEffects
import QtQuick.Effects

Item {
    property alias color: rect_.color

    Rectangle {
        id: rect_

        anchors.fill: parent
        radius: 15

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            font.pixelSize: 14
            text: "Item Lock"
        }
    }
}

