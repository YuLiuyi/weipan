import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import com.syberos.basewidgets 2.0


CInputDialog {
    id: inputDialog

    property string folderName: ""
    property string mode: ""

    property var paths: []
    //property int mode: 0 // 0，只有‘确定’按钮;1，默认‘确定’及取消‘按钮’；
    titleText: qsTr("new folder") + trflag
    placeholderText: qsTr("folder name") + trflag
    visible: false

    signal confirmButtonEnable(bool enable);

    function open(text) {
        if(text === "")
            inputDialog.acceptButtonEnabled = false
        inputDialog.setText(text);
        inputDialog.show();
    }

    function pushQml(file) {
        pageStack.push(Qt.resolvedUrl(file));
    }

    function stringLength(str) {
        var i,sum=0;
        for(i=0;i<str.length;i++){
            if((str.charCodeAt(i)>=0) && (str.charCodeAt(i)<=255))
                sum=sum+1;
            else
                sum=sum+2;
        }
        console.log("string length:" + sum);
        return sum;
    }

    Row {
        spacing: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 160
        anchors.left: parent.left
        anchors.leftMargin: 70
        anchors.right: parent.right
        z: parent.z + 1

        Text{
            id:cautionText
            color:"red"
            font.pixelSize: 20
        }
     }

    Component.onCompleted: {
    }

    onAccepted: {
        var value = inputDialog.text();
        console.log("accept is result is:" + value);
        console.log("mode="+mode);

        if(value === "") {
            return;
        }

        if (folderName === value) {
            //inputDialog.close();
            return;
        }

        inputDialog.setText("");
    }

    Connections{
        target: inputDialog
    }
    Connections {
        target: inputDialog
        onTextChanged: {
            var value = text;
            console.log("*******************" + value)
            if (value.length === 0) {
                inputDialog.acceptButtonEnabled = false
                cautionText.text = ""
            } else if(stringLength(value) > 200) {
                inputDialog.acceptButtonEnabled = false
                cautionText.text = qsTr("The name is too long.")
            } else if(value.indexOf("\"") !== -1
                     ||value.indexOf("\“")!== -1 || value.indexOf("\”") !== -1
                     ||value.indexOf(" ") !== -1 || value.indexOf("　") !== -1
                     ||value.indexOf("/") !== -1 || value.indexOf("／") !== -1
                     ||value.indexOf("\\")!== -1 || value.indexOf("＼") !== -1
                     ||value.indexOf(":") !== -1 || value.indexOf("：") !== -1
                     ||value.indexOf("*") !== -1 || value.indexOf("＊") !== -1
                     ||value.indexOf("?") !== -1 || value.indexOf("？") !== -1
                     ||value.indexOf("<") !== -1 || value.indexOf("＜") !== -1
                     ||value.indexOf(">") !== -1 || value.indexOf("＞") !== -1
                     ||value.indexOf("|") !== -1 || value.indexOf("｜") !== -1
                     ||value.indexOf("。") !== -1) {
                inputDialog.acceptButtonEnabled = false
                //cautionText.text = qsTr("The name cannot include \"\\/：*?<>|.")
                cautionText.text = qsTr("The name cannot include special character")
            } else {
                inputDialog.acceptButtonEnabled = true
                cautionText.text = ""
            }
        }
    }
    onShowFinished: {
        messageAreaItem.focus = true;
    }
    onHideFinished: {
    }
}


