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

        property int c_index: 0
        property string baseDir: ""

        property date currentDate: new Date()
        property string c_name: ""
        property string c_date: ""
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
            if (status == CPageStatus.Show) {
                console.log("[mainpg_pg] ")
                mainListModel.clear()
                contrl.reqMetaData("");
            }
        }

        Component.onCompleted: {

//            indicator.visible = true;
            contrl.readCfg();
            contrl.getUserInfo();
            contrl.reqMetaData("");
        }

        Connections{
            target: mainListModel

            onDeleteFinished: {
                console.log("delete finished");
                indicatorDialog.hide()
            }
        }

        Connections{
            target: contrl

            onShowError :{
                gToast.requestToast("error!请重新登录!")
            }

            onShowWeb :{
                console.log("url ==="+contrl.loadWeb());
                page.c_url = contrl.loadWeb();
                contrl.loadWeb()
                main.visible = false;
                scrollView.visible = true;
            }

            onDownloadFinished :{
                console.log("download finished!")
                gToast.requestToast("已下载到手机！")
                view.editing = false
            }


            onProcFolderFinished :{
                //                        contrl.reqMetaData(path);
            }

            onEmptyFile: {
                console.log("文件夹为空!");
                gToast.requestToast("文件夹为空!可新建文件夹，上传文档");
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
                visible: true
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

                //                文件或者目录信息

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
                        Qt.quit()
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

                    delegate: /*Rectangle*/CEditListViewDelegate {
                        id: _delegate
                        width: view.width
                        height: 120

                        //                        MouseArea {
                        //                            anchors.fill: parent

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
                                console.log("is wen jian")

                                // if the file is not downloaded, need to download, then open the file.
                                var txt1_path = mainListModel.getPath(index)
                                var txt_title1 = mainListModel.getTitle(index)
                                var txt_content = contrl.readFile("/home/user/"+txt_title1)
                                gToast.requestToast("目前还不能打开文件")
//                                contrl.getDwnloadPath(txt1_path)
//                                contrl.reqDownloadFile()
                                // open the file
//                                console.log("title===" + txt_title1 + "  content===" + txt_content)
//                                pageStack.push("qrc:///qml/ShowFile.qml",{title: txt_title1,text: txt_content})

                            }
                        }
                        //                            onPressAndHold: {
                        //                                console.log("press and hold")
                        //                                toolbar.animationVisible = true
                        //                            }
                        //                        }

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
                    //                                    if(listCheckedCount > 0){
                    //                                        toolbar.enabledIndexes([0,1,2], true)
                    //                                    }else{
                    //                                        toolbar.enabledIndexes([0,1,2], false)
                    //                                    }

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
                            //                    var pathList = [];
                            //                    var i;
                            //                    for (i = 0; i < indexList.length; i++) {
                            //                        pathList.push(historyModel.getPath(indexList[i]))
                            //                    }

                            //                    //view.editing = false;
                            //                    shareDialog.open(pathList, CMIMEDialogTool.Share, "")

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
                            }
                        }
                    }
                }

                CDialog {
                    id: confirmDeleteDialog
                    visible: false
                    titleText: qsTr("delete")
                    titleAreaEnabled: true;
                    messageText:  "Sure to delete the selected file?"

                    onAccepted:  {
                        console.log("RonAccepted.")
                        console.log("------------mainListModel.getPath---------------------",mainListModel.getPath(page.c_index))
                        var path = mainListModel.getPath(page.c_index);
                        view.editing = false
                        visible = false
                        mainListModel.remove(page.c_index,page.c_index)
                        contrl.reqDeleteFile(path)
                    }

                    onRejected: {
                        view.editing = false
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
                        text: "新建"
                        width: 240
                        height: 80
                        onClicked: {
                            console.log("create")
                            fileName_Dlg.show();
                        }
                    }
                    Button {
                        id: upload_btn
                        text: "上传"
                        width: 240
                        height: 80
                        onClicked: {
                            console.log("upload")
                            pageStack.push(filesPickerCom)                       }

                    }
                    Button {
                        id: loadList_btn
                        width: 240
                        height: 80
                        text: "传输列表"
                        onClicked: {
                            pageStack.push("qrc:///qml/LoadList.qml")
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
                            window.clearFocus();

                        }

                        onCancel:{

                        }
                    }
                }

                CIndicator{
                    id: indicator
                    anchors.fill: parent
                    visible: contrl.isRequesting
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
                            gToast.requestToast("此名称已存在，请重命名！")
                            return;
                            show();
                        }

                        var f_path = page.c_path;

                        contrl.createFolder(value, f_path);

                        fileName_Dlg.setText("");
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

