import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.ImageLoader
import QtQuick.Dialogs
import Qt.labs.platform
Item {
    id: login_root

    signal  logining(string account, string password);
    signal  userAccountChanged(string account)
    signal  registerAccount(string image_path, string user_name, string passwrod)

    objectName: "root"
    property real windowDpi: 0.0
    property var imageHead: ":/resource/ico/LOGO.png"
    property alias imageData: user_head.imageByteArray
    property alias roundImage: user_head.isRoundImage
    property bool fromImageData: false
    property string selectUserHeadPath
    anchors.fill: parent



    FileDialog {
        id:file_dialog
        title: "选择头像"
        nameFilters:["(*.png *.jpg)"]
        acceptLabel: "确定"
        rejectLabel: "取消"
        fileMode:FileDialog.OpenFile
        onAccepted: {
            login_root.selectUserHeadPath = file_dialog.currentFile
            var item = file_dialog.currentFile.toString()
            item = item.slice(8)
            register_userhead.isRoundImage = true
            register_userhead.imageUrl = item
        }
    }

    ColumnLayout {

        function adjustResizeConentMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin;
            anchors.topMargin = top_margin;
            anchors.rightMargin = right_margin;
            anchors.bottomMargin = bottom_margin;
        }

        anchors.fill: parent
        Item {
            id: title_bar
            Layout.fillWidth: true
            Layout.preferredHeight: 30
        }
        SwipeView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"
                MouseArea {
                    anchors.fill: parent
                    onClicked: forceActiveFocus()
                }
                ColumnLayout {
                    anchors.fill: parent

                    ImageLoader {
                        id: user_head
                        Layout.preferredWidth: 70
                        Layout.preferredHeight: 70
                        Layout.alignment: Qt.AlignCenter
                        imageWidth: 70
                        imageHeight: 70
                        windowDpi: login_root.windowDpi
                        imageUrl: imageHead
                        isFromData: login_root.fromImageData
                        isRoundImage: login_root.roundImage
                    }
                    Loader {
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 10
                    }
                    TextField {
                        id: account_
                        Layout.preferredWidth: parent.width - 80
                        Layout.preferredHeight: 40
                        placeholderText: "账号"
                        Layout.alignment: Qt.AlignCenter
                        Material.elevation: 6
                        Material.accent: "red"
                        maximumLength: 9
                        validator: IntValidator {
                            bottom: 0; top: 999999999
                        }
                        onTextChanged: {
                            if (account_.length === 9)
                                login_root.userAccountChanged(account_.text)
                        }
                    }
                    Loader {

                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 10
                    }
                    TextField {
                        id: password_
                        Layout.preferredWidth: parent.width - 80
                        Layout.preferredHeight: 40
                        placeholderText: "密码"
                        Layout.alignment: Qt.AlignCenter
                        Material.elevation: 6
                        Material.accent: "red"
                        echoMode: TextField.Password
                    }

                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    Button {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 50
                        Layout.bottomMargin: 20
                        Material.accent: "red"
                        Layout.alignment: Qt.AlignCenter
                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "登录"
                            color: "red"
                            font.pixelSize: 15
                        }
                        onClicked: {
                            if (account_.length <= 0 || password_.length <= 0)
                                return;
                            login_root.logining(account_.text, password_.text)
                        }
                    }
                }
            }

            Rectangle {
                id: register_
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "transparent"
                ColumnLayout {
                    anchors.fill: parent

                    ImageLoader {
                        id: register_userhead
                        Layout.preferredWidth: 70
                        Layout.preferredHeight: 70
                        Layout.alignment: Qt.AlignCenter
                        imageWidth: 70
                        imageHeight: 70
                        windowDpi: login_root.windowDpi
                        imageUrl: imageHead

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                file_dialog.open()
                            }
                        }
                    }
                    Loader {
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 10
                    }
                    TextField {
                        id: register_account
                        Layout.preferredWidth: parent.width - 80
                        Layout.preferredHeight: 40
                        placeholderText: "昵称"
                        Layout.alignment: Qt.AlignCenter
                        Material.elevation: 6
                        Material.accent: "red"
                        maximumLength: 9
                    }
                    Loader {
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 10
                    }
                    TextField {
                        id: register_password
                        Layout.preferredWidth: parent.width - 80
                        Layout.preferredHeight: 40
                        placeholderText: "密码"
                        Layout.alignment: Qt.AlignCenter
                        Material.elevation: 6
                        Material.accent: "red"
                        echoMode: TextField.Password
                    }

                    Loader {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    Button {
                        Layout.preferredWidth: 120
                        Layout.preferredHeight: 50
                        Layout.bottomMargin: 20
                        Material.accent: "red"
                        Layout.alignment: Qt.AlignCenter
                        Text {
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            text: "注册"
                            color: "red"
                            font.pixelSize: 15
                        }
                        onClicked: {
                            if (register_account.length <= 0 || register_password.length <= 0)
                                return;
                            if(login_root.selectUserHeadPath.length <=0) {
                                console.log("请上传一个头像吧");
                                return
                            }
                            login_root.registerAccount(login_root.selectUserHeadPath,register_account.text, register_password.text)
                        }
                    }
                }
            }
        }
    }
}