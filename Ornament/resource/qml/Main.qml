import QtQuick
import Qt.FramelessWindow 1.0
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQml

FramelessWindow {
    id: main_window
    visible:true
    objectName :"root"

        signal closeWindowSignal
        signal maxWindowSignal
        signal minWindowSignal

    //显示主页
    function setStacklayoutCurrentIndex(index) {
        stack_layout.currentIndex = index;
        home_page.showHomeContainer();
    }

    function setWindowMaxSate()  {
        if(main_window.visibility === Window.Maximized)
            main_window.showNormal();
        else
            main_window.showMaximized();

    }

    function  closeWindow() {
        main_window.objectName ="quit"
        Qt.quit();
    }

    Component.onCompleted: {
        content_container.setStacklayoutCurrentIndex.connect(main_window.setStacklayoutCurrentIndex);
        main_window.setTitleBar(title_bar.spacerItem);

        //标题按钮的操作
        title_bar.minWindowSignal.connect(main_window.showMinimized);
        title_bar.closeWindowSignal.connect(main_window.closeWindow);
        title_bar.maxWindowSignal.connect(main_window.setWindowMaxSate);
        home_page.visible = false //程序启动时启动动画
        home_page.showHomeContainer();

        content_container.createFriendItem(global.userAccount,global.userName,global.userHead)
    }

    ColumnLayout {
        function adjustResizeConentMargins(left_margin, top_margin, right_margin, bottom_margin) {
            anchors.leftMargin = left_margin;
            anchors.topMargin = top_margin;
            anchors.rightMargin = right_margin;
            anchors.bottomMargin = bottom_margin;
        } 

        anchors.fill: parent
        anchors.margins: 0

        //title_bar
        TitleBar {
            id: title_bar

            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.leftMargin:20
            Layout.rightMargin:20
        }
        StackLayout {
            id: stack_layout

            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: 0

            HomeContainer {
                id: home_page
                Layout.fillHeight: true
                Layout.fillWidth: true

                onIsHideChanged: {
                    if (home_page.isHide) {
                        if (stack_layout.count > 1) {
                            content_container.setFeatureCurrentIndex();
                            stack_layout.currentIndex = 1;
                        }
                    }
                }
            }
            ContentContainer {
                id: content_container

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}

