import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import com.syberos.basewidgets 2.0
import Qt.labs.folderlistmodel 1.0
import com.syberos.filemanager.filepicker 1.0

CPage {
    id: showPage

    property string currentpath: ""
    property string baseDir: ""
    property date currentDate: new Date()
    property string c_name: ""
    property string c_date: ""
    property var rmIndexList: []
    property bool ismuliSelected: true
    property string c_title: ""
    property string c_path: ""
    property int c_index: 0

    onStatusChanged: {
        console.log("status==="+status);

        indicator.visible = true;
        if (status == CPageStatus.Show) {
            console.log("[show111page] c_path: ", c_path)
            mainListModel.clear()
            contrl.reqMetaData(c_path);
            indicator.visible = false;
        }
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
                text: c_title
            }
        }

        CEditTitleBar {
            id: editTitleBar
            anchors.top: rect.bottom
            width: parent.width
            editView: view
            titleItemEnabled: true
            titlePixelSize: editView.editing ? gUiConst.getValue("S5") : gUiConst.getValue("S1")
            titleTextColor: gUiConst.getValue("CT2")
            rightItemEnabled: false
            leftItemSize: editView.editing? Qt.size(120,60) : Qt.size(40, 40)
            leftItemBackgroundEnabled: false
            spacingBetweenLeftBorderAndLeftItem: editView.editing ? 40 : 20
            spacingBetweenLeftItemAndTitleItem: 20
            editLeftItemPixelSize:gUiConst.getValue("S5")
            onLeftItemTriggered: {
                pageStack.pop();
            }

            Text {
                id: titileText
                text:(qsTr("列表"))
                anchors.left: editTitleBar.left
                anchors.leftMargin: 80
                anchors.verticalCenter: editTitleBar.verticalCenter
                font.pixelSize: gUiConst.getValue("S1")
                color: gUiConst.getValue("CT1")
                opacity: view.editing ? 0 : 1
                Behavior on opacity {
                    NumberAnimation { duration: 150}
                }
            }
        }

        CEditListView {
            id: view
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: editTitleBar.bottom
            anchors.bottom: {
                if(view.editing){
                    return toolbar.top
                }else{
                    return tab_btn.top
                }
            }

            model: mainListModel
            clip: true

            signal selectionStateChanged

            delegate: CEditListViewDelegate {
                id: _delegate
                width: view.width
                height: 120

                onClicked:  {
                    console.log("clicked")
                    mainListModel.currentIndex = index;
                    if(mainListModel.getType(index)) {
                        console.log("is wen jian jia==========currentPath===="+mainListModel.getPath(index))
                        var txt_title = mainListModel.getTitle(index)
                        var txt_path = mainListModel.getPath(index)
                        // enter the folder
                        pageStack.push("qrc:///qml/ShowPage.qml", {c_title: txt_title, c_path: txt_path})

                    }else if(!mainListModel.getType(index)) {
                        console.log("is jian jia")
                        // if the file is not downloaded, need to download, then open the file.
                        var txt1_path = mainListModel.getPath(index)
                        var txt_title1 = mainListModel.getTitle(index)
                        var txt_content = contrl.readFile("/home/user/"+txt_title1)
                        gToast.requestToast("目前还不能打开文件")
//                        contrl.getDwnloadPath(txt1_path)
//                        contrl.reqDownloadFile()
//                        indicator.visible = true
                        // open the file
                        console.log("title===" + txt_title1 + "  content===" + txt_content)
//                        pageStack.push("qrc:///qml/ShowFile.qml",{title: txt_title1,text: txt_content})

                    }
                }

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
                        source: type?"qrc:/images/res/folder.png":"qrc:/images/res/3document.png"
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
                            id: titleTxtInfo
                            height: parent.height / 2
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.rightMargin: view.editing ? 80 : 0
                            text:  title
                            color: "#000000"
                            font.pixelSize:   30
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                        }

                        Row {
                            width: parent.width
                            height: parent.height / 2
                            spacing: 5

                            Text {
                                id: sizeInfo
                                width: parent.width / 2
                                height: parent.height
                                font.pixelSize:   20
                                text: /*type? size+"个文件":*/size
                                color: "#787777"
                            }

                            Text {
                                id: dateInfo
                                width: parent.width / 2  - 10
                                height: parent.height
                                font.pixelSize:   20
                                text: date
                                color: "#787777"
                                horizontalAlignment: Text.AlignRight
                                anchors.right:parent.right
                                anchors.rightMargin: view.editing ? 80 : 0
                            }
                        }
                    }

                }
            }
        }

        property var listCheckedCount: view.selectedCount

        onListCheckedCountChanged: {
            //            if(listCheckedCount > 0){
            //                toolbar.enabledIndexes([0,1,2], true)
            //            }else{
            //                toolbar.enabledIndexes([0,1,2], false)
            //            }

            if(listCheckedCount === 1){
                toolbar.enabledIndexes([0,1,2], true)
            }else{
                toolbar.enabledIndexes([0,1,2], false)
            }
        }

        CFlowToolBar {
            id: toolbar
            anchors.bottom: tab_btn.top
            width:parent.width
            animationVisible: view.editing
            backgroundEnabled: true

            names: [os.i18n.ctr(qsTr("分享")), os.i18n.ctr(qsTr("删除")), os.i18n.ctr(qsTr("下载"))]

            onClicked: {
                var indexList = view.selectedIndexes[0]
                page.c_index = view.selectedIndexes[0]
                if(index == 0){
                    var path = mainListModel.getPath(indexList)

                    gToast.requestToast("暂不支持分享!")
                    //view.editing = false;
//                    shareDialog.open("file://"+path, CMIMEDialogTool.Share, "")

                }else if(index == 1){

                    console.log("=====my count ============",view.selectedIndexes.length)
                    confirmDeleteDialog.show()

                }else if(index == 2) {
                    console.log("======download file=============", view.selectedIndexes.length)
                    var type = mainListModel.getType(indexList)
                    if(type) {
                        toolbar.enabledIndexes([2], false);
                        gToast.requestToast("不能下载文件夹!");
                    }
                    else {
                        var d_path = mainListModel.getPath(indexList)
                        console.log("d_path = "+ d_path)
                        contrl.getDwnloadPath(d_path)
                        contrl.reqDownloadFile()
                        view.editing = false
//                        indicator.visible = true
                    }
                }
            }
        }

        CMIMEDialog {
            id: shareDialog
        }

        CDialog {
            id: confirmDeleteDialog
            visible: false
            titleText: qsTr("删除")
            titleAreaEnabled: true;
            messageText:  mainListModel.getType(page.c_index)?"确定删除选定的文件夹?":"确定删除选定的文件?"
            onAccepted:  {

                console.log("RonAccepted.")
                view.editing = false
                visible = false
                var path = mainListModel.getPath(page.c_index);
                console.log("------------mainListModel.getPath---------------------",mainListModel.getPath(page.c_index))
                mainListModel.remove(page.c_index, page.c_index)
                contrl.reqDeleteFile(path)
            }
            onRejected: {
                view.editing = false
            }
        }

        Connections{
            target: mainListModel

            onDeleteFinished:{
                console.log("delete finished");
                indicatorDialog.hide()
            }
        }

        CIndicatorDialog{
            id:indicatorDialog
        }

        Row {
            id: tab_btn
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: create_btn
//                        text: "新建"
                width: 240
                height: 80
                style: ButtonStyle {
                  label: Text {
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    text: "新建"
                  }
                }
                onClicked: {
                    console.log("create")
                    view.editing = false;
                    fileName_Dlg.show();
                }
            }


            Button {
                id: upload_btn
//                        text: "上传"
                width: 240
                height: 80
                style: ButtonStyle {
                  label: Text {
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    text: "上传"
                  }
                }
                onClicked: {
                    console.log("upload")
                    view.editing = false;
                    pageStack.push(filesPickerCom)                       }

            }
            Button {
                id: loadList_btn
                width: 240
                height: 80

                style: ButtonStyle {
                  label: Text {
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Helvetica"
                    font.pixelSize: 30
                    color: "black"
                    text: "传输列表"
                  }
                }
                onClicked: {
                    view.editing = false;
                    pageStack.push("qrc:///qml/LoadList.qml")
                }
            }
        }

        InputDialog {
            id: fileName_Dlg
            titleText: "新建文件夹"
            placeholderText: "请输入文件名..."
            onAccepted: {
                var value = fileName_Dlg.text();
                console.log("accept is result is:" + value);

                if(value === "") {
                    return;
                }

                if (folderName === value) {
                    //inputDialog.close();
                    return;
                }

                var f_path = c_path;

                contrl.createFolder(value, f_path);

                fileName_Dlg.setText("");
            }
        }

        Component {
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
                    page.filename = filePath;
                    console.log("filePicker filesPath = "+filePath)
                    contrl.getUploadFilePath(filePath);
                    contrl.reqUploadFile(c_path)
                    //                    pageStack.push("qrc:///qml/LoadList.qml")
                    //                    indicator.visible = true;
                    window.clearFocus();
                }
            }
        }

        CIndicator{
            id: indicator
            anchors.fill: parent
            visible: false
        }
    }
}
