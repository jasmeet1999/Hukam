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

public Q_SLOTS:
    void getRequest();

private Q_SLOTS:
    void onReply(QNetworkReply* reply);

private:
    QNetworkAccessManager *manager;
};

#endif /* REQUESTBANI_HPP_ */
