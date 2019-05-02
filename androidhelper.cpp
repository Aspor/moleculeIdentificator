#include "androidhelper.h"
#include<QtAndroidExtras/QAndroidJniObject>


AndroidHelper::AndroidHelper(): QObject()
{

}

void AndroidHelper::setScreenOrientation(int orientation)
{
    QAndroidJniObject activity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if ( activity.isValid() )
    {
        activity.callMethod<void>
                ("setRequestedOrientation" // method name
                 , "(I)V" // signature
                 , orientation);
    }
}
