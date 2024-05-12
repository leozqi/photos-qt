import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import com.leozqi.photos

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    FileDialog {
        id: addImageDialog
        onAccepted: image.source = selectedFile
    }

    FileDialog {
        id: openAlbumDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Albums (*.album)"]
        onAccepted: PhotoInterface.openAlbum(selectedFile)
    }

    FileDialog {
        id: saveAlbumDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Albums (*.album)"]
        onAccepted: PhotoInterface.openAlbum(selectedFile)
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                id: actionNewAlbum
                text: qsTr("&New album...")
                shortcut: StandardKey.New
                onTriggered: saveAlbumDialog.open()
            }
            Action {
                id: actionOpenAlbum
                text: qsTr("&Open album...")
                shortcut: StandardKey.Open
                onTriggered: openAlbumDialog.open()
            }
            Action { text: qsTr("&Save album") }
            Action { text: qsTr("Save album as...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action {
                id: actionAddImg
                text: qsTr("&Add Image")
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




