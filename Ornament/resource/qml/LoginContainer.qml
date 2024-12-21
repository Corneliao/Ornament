import QtQuick
import QtQuick.Layouts


Rectangle {
    id:login_root
    signal  loginSucceed();
    objectName:"root"

    implicitHeight:450
    implicitWidth:320

    ColumnLayout {
        anchors.fill:parent
        objectName:"lay"
        function adjustResizeConentMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin;
            anchors.topMargin = top_margin;
            anchors.rightMargin = right_margin;
            anchors.bottomMargin = bottom_margin;
        }

        Item {
            id:title_bar
            Layout.alignment:Qt.AlignTop
            Layout.fillWidth:true
            Layout.preferredHeight:40
            objectName:"title_bar"
        }
        Image {
            Layout.preferredHeight:60
            Layout.preferredWidth:60
            source:"qrc:/resource/ico/LOGO.png"
            mipmap:true
            antialiasing:true
            fillMode:Image.PreserveAspectFit
            smooth:true
            Layout.alignment:Qt.AlignCenter
        }
        Loader {
            Layout.fillWidth:true
            Layout.fillHeight:true
        }
    }
}