import QtQuick
import Qt5Compat.GraphicalEffects

Item {
    id: round_image

    property alias fillMode: _image.fillMode
    property alias radius: _mask.radius
    property alias slace: _image.scale
    property alias source: _image.source

    Image {
        id: _image

        anchors.fill: parent
        antialiasing: true
        fillMode: Image.PreserveAspectFit
        mipmap: true
        visible: false
    }
    Rectangle {
        id: _mask

        anchors.fill: parent
        antialiasing: true
        color: "black"
        visible: false
    }
    OpacityMask {
        id: mask_image

        anchors.fill: round_image
        antialiasing: true
        maskSource: _mask
        source: _image
        visible: true
    }
}

