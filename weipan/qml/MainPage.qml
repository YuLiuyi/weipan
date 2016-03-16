import QtQuick 2.0
import com.syberos.basewidgets 2.0
import Qt.labs.folderlistmodel 1.0

CPage {
    id: mainpg_pg

    property string currentpath: ""
    property string baseDir: ""
    property date currentDate: new Date()
    property string c_name: ""
    property string c_date: ""
    property var rmIndexList: []
    property bool ismuliSelected: true

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
                text: "首页"
            }
        }

        //文件或者目录信息

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
                contrl.reqMetaData("");
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
                        contrl.reqMetaData(mainListModel.getPath(index));
                        // enter the folder

                    }else if(!mainListModel.getType(index)) {
                        console.log("is jian jia")
                        gToast.requestToast("为文件!");
                        // open the file
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
            if(listCheckedCount > 0){
                toolbar.enabledIndexes([0,2], true)
            }else{
                toolbar.enabledIndexes([0,2], false)
            }

            if(listCheckedCount === 1){
                toolbar.enabledIndexes([1,3], true)
            }else{
                toolbar.enabledIndexes([1,3], false)
            }
        }

        CFlowToolBar {
            id: toolbar
            anchors.bottom: tab_btn.top
            width:parent.width
            animationVisible: view.editing
            backgroundEnabled: true

            names: [os.i18n.ctr(qsTr("分享")), os.i18n.ctr(qsTr("重命名")), os.i18n.ctr(qsTr("删除")), os.i18n.ctr(qsTr("下载"))]

            onClicked: {
                var indexList = view.selectedIndexes
                if(index == 0){
                    //                    var pathList = [];
                    //                    var i;
                    //                    for (i = 0; i < indexList.length; i++) {
                    //                        pathList.push(historyModel.getPath(indexList[i]))
                    //                    }

                    //                    //view.editing = false;
                    //                    shareDialog.open(pathList, CMIMEDialogTool.Share, "")

                }else if(index == 1){
                    renameDialog.show()
                    //                    currentpath = view.model.getPath(indexList[0])
                    //                    modellasttime = view.model.getLastTime(indexList[0])
                    //                    changeNameDialog.titleText = qsTr("rename save")+rootWindow.transflag
                    //                    changeNameDialog.origintext = view.model.getName(indexList[0])
                    //                    changeNameDialog.show()
                }else if(index == 2){
                    console.log("=====my count ============",view.selectedIndexes.length)
                    rmIndexList = view.selectedIndexes
                    if(rmIndexList.length == 0){
                        return
                    }
                    confirmDeleteDialog.show()

                }else if(index == 3) {
                    console.log("======download file=============", view.selectedIndexes.length)
                    var d_path = mainListModel.getPath(indexList)
                    console.log("d_path = "+ d_path)
                    contrl.getDwnloadPath(d_path)
                    contrl.reqDownLoadFile()
                    indicator.visible = true
                }
            }
        }

        CDialog {
            id: confirmDeleteDialog
            visible: false
            titleText: qsTr("delete")
            titleAreaEnabled: true;
            messageText: !ismuliSelected || rmIndexList.length == 1?
                             qsTr("Sure to delete the selected file?")
                           :qsTr("Sure to delete the selected ") + rmIndexList.length + qsTr(" file?")
            onAccepted:  {
                console.log("RecordList.qml:onAccepted.")
                view.editing = false
                visible = false
                if(!ismuliSelected) {
                    mainListModel.remove(currentpath)
                    return;
                } else {
                    console.log("------------historyModel.getPath---------------------",mainListModel.getPath(rmIndexList[0]))
                    for(var i = 0; i < rmIndexList.length; i++) {
                        var index = rmIndexList[i];
                        console.log("rmIndex index = "+ index +" path = " + mainListModel.getPath(rmIndexList[i]))
                        mainListModel.remove(index, index)
                    }
                    //                    indicatorDialog.messageText = os.i18n.ctr(qsTr("Deleting..."))
                    //                    indicatorDialog.show()
                }
            }
        }

        Connections{
            target: mainListModel
            //            onCountChanged:{
            //                if(mainListModel.count === 0 && listPage.status === CPageStatus.Show){
            //                    if(!crushDialog.crushing){
            //                        pageStack.pop()
            //                    }
            //                }
            //            }


            onDeleteFinished:{
                console.log("delete finished");
                indicatorDialog.hide()
            }
        }

        Connections{
            target: contrl

            onEmptyFile: {
                console.log("文件夹为空!");
                gToast.requestToast("文件夹为空!");
            }

            onDownloadFinished :{
                console.log("download finished!")
                indicator.visible = false
                gToast.requestToast("下载完成！");
                view.editing = false
            }

        }

        CIndicatorDialog{
            id:indicatorDialog
        }


        Row {
            id: tab_btn
            spacing: 15
            height: 110
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            CButton {
                id: userInfo_btn
                text: "个人"
                width: 180
                height: 100
                onClicked: {
                    pageStack.push("qrc:///qml/UserInfo.qml")
                }
            }
            CButton {
                id: upload_btn
                text: "上传"
                width: 180
                height: 100
                onClicked: {
                    console.log("upload")
                    pageStack.push("qrc:///qml/Upload.qml")
                }
            }
            CButton {
                id: create_btn
                text: "新建"
                width: 180
                height: 100
                onClicked: {
                    console.log("create")
                    fileName_Dlg.show();
                }
            }
        }
        CInputDialog {
            id: fileName_Dlg
            placeholderText: "please input folderName..."
            onAccepted: {
                var c_name = fileName_Dlg.text()
                console.log("c_name = " + c_name);
                //                var c_date = currentDate.toLocaleString();
                //                console.log("c_date = " + c_date);
                contrl.createFolder(c_name);
                fileName_Dlg.setText("")
            }
        }

        CInputDialog {
            id: renameDialog
            titleText: "rename this folder?"
            onAccepted: {
                var value = renameDialog.text()
                mainListModel.rename(value)
                console.log("rename value === "+value)
                renameDialog.setText("")
            }
        }

        CIndicator{
            id: indicator
            anchors.fill: parent
            visible: false
        }
    }
}
