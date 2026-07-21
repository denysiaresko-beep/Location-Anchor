import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import LocationAnchor


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("tracker")

    LocationTracker {
        id:tracker

        onMarkCreated: (lat, lon, timeString) => {
                           marksModel.append({
                                                "lat": lat,
                                                "lon": lon,
                                                "time": timeString
                                             })
                       }
    }


    ColumnLayout {
        anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            Rectangle {
                Layout.fillWidth: true
                height: 80
                color: "#ffffff"
                radius: 10
                border.color: "#e0e0e0"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Text {
                        text: "Status: "
                        font.pixelSize: 16
                        font.bold: true
                        color: "#333"
                    }

                    Text {
                        text: "Coord: "
                        font.pixelSize: 14
                        color: "#666"
                    }
                }
            }

            Text {
                text: "History:"
                font.pixelSize: 16
                font.bold: true
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "white"
                radius: 10
                border.color: "grey"
            }

            ListView {
                id: listView
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                model: ListModel {
                    id: marksModel
                }

                delegate: ItemDelegate {
                    width: listView.width
                    text: "[" + model.time + "] " + model.lat.toFixed(5) + ", " + model.lon.toFixed(5)
                    font.pixelSize: 14
                }

                onCountChanged: listView.positionViewAtEnd()
            }

        RowLayout {
            Layout.fillWidth: true

            Button {
                Layout.fillWidth: true
                text: "Simulation: Home"
                onClicked: tracker.simulateLocation(50.4501, 30.5252)
            }

            Button {
                Layout.fillWidth: true
                text: "Simulation: Work"
                onClicked: tracker.simulateLocation(55.8383, 26.2626)
            }
        }

        Button {
            Layout.fillWidth: true
            text: "Clear Log"
            onClicked: marksModel.clear()
        }

    }
}
