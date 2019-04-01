import QtQuick 2.4
import QtMultimedia 5.5
import QtQuick.Controls 2.0
import QtQuick.Window 2.2


Item {
    id: camera
    width: Window.width
    height: Window.height
    objectName: "camera"

    Image {
        id: photoPreview
        anchors.bottomMargin: 66
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        z: 1
        //        width: parent.width
        //        height: parent.height-100
        //rotation: 90;
        objectName: "photoPreview"
    }

    VideoOutput {
        anchors.bottomMargin: 66
        anchors.fill: parent
        source: camera1
        focus : visible
        autoOrientation: true;
        // to receive focus and capture key events when visible
    }

    Camera {
        id: camera1
        objectName: "camera1"
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
        deviceId: QtMultimedia.availableCameras[0].deviceId
        exposure {
            exposureCompensation: 1
            exposureMode: Camera.ExposureAuto
        }
        focus.focusMode: CameraFocus.FocusAuto
        flash.mode: Camera.FlashAuto
        imageCapture {
            id: imageCapture
            onImageCaptured: {
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }
    signal accept(string img)
    signal cancel()
    Row{
        height: 75
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30

    Button {
        id: acceptButton
        x: 25
        y: 0
        height: 75
        width:  parent.width/5
        text: qsTr("Accept")
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        enabled: false;
        onClicked: {
            acceptButton.enabled=false
            focusButton.enabled=true
            captureButton.text = "capture"
            photoPreview.source=""
            camera.accept(camera1.imageCapture.capturedImagePath)
        }
        }

    Button {
        id: cancelButton
        x: 186
        y: 1350
        height: 75
        width:  parent.width/5
        text: "Cancel"
        anchors.left: acceptButton.right
        anchors.leftMargin: 25
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onClicked: {
            acceptButton.enabled=false
            focusButton.enabled=true
            captureButton.text = "capture"
            photoPreview.source=""
            camera.cancel()
        }
    }

    Button {
        id: captureButton
        x: 292
        y: 0
        height: 75
        width:  parent.width/5
        text: "Capture"
        anchors.left: focusButton.right
        anchors.leftMargin: 25
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onClicked: {
            if (captureButton.text=="Capture"){
                camera1.imageCapture.captureToLocation("img.jpg")
                captureButton.text="Clear"
                acceptButton.enabled = true
                focusButton.enabled = false
            }
            else{
                captureButton.text="Capture"
                photoPreview.source=""
                acceptButton.enabled = false
                focusButton.enabled = true
            }
        }
    }
    Button {
        id: focusButton
        y: 1366
        height: 75
        width:  parent.width/5
        text: qsTr("Focus")
        anchors.left: cancelButton.right
        anchors.leftMargin: 25
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        onClicked: {
            camera1.searchAndLock()
        }
    }
    }
}
