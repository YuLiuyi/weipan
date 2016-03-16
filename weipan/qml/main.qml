import QtQuick 2.0
import com.syberos.basewidgets 2.0
import QtQuick.Controls 1.2
import QtWebKit 3.0
import QtWebKit.experimental 1.0


CPageStackWindow {
    initialPage:CPage{
        id: page
        width:parent.width
        height:parent.height

        property string c_url: ""

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
                                contrl.reqMetaData("");
                                scrollView.visible = false;
                                main.visible = true;
                                pageStack.push("qrc:///qml/MainPage.qml")

                            }
                        }
                    }
                }
            }


            Item {
                id: main
                anchors.fill: parent

                Rectangle {
                    id: rect
                    width: parent.width
                    height: 60
                    color: "#7a6767"
                    Text {
                        color: "#5a3939"
                        anchors {
                            verticalCenter: parent.verticalCenter
                            horizontalCenter: parent.horizontalCenter
                        }
                        font.pixelSize: 40
                        font.bold: true
                        text: "微盘"
                    }
                }
                Text {
                    id: main_txt
                    text: "Hello WeiPan"
                    font.pixelSize: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                CButton {
                    id: login_btn
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 100
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 300
                    height: 150
                    text: "enter"
                    onClicked: {
                        if(!contrl.authoriseOk()) {
                            gToast.requestToast("还未登录!");
                            contrl.sendRequest();
                            console.log("url ==="+contrl.loadWeb());
                            page.c_url = contrl.loadWeb();
                            main.visible = false;
                            scrollView.visible = true;
                            console.log("111111!!")
                        } else {
                            contrl.getUserInfo();
                            contrl.reqMetaData("");
                            pageStack.push("qrc:///qml/MainPage.qml")
                        }
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

