import QtQuick
import QtQuick.Layouts

Item {
    id: title

        signal closeWindowSignal
        signal maxWindowSignal
        signal minWindowSignal

    property var spacerItem:spacer_item
    Component.onCompleted: {
        min_button.buttonClicked.connect(title.minWindowSignal);
        max_button.buttonClicked.connect(title.maxWindowSignal);
        close_button.buttonClicked.connect(title.closeWindowSignal);
    }


    RowLayout {
        id: title_layout

        anchors.fill: parent
        objectName: "title_layout"

        Loader {
            id:spacer_item
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            objectName:"spacier"
        }
        TitleToolButton {
            id: min_button

            ico_source: "qrc:/resource/ico/MingcuteMinimizeLine.png"
        }
        TitleToolButton {
            id: max_button

            ico_source: "qrc:/resource/ico/FluentMaximize16Regular.png"
        }
        TitleToolButton {
            id: close_button

            ico_source: "qrc:/resource/ico/IconamoonClose.png"
        }
    }
}