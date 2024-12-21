import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Qt.ImageLoader
Item {
    id: rootItem

    property string image_source

    RoundImage {
        id: backgroundItem

        anchors.fill: parent
        radius: 15
        fillMode:Image.PreserveAspectCrop
        //  source: "file:///D:/Doga/WallPaper/51D9E2A61426E0DD3D05E90F74DCAA17.jpg"
        source:"file:///"+rootItem.image_source
    }
    Item {
        id: maskItem

        anchors.fill: parent
        layer.enabled: true
        layer.smooth: true
        visible: false

        PanelMask {
            item: toolbar
        }
    }
    MultiEffect {
        anchors.fill: parent
        autoPaddingEnabled: false
        blur: 1
        blurEnabled: true
        blurMax: 80
        brightness: 0.1
        maskEnabled: true
        maskSource: maskItem
        maskSpreadAtMin: 1.0
        maskThresholdMin: 0.5
        saturation: 0.5 //饱和度
        source: backgroundItem
    }
    Item {
        id: panelArea

        anchors.fill: parent

        Panel {
            id: toolbar

            anchors.fill: parent
        }
    }

    component Panel: Rectangle {
        id: component

        property real blurValue
        property int m_radius: radius
        property string text

        antialiasing: true
        border.color: "#8090A0"
        border.width: 1
        color: "transparent"
        radius: 15
    }
    component PanelMask: Rectangle {
        required property Item item

        antialiasing: true
        height: item.height
        radius: item.m_radius
        width: item.width
        x: item.x
        y: item.y
    }
}
