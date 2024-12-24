import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.ImageLoader


Item {
    id: message_item
    property string message
    width: ListView.view.width
    height: 40
    property string direction:"recv"
    property alias userHeadData: userHead.imageByteArray

    Label  {
        visible:false
        id:temp
    }

    Component.onCompleted: {
        temp.text = message_item.message
    }

    //消息气泡自适应列表宽度
    function wrapText() {
        // temp.text = message_item.message
        let textwidth = temp.width + 80
        if(textwidth > message_item.ListView.view.width)  {
            message_rect.Layout.preferredWidth = message_item.ListView.view.width - 80
            label.width = message_rect.Layout.preferredWidth-20
            message_item.height = message_rect.height + 5
        }
        else {
            label.text = message_item.message
            message_rect.Layout.preferredWidth =  label.width + 20
            message_item.height = message_rect.height + 10
        }
        //

        // let width = temp.width
        // let textWidth =width + 120
        // console.log(textWidth, message_item.ListView.view.width)
        // if(textWidth > message_item.ListView.view.width)  {
        //     message_rect.width  = message_item.ListView.view.width - 80
        //     message_item.height = message_rect.height + 25
        // }
        // else {
        //     message_rect.width = textWidth
        //     message_rect.text = message_item.message
        //     message_item.height = message_rect.height + 20
        //     console.log("dsads");
        // }
    }


    MouseArea {
        anchors.fill:parent
        onClicked:  {
            console.log(message_item.message)
        }
    }

    state: direction
    states: [
        State {
            name: "recv"
            PropertyChanges {
                target:message_layout
                layoutDirection:Qt.LeftToRight
            }
        },
        State {
            name: "send"
            PropertyChanges {
                target:message_layout
                layoutDirection:Qt.RightToLeft
            }
        }
    ]

    RowLayout {
        id:message_layout
        anchors.fill: parent


        ImageLoader {
            id: userHead
            isRoundImage: true
            isFromData: true
            width: 40
            height: 40
            imageWidth: 40
            imageHeight: 40
            windowDpi: global.windowDpi
            Layout.alignment:Qt.AlignTop
        }

        Loader {
            Layout.preferredHeight:1
            Layout.preferredWidth:3
        }


        //方式-
        Rectangle {
            id:message_rect
            Layout.preferredWidth:label.width + 20
            Layout.preferredHeight:label.height + 10
            radius:7
            color: "#80D0C7"
            Text  {
                id:label
                text:message_item.message
                wrapMode:Label.WrapAnywhere
                anchors.centerIn:parent
                color:"white"
                font.pixelSize:15
            }
            onHeightChanged: {
                if(message_rect.height > message_item.height)  {
                    message_item.height =  message_rect.height + 10
                    message_rect.Layout.alignment  = Qt.AlignTop
                }
                else {
                    message_rect.Layout.alignment  = Qt.AlignCenter
                    message_item.height = message_rect.height +10
                }
                message_item.height = message_rect.height +10
            }
            Component.onCompleted:{
                if (message_rect.Layout.preferredWidth  + 40 > message_item.ListView.view.width) {
                    message_rect.width = message_item.ListView.view.width - 80
                    label.width = message_rect.width-20
                    message_item.height = message_rect.height +5
                }

                if(message_rect.height  > message_item.height)  {
                    message_item.height = message_rect.height + 5
                    message_rect.Layout.alignment = Qt.AlignTop
                }
            }
        }



        // TextArea {
        //     id:message_rect
        //     text: message_item.message
        //     color:"white"
        //     wrapMode:TextArea.WrapAnywhere
        //     font.pixelSize:15
        //     readOnly:true
        //     background:Rectangle {
        //         anchors.fill:parent
        //         color: "#80D0C7"
        //         radius:7
        //     }
        //
        //     Component.onCompleted: {
        //         if(message_rect.width > message_item.ListView.view.width) {
        //             message_rect.width  = message_item.ListView.view.width - 80
        //         }
        //
        //         if(message_rect.height > message_item.height)  {
        //             message_item.height =  message_rect.height + 10
        //             message_rect.Layout.alignment  = Qt.AlignTop
        //             console.log("大")
        //         }
        //
        //
        //     }
        //     onHeightChanged: {
        //         if(message_rect.height > message_item.height)  {
        //             message_item.height =  message_rect.height + 5
        //             message_rect.Layout.alignment  = Qt.AlignTop
        //         }
        //         else {
        //             message_rect.Layout.alignment  = Qt.AlignCenter
        //             message_item.height = message_rect.height +10
        //         }
        //     }
        // }

        Loader {
            Layout.preferredHeight: 5
            Layout.fillWidth: true
        }
    }
}