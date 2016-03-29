import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.2
import Qt.labs.folderlistmodel 1.0
import QtWebKit 3.0
import QtWebKit.experimental 1.0
import com.syberos.filemanager.filepicker 1.0


CPageStackWindow {
    initialPage:CPage{
        id: page
        width:parent.width
        height:parent.height

        property string c_url: ""

        property string c_path: ""
        property string baseDir: ""
        property date currentDate: new Date()
        property string c_name: ""
        property string c_date: ""
        property var rmIndexList: []
        property bool ismuliSelected: true
        property bool flag: true

        property string f_path: ""
        property string webHtml: ""
        property string filename: ""
        property var errorAddress: []
        property int allAttachSize: 0
        property int maxAttachSize: 1024*1024*10       //附件大小最大支持10M
        property var recipientsItem


        onStatusChanged: {
            console.log("main status==="+status);

            if (status == CPageStatus.Show&&contrl.authoriseOk()) {
                console.log("[mainpg_pg] ")
                mainListModel.clear()
                contrl.reqMetaData("");
            }
        }


        Component.onCompleted: {
            if(!contrl.authoriseOk()) {
                gToast.requestToast("还未登录!");
                contrl.sendRequest();
                console.log("url ==="+contrl.loadWeb());
                page.c_url = contrl.loadWeb();
                //                main.visible = false;
                scrollView.visible = true;
                console.log("111111!!")
            } else {
                main.visible = true;
                contrl.getUserInfo();
                //                contrl.reqMetaData("");
            }
        }

        contentAreaItem: Item{

            Item {
                id: scrollView
                visible: false
                anchors.fill: parent

                ScrollView {
                    anchors.fill: parent
                    WebView {
                        id: webView
                        anchors.fill: parent
                        url: page.c_url
                        experimental.userAgent:"Mozilla/5.0 (Linux; Android 4.4.2; GT-I9505 Build/JDQ39) AppleWebKit/537.36 (KHTML, like Gecko) Version/1.5 Chrome/28.0.1500.94 Mobile Safari/537.36"
                        experimental.minimumScale: false;
                        experimental.preferredMinimumContentsWidth: 720
                        onLoadProgressChanged: {
                            console.log("url = " + webView.url + " title = "+webView.title)
                            console.log("progress=" + loadProgress)

                            var s = page.split1(url);
                            console.log("s = " + s);

                            var s1 = page.split2(url);
                            console.log("s1 = " + s1);

                            if(page.tokenExist(url) > 0) {
                                console.log("token has got");
                                contrl.getAccessToken(s, s1);
                                main.visible = true;
                                if(page.flag) {
                                    contrl.reqMetaData("");
                                    contrl.getUserInfo();
                                    page.flag = false;
                                }
                                scrollView.visible = false;
                                //                                pageStack.push("qrc:///qml/MainPage.qml")

                            }
                        }
                    }
                }
            }

            Item {
                id: main
                visible: false
                anchors.fill: parent
                Rectangle {
                    id: rect
                    width: parent.width
                    height: 60
                    color: "#7a6767"
                    Image {
                        id: usrInfo
                        anchors.left: parent.left
                        anchors.leftMargin: 40
                        anchors.verticalCenter: parent.verticalCenter
                        width: 50
                        height: 50
                        source: "qrc:/images/res/user_man_circle.png"
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                pageStack.push("qrc:///qml/UserInfo.qml")
                            }
                        }
                    }

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
                                //                        contrl.reqMetaData(mainListModel.getPath(index));
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
                                contrl.getDwnloadPath(txt1_path)
                                contrl.reqDownLoadFile()
                                indicator.visible = true
                                // open the file
                                console.log("title===" + txt_title1 + "  content===" + txt_content)
                                pageStack.push("qrc:///qml/ShowFile.qml",{title: txt_title1,text: txt_content})

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
                        toolbar.enabledIndexes([1], true)
                        if(mainListModel.getType(!view.selectedIndexes))
                        {
                            toolbar.enabledIndexes([3], true)
                        }
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
                    messageText: page.rmIndexList.length == 1?
                                     qsTr("Sure to delete the selected file?")
                                   :qsTr("Sure to delete the selected ") + rmIndexList.length + qsTr(" file?")
                    onAccepted:  {
                        console.log("RecordList.qml:onAccepted.")
                        view.editing = false
                        visible = false
                        if(!page.ismuliSelected) {
                            //                    mainListModel.remove(currentpath)
                            return;
                        } else {
                            console.log("------------historyModel.getPath---------------------",mainListModel.getPath(page.rmIndexList[0]))
                            for(var i = 0; i < page.rmIndexList.length; i++) {
                                var index = page.rmIndexList[i];
                                console.log("rmIndex index = "+ index +" path = " + mainListModel.getPath(page.rmIndexList[i]))
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
                        gToast.requestToast("文件夹为空!可新建文件夹，上传文档");
                    }

                    onDownloadFinished :{
                        console.log("download finished!")
                        indicator.visible = false
                        gToast.requestToast("已下载到手机根目录！");
                        view.editing = false
                    }

                    onUploadFinished :{
                        console.log("upload finished!")
                        indicator.visible = false;
                        gToast.requestToast("上传完成！");
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
                        id: upload_btn
                        text: "上传"
                        width: 180
                        height: 100
                        onClicked: {
                            console.log("upload")
                            //                            pageStack.push("qrc:///qml/Upload.qml")
                            pageStack.push(filesPickerCom)
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
                            page.filename = filePath;
                            console.log("filePicker filesPath = "+filePath)
                            contrl.getUploadFilePath(filePath);
                            contrl.reqUploadFile(page.f_path)
                            page.f_path = ""
                            indicator.visible = true;
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
                CInputDialog {
                    id: fileName_Dlg
                    placeholderText: "please input folderName..."
                    onAccepted: {
                        var f_name = fileName_Dlg.text()
                        var f_path = c_path;
                        console.log("c_name = " + f_name);
                        //                var c_date = currentDate.toLocaleString();
                        //                console.log("c_date = " + c_date);
                        contrl.createFolder(f_name, f_path);
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
            }
        }

        function split1(m) {
            var s = m.toString();
            var a1 = s.split("&");
            var t = a1[0];
            var s1 = t.split("=");
            return s1[1];

        }

        function split2(m) {
            var s = m.toString();
            var a1 = s.split("&");
            var t = a1[3];
            var s1 = t.split("=");
            return s1[1];

        }


        function tokenExist(m) {
            var s = m.toString();
            var index = s.indexOf("access_token");
            return index;
        }

    }
}

