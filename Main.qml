import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import QtQuick.Layouts
import com.leozqi.photos

ApplicationWindow {
    id: appWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Photos")

    FileDialog {
        id: openAlbumDialog
        fileMode: FileDialog.OpenFile
        nameFilters: ["Albums (*.album)"]
        onAccepted: function() {
            PhotoInterface.openAlbum(selectedFile)
            stack.pop()
            stack.push(albumView)
        }
    }

    FileDialog {
        id: addImageDialog
        fileMode: FileDialog.OpenFiles
        nameFilters: ["Image Files (*.png *.jpg *.jpeg *.bmp *.tiff *.gif *.pbm *.pgm *.ppm *.xbm *.xpm *.svg)"]
    }

    FileDialog {
        id: saveAlbumDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["Albums (*.album)"]
        onAccepted: function() {
            PhotoInterface.openAlbum(selectedFile)
            stack.pop()
            stack.push(albumView)
        }
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

    Component {
        id: welcomeView
        ColumnLayout {
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.horizontalCenter: parent.horizontalCenter
            width: 400
            height: 200
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
    }

    Component {
        id: albumView
        Item {
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
            anchors.topMargin: 80
            anchors.leftMargin: 80
            anchors.rightMargin: 80

            GridView {
                id: albumGrid
                model: DataModel {}
                cellWidth: 250
                cellHeight: 250

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }

                delegate: Column {
                    x: 50
                    y: 50
                    width: 150
                    height: 150
                    Image {
                        source: "image://imageprovider/" + model.display
                    }
                }

                Connections {
                    target: addImageDialog
                    onAccepted: {
                        albumGrid.model.addPhotos(addImageDialog.selectedFiles)
                    }
                }
            }
        }
    }
}




