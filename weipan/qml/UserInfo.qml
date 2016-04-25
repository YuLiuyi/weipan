import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: userInfo_pg

    property string id: ""
    property string name: ""
    property string gender: ""
    property string quota: ""
    property string consumed: ""
    property int  allSize: 0
    property int usedSize: 0

    Component.onCompleted: {
        userInfo_pg.id = contrl.showUserInfo(0)
        userInfo_pg.name = contrl.showUserInfo(1)
        userInfo_pg.gender = contrl.showUserInfo(2)
        userInfo_pg.quota = contrl.showUserInfo(3)
        userInfo_pg.consumed = contrl.showUserInfo(4)
        userInfo_pg.allSize = parseInt(userInfo_pg.quota, 10)
        userInfo_pg.usedSize = parseInt(userInfo_pg.consumed, 10)
    }

    function formatSize(fileSizeInBytes) {
        var i = -1;
        var byteUnits = [' kB', ' MB', ' GB', ' TB', 'PB', 'EB', 'ZB', 'YB'];
        do {
            fileSizeInBytes = fileSizeInBytes / 1024;
            i++;
        } while (fileSizeInBytes > 1024);

        return Math.max(fileSizeInBytes, 0.1).toFixed(1) + byteUnits[i];
    }


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
            anchors.top: rect.bottom
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 40
            anchors.verticalCenter: parent.verticalCenter

            Rectangle {
                width: parent.width
                height: 120
                Text {
                    color: "#513e3e"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 30
                    text: "ID号 : "+id

                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    color: gUiConst.getValue("CL1")
                }
            }
            Rectangle {
                width: parent.width
                height: 120
                Text {
                    color: "#513e3e"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 30
                    text: "用户名 : "+name
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    color: gUiConst.getValue("CL1")
                }
            }
            Rectangle {
                width: parent.width
                height: 120
                Text {
                    color: "#513e3e"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 30
                    text: "性别 : "+gender
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    color: gUiConst.getValue("CL1")
                }

            }
            Rectangle {
                height: 120
                width: parent.width
                Text {
                    color: "#513e3e"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 30
                    text: "总容量 : "+formatSize(allSize)
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    color: gUiConst.getValue("CL1")                }

            }
            Rectangle {
                height: 120
                width: parent.width
                Text {
                    color: "#513e3e"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 30
                    text: "已用容量 : "+formatSize(usedSize)
                }
                Rectangle {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 2
                    color: gUiConst.getValue("CL1")
                }
            }
        }
    }
}
