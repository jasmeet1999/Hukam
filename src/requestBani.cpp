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
#include "AppSetting.hpp"

RequestBani::RequestBani(QObject *parent):
QObject(parent),
manager(new QNetworkAccessManager(this))
{
    AppSetting *appSetting = new AppSetting();
    this->larivaarState = appSetting->getLarivaarState();
}

void RequestBani::getRequest(){
    const QString API = "https://api.gurbaninow.com/v2/hukamnama/today";

    const QUrl url(API);

    QNetworkRequest request(url);

    manager->get(request);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReply(QNetworkReply*)));
}

void RequestBani::sendGETRequest(bool larivaarState) {

    this->larivaarState = larivaarState;

    const QString API = "https://api.gurbaninow.com/v2/hukamnama/today";

    const QUrl url(API);

    QNetworkRequest request(url);

    manager->get(request);
}

void RequestBani::onReply(QNetworkReply *reply) {

    bb::data::JsonDataAccess jda;
    QVariant jsonData = jda.load(reply);
    QMap<QString ,QVariant> jsonRoot = jsonData.toMap();

    QString raagMahala;
    QString bani;

    bool error = jsonRoot.find("error").value().toBool();

    if (error == false) {
        QList<QVariant> hukamnama = jsonRoot.value("hukamnama").toList();

        QMap<QString ,QVariant> mahala = hukamnama.at(0).toMap();

        QMap<QString ,QVariant> line = mahala.value("line").toMap();

        if (!larivaarState) {
            QMap<QString ,QVariant> mGurmukhi = line.value("gurmukhi").toMap();
            raagMahala = mGurmukhi.find("unicode").value().toString().toUtf8();
        } else {
            QMap<QString ,QVariant> mLarivaar = line.value("larivaar").toMap();
            raagMahala = mLarivaar.find("unicode").value().toString().toUtf8();
        }

        for (int i = 1; i < hukamnama.length(); i++) {
            QMap<QString ,QVariant> baniLine = hukamnama.at(i).toMap();

            QMap<QString ,QVariant> line = baniLine.value("line").toMap();

            if (!larivaarState) {
                QMap<QString ,QVariant> gurmukhi = line.value("gurmukhi").toMap();
                bani = bani + gurmukhi.find("unicode").value().toString().toUtf8();
            } else {
                QMap<QString ,QVariant> larivaar = line.value("larivaar").toMap();
                bani = bani + larivaar.find("unicode").value().toString().toUtf8();
            }

            bani = bani + "\n";
        }

        emit raagAndMahalaComplete(QString::fromUtf8(raagMahala.toAscii()));
        emit baniComplete(QString::fromUtf8(bani.toAscii()));
    }
}

RequestBani::~RequestBani()
{
}

