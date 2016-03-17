import QtQuick 2.0
import com.syberos.basewidgets 2.0

CPage {
    id: showTxt

    property string title: "title"
    property string text: "value"

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
                text: title
            }
        }

        Text {
            id: txt
            anchors.top: rect.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 40
            anchors.right: parent.right
            anchors.rightMargin: 40
            text: text
            font.pixelSize: 30
            color: "#291b1b"
        }
    }
}
