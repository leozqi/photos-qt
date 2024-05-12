import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Dialogs



ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    FileDialog {
        id: addImageDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: image.source = selectedFile
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action {
                id: actionAddImg
                text: qsTr("&Add Image")
                shortcut: StandardKey.New
                onTriggered: addImageDialog.open()
            }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }

    GridView {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        model: PhotoModel {}
        delegate: Column {
            Image { source: portrait; anchors.horizontalCenter: parent.horizontalCenter }
            Text { text: name; anchors.horizontalCenter: parent.horizontalCenter }
        }
    }
}




