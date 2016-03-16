import QtQuick 2.0
import com.syberos.basewidgets 2.0
//import ListModel 1.0

CPage {
    id: mainpg_pg

    contentAreaItem: Item {
        Rectangle {
            id: rect
            width: parent.width
            height: 60
            color: "#7a6767"

            Text {
                id: title_txt
                color: "#5a3939"
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }

                font.pixelSize: 40
                font.bold: true
                text: "个人"
            }
        }

        Column {
            id: msg
            spacing: 0
            anchors.top: rect.bottom
            anchors.left: parent.left
            anchors.leftMargin: 20
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
            Text {
                width: parent.width
                font.pixelSize: 25
                height: 60
                text: "ID : "+contrl.showUserInfo(0)
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#807373"
                }
            }
            Text {
                width: parent.width
                font.pixelSize: 25
                height: 60
                text: "Name : "+contrl.showUserInfo(1)
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#807373"
                }
            }
            Text {
                width: parent.width
                font.pixelSize: 25
                height: 60
                text: "Gender : "+contrl.showUserInfo(2)
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#807373"
                }

            }
            Text {
                width: parent.width
                font.pixelSize: 25
                height: 60
                text: "Quota : "+contrl.showUserInfo(3)
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#807373"
                }
            }
            Text {
                width: parent.width
                font.pixelSize: 25
                height: 60
                text: "Consumed : "+contrl.showUserInfo(4)
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 1
                    color: "#807373"
                }
            }
        }
    }
}
