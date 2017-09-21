import QtQuick 2.4
import QtMultimedia 5.5
import QtQuick.Controls 2.0

Item {
    id: camera
    width: 400
    height: 400
    objectName: "camera"

    Camera {
        id: camera1
        objectName: "camera1"
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash
        deviceId: QtMultimedia.availableCameras[0].deviceId
        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

        flash.mode: Camera.FlashOn

        imageCapture {
            onImageCaptured: {
                photoPreview.source = preview  // Show the preview in an Image
            }
        }
    }

    VideoOutput {
        anchors.rightMargin: 0
        anchors.bottomMargin: 62
        anchors.leftMargin: 0
        anchors.topMargin: 0
        source: camera1
        anchors.fill: parent
        focus : visible // to receive focus and capture key events when visible
    }

    Image {
        id: photoPreview
        width: 400
        height: 400

        objectName: "photoPreview"
    }
    signal accept(string img, size s, size s2 )
    signal cancel()


    Button {
        id: cancelButton
        x: 186
        y: 344
        text: "Cancel"
        onClicked: camera.cancel()
    }

    Button {
        id: captureButton
        x: 292
        y: 344
        text: qsTr("Capture")
        onClicked: camera1.imageCapture.capture()
    }
    Button {
        id: acceptButton
        x: 25
        y: 344
        text: qsTr("Accept")
        onClicked: camera.accept(photoPreview.source,photoPreview.sourceSize,photoPreview.sourceSize)
    }

//    Switch {
//        id: flashSwich
//        x: 25
//        y: 344
//        onCheckedChanged: camera1.FlashOff
//    }

}
