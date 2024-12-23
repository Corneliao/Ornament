import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.ImageLoader
Rectangle {
    id:login_root
    signal  logining(string account,string password);
    signal userAccountChanged(string account)
    objectName:"root"
    property real windowDpi:0.0
    property var imageHead:":/resource/ico/LOGO.png"
    property alias imageData: user_head.imageByteArray
    property alias roundImage: user_head.isRoundImage
    property bool fromImageData:false
    anchors.fill:parent

    MouseArea {
        anchors.fill:parent
        onClicked: forceActiveFocus()
    }

    ColumnLayout {
        anchors.fill:parent
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
        }
        Rectangle {
            Layout.preferredHeight:80
            Layout.preferredWidth:80
            radius:80/2
            Layout.alignment:Qt.AlignCenter

            ImageLoader {
                id:user_head
                width:70
                height:70
                imageWidth:70
                imageHeight:70
                windowDpi:login_root.windowDpi
                anchors.centerIn:parent
                imageUrl:imageHead
                isFromData:login_root.fromImageData
                isRoundImage:login_root.roundImage
            }
        }

        Loader {
            Layout.preferredHeight:20
            Layout.preferredWidth:10
        }
        TextField {
            id:account_
            Layout.preferredWidth:parent.width-80
            Layout.preferredHeight:40
            placeholderText:"账号"
            Layout.alignment:Qt.AlignCenter
            Material.elevation:6
            Material.accent:"red"
            maximumLength:9
            onTextChanged:  {
                if(account_.length === 9)
                    login_root.userAccountChanged(account_.text)
            }
        }
        Loader {

            Layout.preferredHeight:20
            Layout.preferredWidth:10
        }
        TextField {
            id:password_
            Layout.preferredWidth:parent.width-80
            Layout.preferredHeight:40
            placeholderText:"密码"
            Layout.alignment:Qt.AlignCenter
            Material.elevation:6
            Material.accent:"red"
            echoMode:TextField.Password
        }

        Loader {
            Layout.fillWidth:true
            Layout.fillHeight:true
        }

        Button  {
            Layout.preferredWidth:120
            Layout.preferredHeight:50
            Layout.bottomMargin:20
            Material.accent:"red"
            Layout.alignment:Qt.AlignCenter
            Text {
                anchors.centerIn:parent
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
                text:"登录"
                color:"red"
                font.pixelSize:15
            }
            onClicked: {
                if(account_.length <= 0 || password_.length <=0)
                    return;
                login_root.logining(account_.text,password_.text)
            }
        }
    }
    // gradient: Gradient {
    //     GradientStop { position: 0.3; color: "#8BC6EC" }
    //     GradientStop { position: 0.7; color: "#9599E2" }
    // }
}