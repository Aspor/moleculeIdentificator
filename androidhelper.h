#ifndef ANDROIDHELPER_H
#define ANDROIDHELPER_H

#include <QObject>

class AndroidHelper: public QObject
{
    Q_OBJECT
public:
    AndroidHelper();
public slots:
    void setScreenOrientation(int orientation=4);
};

#endif // ANDROIDHELPER_H
