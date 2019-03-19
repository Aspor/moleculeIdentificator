#ifndef API_H
#define API_H

#include"molecule.h"
#include<QtNetwork>
#include<QXmlStreamReader>

//Class for comminicating with chemSpider API

class API:public QObject
{
    Q_OBJECT
public:
    API(QString baseURL,QString securityToken);

    Molecule* search(QString SMILE);

private:
    QString baseUrl;
    QNetworkReply* post(QString url, QString key, QString data);
    QNetworkReply* put(QString url, QString data);
    QNetworkReply* get(QString url);
    QNetworkAccessManager manager;
    QString securityToken;
};

#endif // API_H
