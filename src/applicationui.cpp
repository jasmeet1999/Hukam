/*
 * Copyright (c) 2011-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/LocaleHandler>

#include <bb/cascades/Container>
#include <bb/cascades/StackLayout>
#include <bb/cascades/Page>

#include <bb/data/JsonDataAccess>

using namespace bb::cascades;

ApplicationUI::ApplicationUI() :
        QObject()
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    Container *pContainer = new Container();
    pContainer->setLayout(StackLayout::create());

    const QString API = "https://api.gurbaninow.com/v2/hukamnama/today";

    const QUrl url(API);

    QNetworkRequest request(url);

    manager = new QNetworkAccessManager();
    manager->get(request);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onReply(QNetworkReply*)));

    Page *page = new Page();
    page->setContent(pContainer);

    Application::instance()->setScene(page);
}

void ApplicationUI::onReply(QNetworkReply *reply)
{
    bb::data::JsonDataAccess json;
    QVariant list = json.load(reply);
    QMap<QString ,QVariant> map = list.toMap();

    bool check = map.contains("error");

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
        int a;
        a++;
    }

    int a=1;
    a++;

}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("Hukam_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}
