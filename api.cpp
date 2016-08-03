#include "api.h"
#include <QThread>

API::API(QString baseUrl, QString securityToken)
{
    this->baseUrl=baseUrl;
    this->securityToken=securityToken;
}
QNetworkReply* API::post(QString url, QString key, QString data){
    QUrlQuery postData;
    postData.addQueryItem(key, data);
    postData.addQueryItem("token",securityToken);
    QNetworkRequest request(QUrl(baseUrl+url));
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");
    return manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}
QNetworkReply* API::put(QString url, QString data){
    QByteArray array (data.toStdString().c_str());

    return manager.put(QNetworkRequest(QUrl(url)),array);
}
QNetworkReply* API::get(QString url){
    const QUrl newUrl = QUrl::fromUserInput(baseUrl+url);

    return manager.get(QNetworkRequest(newUrl));
}

Molecule* API::search(QString SMILE){
    QString apiURL="/Search.asmx/SimpleSearch";
    QNetworkReply* csidReply=post(apiURL,"query", SMILE);
    QEventLoop loop;
    connect(csidReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    if(csidReply->error()!=QNetworkReply::NoError)
        return new Molecule();
    QXmlStreamReader* reader = new QXmlStreamReader(csidReply);
    reader->readNext();
    reader->readNext();
    reader->readNext();
    reader->readNext();
    QString molInd=reader->readElementText();
    QNetworkReply* molInfoReply = post("/MassSpecAPI.asmx/GetExtendedCompoundInfo","CSID",molInd);
    connect(molInfoReply,SIGNAL(finished()),&loop,SLOT(quit()));
    loop.exec();
    if(molInfoReply->error()!=QNetworkReply::NoError)
        return new Molecule();
    reader->clear();
    reader->setDevice(molInfoReply);
    std::vector<std::array<std::string,2> >mol;

    reader->readNext();
    reader->readNext();
    while (!(reader->atEnd())){
        int type=reader->readNext();
        if(type==reader->StartElement){
            std::string name=reader->name().toString().toStdString();
            std::string data=reader->readElementText().toStdString();
            mol.push_back({name,data});
        }
    }
    csidReply->deleteLater();
    molInfoReply->deleteLater();
    delete reader;
    return new Molecule(mol);
}

