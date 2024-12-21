import QtQuick
import QtQuick.Effects

Item {
    id: rootItem

    property Item image_item

    anchors.fill: parent

    //GlassEffect
    MultiEffect {
        anchors.fill: parent
        autoPaddingEnabled: false
        blurEnabled: true
        blurMax: 80
        brightness: 0.1
        maskEnabled: true
        maskSource: maskItem
        maskSpreadAtMin: 1.0
        maskThresholdMin: 0.5
        saturation: 0.1
        source: rootItem.image_item
    }
    Item {
        id: maskItem

        anchors.fill: parent
    }
}

