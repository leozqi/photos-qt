import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import QtQuick.Layouts
import com.leozqi.photos

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Photos")

    FileDialog {
        id: addImageDialog
        fileMode: FileDialog.OpenFiles
        nameFilters: ["Image Files (*.png *.jpg *.jpeg *.bmp *.tiff *.gif *.pbm *.pgm *.ppm *.xbm *.xpm *.svg)"]
        onAccepted: PhotoInterface.addPhotos(selectedFiles)
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

    StackView {
         id: stack
         initialItem: welcomeView
         anchors.fill: parent
    }

    ColumnLayout {
        id: welcomeView
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        width: 400
        height: 300
        focus: true

        Image {
            Layout.alignment: Qt.AlignHCenter
            source: "images/photo-album.png"
        }

        Button {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Create a new album ...")
            onClicked: saveAlbumDialog.open()
        }

        Button {
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Open an album ...")
            onClicked: openAlbumDialog.open()
        }
    }

    // Control {
    //     padding: 50
    //     anchors {
    //         left: parent.left
    //         right: parent.right
    //         top: parent.top
    //         bottom: parent.bottom
    //     }

    //     GridView {
    //         cellWidth: 110
    //         cellHeight: 110

    //         anchors {
    //             left: parent.left
    //             right: parent.right
    //             top: parent.top
    //             bottom: parent.bottom
    //         }

    //         model: DataModel {}
    //         delegate: Column {
    //             x: 10
    //             y: 10
    //             width: 100
    //             height: 100
    //             Image { source: portrait; anchors.horizontalCenter: parent.horizontalCenter }
    //             Text { text: name; anchors.horizontalCenter: parent.horizontalCenter }
    //         }
    //     }

    // }

    // Component.onCompleted: openAlbumDialog.open()
}




