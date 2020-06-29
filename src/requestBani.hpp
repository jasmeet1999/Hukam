/*
 * requestBani.hpp
 *
 *  Created on: Jun 10, 2020
 *      Author: jasmeetsingh
 */

#ifndef REQUESTBANI_HPP_
#define REQUESTBANI_HPP_

#include <QtCore/QObject>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class QNetworkAccessManager;

class RequestBani: public QObject
{
    Q_OBJECT
public:
    RequestBani(QObject *parent = 0);
    virtual ~RequestBani();

Q_SIGNALS:
 void raagAndMahalaComplete(const QString);
 void baniComplete(const QString);

public Q_SLOTS:
    void getRequest();
    void sendGETRequest(bool);

private Q_SLOTS:
    void onReply(QNetworkReply* reply);

private:
    QNetworkAccessManager *manager;
    bool larivaarState;
};

#endif /* REQUESTBANI_HPP_ */
