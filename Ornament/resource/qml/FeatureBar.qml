import QtQuick
import QtQuick.Layouts

Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        FeatureButton {
        }
        FeatureButton {
        }
        Loader {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width
        }
    }
}