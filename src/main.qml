import QtQuick 2.9
import org.asteroid.controls 1.0
import org.asteroid.utils 1.0
import org.asteroid.recorder 1.0
import Qt.labs.folderlistmodel 2.1
import QtMultimedia 5.4
import Nemo.KeepAlive 1.1

Application {
    id: app
    centerColor: "#dfb103"
    outerColor: "#be4e0e"

    function prefixZero(x) {
        return ("0" + x).substr(-2,2);
    }

    AudioRecorder {
        id: recorder
        onRecordingChanged: console.log(recording)
        /*onAudioChanged: {
            var m = Math.abs(audio)
            while (audioSamples) {
                var t = Math.abs(audio)
                if (t > m) m = t
            }
            jk.x = app.width/2 + m*(app.width-10)
        }*/
    }

    Audio {
        id: recordFile
    }
    
    ListView {
        id: lv
        anchors.fill:parent
        model: 
        ListModel {
            id: entries
            ListElement {
                isLast: false
            }
            ListElement {
                isLast: true
            }
        }
        delegate: Item {
            width: app.width;
            height: app.height
            Item {
                anchors.fill: parent
                visible: !isLast

                Rectangle {
                    id: jk
                    x: app.width/2 + recorder.audio*(app.width-10)
                    y: parent.height/2
                    width: 10
                    height: 10
                    color: "yellow"
                    Behavior on x { NumberAnimation { id:aq; duration: 100} }
                }
                IconButton {
                    id: recordButton
                    edge: undefinedEdge
                    anchors.centerIn: parent
                    width: Dims.w(40)
                    height: width
                    iconName: recorder.recording ? "ios-stop-outline" : "ios-record"
                    onClicked: {
                        if (recorder.recording) {
                            recorder.stop()
                        } else {
                            var fileName = "record_" + new Date().getFullYear() + prefixZero(new Date().getMonth()) + prefixZero(new Date().getDate())
                                            + "_" + prefixZero(new Date().getHours()) + prefixZero(new Date().getMinutes()) + prefixZero(new Date().getSeconds()) + ".wav";
                            recorder.record(fileName)
                        }
                    }
                }
            }
            Item {
                anchors.fill: parent
                visible: isLast
                Component {
                    id: lvHeader
                    Item {
                        anchors.right: parent.right
                        anchors.left: parent.left
                        width: parent.width
                        height: (DeviceInfo.hasRoundScreen ? app.height/12 : 0)
                    }
                }
                Component {
                    id: lvFooter
                    Item {
                        anchors.right: parent.right
                        anchors.left: parent.left
                        width: parent.width
                        height: (DeviceInfo.hasRoundScreen ? app.height/12 : 0) + (app.height/6)
                    }
                }
                ListView {
                    id: grid
                    anchors.fill: parent
                    header: lvHeader
                    footer: lvFooter
                    model: FolderListModel {
                        id: folderModel
                        folder: "file:///home/ceres/recordings/"
                        nameFilters: ["*.wav"]
                    }

                    Component {
                        id: fileDelegate
                        Item {
                            height: app.height/6
                            width: app.width
                            Marquee {
                                width: DeviceInfo.hasRoundScreen ? Dims.w(60) : Dims.w(80)
                                height: Dims.h(10)
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: fileName
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    recordFile.source = folderModel.folder + "/" + fileName
                                    recordFile.play()
                                }
                            }
                        }
                    }
                    delegate: fileDelegate
                }
            }
        }
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
    }
    PageDot {
        height: Dims.h(3)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Dims.h(3)
        currentIndex: lv.currentIndex
        dotNumber: 2
    }
    Component.onCompleted: DisplayBlanking.preventBlanking = recorder.recording
}