import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12
import IC705.RemoteControl 1.0
import IC705.RotaryEncoder 1.0


ApplicationWindow {
    id: window
    visible: true
    width: 480
    minimumWidth: width
    maximumWidth: width
    height: 620
    minimumHeight: height
    maximumHeight: height
    color: "#333333"
    property alias window: window
    title: "CI-V interface for ICOM IC-705"

    function frequencyToString(frequency)
    {
        return Number(frequency).toLocaleString(Qt.locale("de_DE"), 'f', 0);
    }

    function hzToString(frequency)
    {
        let Hz = Number(frequency);
        let MHz = Math.floor(Hz / 1000000);
        let kHz = Math.floor((Hz - (MHz * 1000000)) / 1000);
        let dHz = Math.floor((Hz - (MHz * 1000000) - (kHz * 1000)) / 10);

        return MHz + "." + kHz + "." + dHz;
    }

    function squelchStatusToColor(squelchStatus)
    {
        return Number(squelchStatus) ? "yellowgreen" : "dimgrey";
    }

    function overflowStatusToColor(overflowStatus)
    {
        return Number(overflowStatus) ? "red" : "dimgrey"
    }

    RemoteControl {
        id: ic705
    }

    RotaryEncoder {
        id: vfoWheel
        // Link RotaryEncoder with RemoteControle C++ classes
        onIncrement: ic705.onVfoWheelIncrement();
        onDecrement: ic705.onVfoWheelDecrement();
        onStepSizeChanged: ic705.onVfoStepSizeChanged(value);
    }

    ColumnLayout
    {
        id: mainFrame
        anchors.fill: parent
        anchors.margins: 10
        anchors.rightMargin: 10

        RowLayout {
            ColumnLayout {
                Layout.fillWidth: true
            }
            Text {
                Layout.rightMargin: 5
                color: "whitesmoke"
                text: qsTr("Update interval [ms]:");
            }
            ComboBox {
                height: 40
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                currentIndex: ic705.frequencyPollIndex
                model: ic705.pollIntervalList
                onCurrentIndexChanged: ic705.frequencyPollIndex = currentIndex;
            }
        } /* RowLayout */

        GroupBox {
            Layout.fillWidth: true
            label: Label {
                color: "whitesmoke"
                text: qsTr("Receiver:")
            }

            RowLayout {
                anchors.fill: parent
                Label {
                    color: "whitesmoke"
                    text: qsTr("S-Meter")
                }
                ProgressBar {
                    id: sMeterBar
                    value: ic705.sMeter
                    background: Rectangle {
                        implicitHeight: 10
                        implicitWidth: 200
                        color: "dimgrey"
                        radius: 1
                    }
                    contentItem: Item {
                        implicitWidth: 198
                        Rectangle {
                            width: sMeterBar.visualPosition * parent.width
                            height: parent.height
                            color: "dodgerblue"
                            radius: 1
                        }
                    }
                }
                Text {
                    color: "whitesmoke"
                    text: ic705.sMeterText
                }

                Column {
                    Layout.fillWidth: true
                }

                Label {
                    color: "whitesmoke"
                    text: qsTr("SQL")
                }
                Rectangle {
                    id: squelchStatus
                    width: 20
                    height: 10
                    color: squelchStatusToColor(ic705.squelchStatus);
                    radius: 1
                    Layout.fillHeight: false
                }

                Label {
                    color: "whitesmoke"
                    text: qsTr("OVF")
                }
                Rectangle {
                    id: overflowStatus
                    width: 20
                    height: 10
                    color: overflowStatusToColor(ic705.overflowStatus);
                    radius: 1
                    Layout.fillHeight: false
                }
            } /* RowLayout */
        } /* GroupBox */

        GroupBox {
            id: vfoGroup
            Layout.fillWidth: true
            label: Label {
                color: "whitesmoke"
                text: qsTr("VFO")
            }

            ColumnLayout {
                anchors.fill: parent
                GridLayout {
                    columns: 2

                    Label {
                        color: "whitesmoke"
                        text: qsTr("TX:")
                    }
                    Text {
                        color: "whitesmoke"
                        text: frequencyToString(ic705.txFrequency);
                        font.pixelSize: 24
                    }
                    Label {
                        color: "whitesmoke"
                        text: qsTr("RX:")
                    }
                    Text {
                        color: "whitesmoke"
                        text: frequencyToString(ic705.rxFrequency);
                        font.pixelSize: 48
                    }
                } /* GridLayout */

                RowLayout {
                    CircularSlider {
                        id: vfoWheelControl

                        width: 100
                        height: width
                        Layout.maximumWidth: width
                        Layout.maximumHeight: width
                        Layout.fillHeight: false
                        Layout.fillWidth: false

                        trackWidth: width / 2 - 2
                        handleWidth: width / 4
                        handleHeight: width / 4
                        handleVerticalOffset: -5

                        startAngle: 0
                        endAngle: 360
                        minValue: 0
                        maxValue: vfoWheel.max
                        snap: false
                        stepSize: 1

                        // No direct value binding because CircularSlider does something strange then
                        value: 0
                        onValueChanged: vfoWheel.value = value;

                        trackColor: "#505050"
                        hideTrack: false
                        hideProgress: true

                        handle: Rectangle {
                            transform: Translate {
                                x: (vfoWheelControl.handleWidth - width) / 2
                                y: (vfoWheelControl.handleHeight - height) / 2
                            }
                            width: vfoWheelControl.handleWidth
                            height: vfoWheelControl.handleHeight
                            radius: width / 2
                            antialiasing: true
                            color: "#e0e0e0"

                            border.color: "#202020"
                            border.width: 1
                        }
                    }
                    Label {
                        Layout.rightMargin: 5
                        color: "whitesmoke"
                        text: qsTr("Step:");
                    }
                    ComboBox
                    {
                        model: vfoWheel.stepSizeList
                        currentIndex: vfoWheel.stepSizeIndex
                        onCurrentIndexChanged: vfoWheel.stepSizeIndex = currentIndex;
                    }
                } /* RowLayout */

            } /* ColumnLayout */
        } /* GroupBox */

        GroupBox {
            Layout.fillWidth: true
            label: Label {
                color: "whitesmoke"
                text: qsTr("GPS")
            }

            GridLayout {
                anchors.fill: parent
                columns: 2
                columnSpacing: 20

                Label {
                    color: "whitesmoke"
                    text: qsTr("Enable:")
                }
                CheckBox {
                    id: gpsEnabled
                    checked: ic705.gpsEnable
                    onCheckedChanged: ic705.gpsEnable = checked;
                }
                Label {
                    color: "whitesmoke"
                    text: qsTr("Position:")
                }
                Text {
                    color: gpsEnabled.checked ? "whitesmoke" : "gray"
                    text: ic705.gpsDataValid ? ic705.positionText : "-"
                    font.pixelSize: 20
                    Layout.fillWidth: true
                }
                Label {
                    color: "whitesmoke"
                    text: qsTr("QTH Locator:")
                }
                Text {
                    color: gpsEnabled.checked ? "whitesmoke" : "gray"
                    text: ic705.gpsDataValid ? ic705.locatorText : ""
                    font.pixelSize: 20
                    Layout.fillWidth: true
                }
                Label {
                    color: "whitesmoke"
                    text: qsTr("Height:")
                }
                Text {
                    color: gpsEnabled.checked ? "whitesmoke" : "gray"
                    text: ic705.gpsDataValid ? (ic705.altitudeValid ? ic705.altitude.toFixed(1) + " m" : "-") : ""
                    font.pixelSize: 20
                    Layout.fillWidth: true
                }
                Label {
                    color: "whitesmoke"
                    text: qsTr("Speed / Heading:")
                }
                Text {
                    color: gpsEnabled.checked ? "whitesmoke" : "gray"
                    text: ic705.gpsDataValid ? ((ic705.speedValid ? ic705.speed.toFixed(1) : "-") + " km/h  " + ic705.heading + "°") : ""
                    font.pixelSize: 20
                    Layout.fillWidth: true
                }
                Label {
                    color: "whitesmoke"
                    text: qsTr("Timestamp (UTC):")
                }
                Text {
                    color: gpsEnabled.checked ? "whitesmoke" : "gray"
                    text: ic705.gpsDataValid ? Qt.formatDateTime(ic705.timestamp, "dd.MM.yyyy  hh:mm:ss") : ""
                    font.pixelSize: 20
                    Layout.fillWidth: true
                }
            } /* GridLayout */
        } /* GroupBox */

        Text {
            color: "whitesmoke"
            text: ic705.statusText;
        }

    } /* ColumnLayout */
}







/*##^##
Designer {
    D{i:0;formeditorZoom:0.66}
}
##^##*/
