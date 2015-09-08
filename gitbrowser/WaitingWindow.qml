import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

Rectangle {//Серый полупрозрачный фон
    id: root
    objectName: "waitingWindow"
    color: "#77889980"
    signal retry()
    Rectangle {//Окно синхронизации
        id: sync
        visible: true
        width: 250
        height: 70
        color: "#ffffff"
        anchors.centerIn: parent
        radius: 7
        border.width: 2
        border.color: "gray"
        RowLayout {
            anchors.centerIn: parent
            width: parent.width - 20
            transformOrigin: Item.Center
            spacing: 10
            AnimatedImage {
                Layout.preferredWidth: 40
                Layout.preferredHeight: 40
                source: "qrc:/images/sync.gif"
            }
            Item {
                Layout.preferredWidth: 200
                Layout.fillHeight: true
                Text {
                    anchors.centerIn: parent
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Синхронизация")
                    font.pixelSize: 15
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    Rectangle {//Окно вывода ошибки
        id: error
        property string errorString: "Ошибка"
        visible: false
        width: 300
        height: 210
        color: "#ffffff"
        anchors.centerIn: parent
        radius: 7
        border.width: 2
        border.color: "gray"
        ColumnLayout{
            anchors.centerIn: parent
            width: parent.width - 17
            spacing: 10
            RowLayout {
                Layout.fillWidth: true
                transformOrigin: Item.Center
                spacing: 10
                Image {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                    source: "qrc:/images/error.png"
                }
                Item {
                    Layout.preferredWidth: 200
                    Layout.fillHeight: true
                    Text {
                        anchors.centerIn: parent
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Ошибка")
                        font.pointSize: 15
                        wrapMode: Text.WordWrap
                    }
                }
            }
            TextArea {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                font.pointSize: 10
                style: TextAreaStyle {
                    renderType: Text.QtRendering
                }
                text: error.errorString
                readOnly: true
            }
            RowLayout {
                Layout.alignment: Qt.AlignRight
                Button {
                    text: qsTr("Подробнее")
                }
                Button {
                    text: qsTr("Попробовать снова")
                    onClicked: root.retry()
                }
            }
        }
    }

    function showSync() {
        sync.visible = true;
        error.visible = false;
    }

    function showError(msg) {
        error.errorString = msg;
        sync.visible = false;
        error.visible = true;
    }
}
