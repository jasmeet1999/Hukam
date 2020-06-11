/*
 * requestBani.cpp
 *
 *  Created on: Jun 10, 2020
 *      Author: jasmeetsingh
 */

#include <src/requestBani.hpp>

#include <QtCore/QObject>

#include <QtNetwork/QnetworkRequest>

#include <bb/data/JsonDataAccess>

RequestBani::RequestBani(QObject *parent):
QObject(parent),
manager(new QNetworkAccessManager(this))
{

}

void RequestBani::getRequest(){
    const QString API = "https://api.gurbaninow.com/v2/hukamnama/today";

    const QUrl url(API);

    QNetworkRequest request(url);

    manager->get(request);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReply(QNetworkReply*)));
}

void RequestBani::onReply(QNetworkReply *reply){
    bb::data::JsonDataAccess json;
    QVariant list = json.load(reply);
    QMap<QString ,QVariant> map = list.toMap();

    bool check = map.contains("error");
    Q_UNUSED(check);

    QString ans = map.value("error").toString();

    QMap<QString ,QVariant> date = map.value("date").toMap();
    QMap<QString ,QVariant> gregorian = date.value("gregorian").toMap();

    QMap<QString ,QVariant> nanakshahi = date.value("nanakshahi").toMap();
    QMap<QString ,QVariant> punjabi = nanakshahi.value("punjabi").toMap();

    QMap<QString ,QVariant>::const_iterator punjabiDateItr = punjabi.begin();

    QString pbDate = " "+punjabiDateItr.key();
    QString pbDateN = " "+punjabiDateItr.value().toString().toUtf8();
    QString pb = QString::fromLatin1("%1 : %2").arg(punjabiDateItr.key(),pbDateN);

    QMap<QString ,QVariant>::const_iterator dateItr = gregorian.begin();
    dateItr++;
    QString key = dateItr.key();
    QString val = " ";
    val.append(dateItr.value().toString());

    if (map.value("error").toString() == "false"){

    }

    emit complete(pb);
}

RequestBani::~RequestBani()
{
}

