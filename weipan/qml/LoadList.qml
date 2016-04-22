import QtQuick 2.0
import QtQuick.Controls 1.2
import com.syberos.basewidgets 2.0
import Qt.labs.folderlistmodel 1.0

CPage {
    id: mainpg_pg

    property string c_path: ""
    property string baseDir: ""
    property date currentDate: new Date()
    property string c_name: ""
    property string c_date: ""
    property bool ismuliSelected: true

    onStatusChanged: {
        console.log("main status==="+status);

        if (status == CPageStatus.Show) {
            console.log("[mainpg_pg] ")

        }
    }

    Connections{
        target: contrl

        onUploadFinished :{
            console.log("upload finished!")
            gToast.requestToast("上传完成！")
        }
    }

    contentAreaItem: Item {
        id: loadList
        anchors.fill: parent

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
                text: "传输列表"
            }
        }


        //文件或者目录信息

        CSwipeTabView {
            id: tb
            width: parent.width
            anchors.top: rect.bottom
            anchors.bottom: parent.bottom
            visible: true

            tabBar: CSwipeTabBar {
                tabView: tb
                anchors.left: parent.left
                anchors.leftMargin: 100
                anchors.right: parent.right
                anchors.rightMargin: 100
                tabSpacing: 164
                CShadowLine {
                    anchors.left: parent.left
                    anchors.leftMargin: -100
                    anchors.right: parent.right
                    anchors.rightMargin: -100
                    anchors.top: parent.bottom
                }
            }

            CSwipeTab {
                id: loadertrack
                tabName: "上传列表"
                ListView {
                    id: uploadListView
                    anchors.fill: parent
                    model: listmodel
                    clip: true
                    delegate: Rectangle {
                        id: item
                        height: 160
                        width: parent.width
                        Rectangle{
                            id: decorateLine
                            color: gUiConst.getValue("CL1")
                            height: 2
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 40
                        }

                        Row {
                            width: parent.width
                            height: parent.height - 2 * 30
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 40
                            spacing: 5

                            Image {
                                id: img
                                source: "qrc:/images/res/3document.png"
                                width: 80
                                height: 80
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                            }


                            Column {
                                height: parent.height
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: img.right
                                anchors.leftMargin: 40
                                anchors.right: parent.right
                                spacing: 5

                                Text {
                                    height: parent.height / 2
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.rightMargin: view.editing ? 80 : 0
                                    text:  filename
                                    color: "#000000"
                                    font.pixelSize:   20
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                }

                                Row {
                                    width: parent.width
                                    height: parent.height / 2
                                    spacing: 5

                                    ProgressBar {
                                        id: progressBar
                                        width: parent.width-60
                                        height: 30
                                        maximumValue: 100
                                        minimumValue: 0
                                        value: progress
                                    }

                                    Text {
                                        id: dateInfo
                                        width: 50
                                        height: parent.height
                                        font.pixelSize:   20
                                        text: progressBar.value+"%"
                                        horizontalAlignment: Text.AlignRight
                                        anchors.right:parent.right
                                    }
                                }
                            }
                        }
                    }
                }
            }
            CSwipeTab {
                id: loaderartist
                tabName: "下载列表"
                ListView {
                    id: downloadListView
                    anchors.fill: parent
                    model: downListmodel
                    clip: true
                    delegate: Rectangle {
                        id: d_item
                        height: 160
                        width: parent.width
                        Rectangle{
                            id: d_decorateLine
                            color: gUiConst.getValue("CL1")
                            height: 2
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 40
                        }

                        Row {
                            width: parent.width
                            height: parent.height - 2 * 30
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 40
                            anchors.right: parent.right
                            anchors.rightMargin: 40
                            spacing: 5

                            Image {
                                id: d_img
                                source: "qrc:/images/res/3document.png"
                                width: 80
                                height: 80
                                anchors.left: parent.left
                                anchors.verticalCenter: parent.verticalCenter
                            }


                            Column {
                                height: parent.height
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: d_img.right
                                anchors.leftMargin: 40
                                anchors.right: parent.right
                                spacing: 5

                                Text {
                                    height: parent.height / 2
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    anchors.rightMargin: view.editing ? 80 : 0
                                    text:  filename
                                    color: "#000000"
                                    font.pixelSize:   20
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                }

                                Row {
                                    width: parent.width
                                    height: parent.height / 2
                                    spacing: 5

                                    ProgressBar {
                                        id: d_progressBar
                                        width: parent.width-60
                                        height: 40
                                        maximumValue: 100
                                        minimumValue: 0
                                        value: progress
                                    }

                                    Text {
                                        id: d_dateInfo
                                        width: 50
                                        height: parent.height
                                        font.pixelSize:   20
                                        text: d_progressBar.value+"%"
                                        horizontalAlignment: Text.AlignRight
                                        anchors.right:parent.right
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
