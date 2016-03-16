import QtQuick 2.0
import QtQuick.Dialogs 1.0
import com.syberos.basewidgets 2.0
import com.syberos.filemanager.filepicker 1.0

CPage {
    id: mainpg_pg

    property string webHtml: ""
    property var errorAddress: []
    property int allAttachSize: 0
    property int maxAttachSize: 1024*1024*10       //附件大小最大支持10M
    property var recipientsItem

    contentAreaItem: Item {
        Connections{
            target: contrl
            onUploadFinished :{
                console.log("upload finished!")
                indicator.visible = false;
                gToast.requestToast("上传完成！");
            }
        }

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
                text: "Upload"
            }
        }

        Row {
            id: upload_ctrl
            anchors.top: rect.bottom
            anchors.topMargin: 400
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20
            Text {
                font.pixelSize: 30
                text: "choose file:"
                anchors.verticalCenter: cho_btn.verticalCenter
            }

            CButton {
                id: cho_btn
                width: 160
                height: 100
                text: "..."
                onClicked: {
                    pageStack.push(filesPickerCom)
                }
            }

            Text {
                font.pixelSize: 30
                text: "no file"
                anchors.verticalCenter: cho_btn.verticalCenter
            }
        }

        CButton {
            id: upload_btn
            anchors.top: upload_ctrl.bottom
            anchors.topMargin: 200
            anchors.horizontalCenter: parent.horizontalCenter
            width: 200
            height: 100
            text: "upload"
            onClicked: {
                contrl.reqUploadFile()
                indicator.visible = true;
            }
        }
        // file list for select attach
        Component{
            id: filesPickerCom
            SyberosFilesPicker{
                id: filesPicker
                onOk:{
                    var size = 0;
                    var selectedSize = filesPicker.getFileSize()
                    for(var i = 0; i < selectedSize.length; i++){
                        size += parseInt(selectedSize[i])
                    }

                    var filePath = filesPicker.filesPath;
                    console.log("filePicker filesPath = "+filePath)
                    contrl.getUploadFilePath(filePath);
                    window.clearFocus()
                }
            }
        }

        Keys.onReleased: {
            if((event.key === Qt.Key_Back || event.key === Qt.Key_Escape))
            {
                if(gInputContext.softwareInputPanelVisible) {
                    window.clearFocus()
                }else {
                    saveMailCheck(true, false)
                }
                event.accepted = true;
                return;
            }
        }
        CIndicator{
            id: indicator
            anchors.fill: parent
            visible: false
        }
    }
}
