import QtQuick 2.12
import QtQuick.Controls 2.14

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Send multipart SMS on Android device (Qt5 + jni)")

    Rectangle {
        id: titleRect
        width: titleTxt.paintedWidth + 20
        height: titleTxt.paintedHeight + 20
        color: "lightgray"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: titleTxt
            text: "Send multipart SMS"
            color: "darkblue"
            font.pointSize: 24
            font.bold: true
            anchors.centerIn: parent
        }
    }

    GroupBox {
        id: txtBox
        title: qsTr("Message text")

        anchors.left: parent.left
        anchors.top: titleRect.bottom
        anchors.topMargin: 20
        width: parent.width
        contentWidth: width
        height: 180
        contentHeight: logScroll.implicitHeight
        padding: 10

        // https://stackoverflow.com/questions/46579359/styling-groupbox-in-qml-2
        background: Rectangle {
            y: txtBox.topPadding - txtBox.padding
            width: parent.width
            height: parent.height - txtBox.topPadding + txtBox.padding
            color: "transparent"
            border.color: "#21be2b"
            border.width: 3
            radius: 10
        }

        label: Rectangle {
            anchors.left: parent.left
            anchors.leftMargin: 25
            anchors.bottom: parent.top
            anchors.bottomMargin: -height/2

            color: "white"
            width: logTitle.paintedWidth + 40
            height: logTitle.font.pixelSize
            Text {
                id: logTitle
                text: txtBox.title
                anchors.centerIn: parent
                font.pixelSize: 16
            }
        }

        ScrollView {
            id: logScroll
            width: txtBox.width - 20
            height: txtBox.height - 20
            anchors.top: parent.top

            TextArea {
                id: msgText
                text: qsTr("Enter your text here...")

                textFormat: TextEdit.PlainText
                wrapMode: Text.WordWrap
                selectByMouse: true
                //                readOnly: true

                Connections {
                    target: cppSmsSender
                    onLog:   log(txt);
                    onError: error(txt);
                }
            }
        }
    }

    MyButton {
        id: clearMsg
        anchors.right: parent.right
        anchors.top: txtBox.bottom
        anchors.margins: 10

        buttonText: qsTr("Clear msg")

        onClicked: {
//            console.log("Clear Log!");
            msgText.clear();
        }
    }

    Column {
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.top: txtBox.bottom
        anchors.topMargin: 20
        spacing: 10

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 5

            Text {
                text: "Mobile:"
            }
            TextInput {
                id: mobileNumber
                selectByMouse: true
                text: "+33781331776"
                validator: RegExpValidator { regExp: /^(\+)?\d{8,15}$/ }
            }
        }

        MyButton {
            id: sendText
            anchors.horizontalCenter: parent.horizontalCenter
            buttonText: qsTr("Send Text")
            onClicked: cppSmsSender.sendText(mobileNumber.text, msgText.text);
        }
    }

    function log(txt){
        msgText.append(txt);
    }

    function error(txt){
        msgText.append("ERROR: "+txt);
    }

}
