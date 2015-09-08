import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {//Серый полупрозрачный фон
    objectName: "waitingWindow"
    color: "#77889980"
    Rectangle {
        id: sync
        visible: true
        width: 250
        height: 110
        color: "#ffffff"
        anchors.centerIn: parent
        radius: 7
        border.width: 2
        border.color: "gray"
        RowLayout {
            anchors.centerIn: parent
            transformOrigin: Item.Center
            AnimatedImage {
                Layout.preferredWidth: 70
                Layout.preferredHeight: 70
                source: "qrc:/images/sync.gif"
            }
            Item {
                Layout.preferredWidth: 140
                Layout.fillHeight: true
                Text {
                    anchors.centerIn: parent
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Синхронизация")
                    font.pixelSize: 15
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    Rectangle {
        id: error
        visible: false
        width: 250
        height: 110
        color: "#ffffff"
        anchors.centerIn: parent
        radius: 7
        border.width: 2
        border.color: "gray"
        RowLayout {
            anchors.centerIn: parent
            transformOrigin: Item.Center
            Image {
                Layout.preferredWidth: 70
                Layout.preferredHeight: 70
                source: "qrc:/images/error.png"
            }
            Item {
                Layout.preferredWidth: 140
                Layout.fillHeight: true
                Text {
                    anchors.centerIn: parent
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Ошибка")
                    font.pixelSize: 15
                    wrapMode: Text.WordWrap
                }
            }
        }
    }

    function showSynch(msg) {
        sync.visible = true;
        error.visible = false;
        console.log("Got message:", msg)
        return "some return value"
    }

    function showError(msg) {
        sync.visible = false;
        error.visible = true;
        console.log("Got message:", msg)
        return "some return value"
    }
}
