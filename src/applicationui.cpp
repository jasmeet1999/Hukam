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
#include <bb/cascades/Label>
#include <bb/cascades/TitleBar>
#include <bb/cascades/ScrollView>

#include "RequestBani.hpp"

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

    QString titleName = QString::fromUtf8("ਹੁਕਮਨਾਮਾ");

    TitleBar *title = new TitleBar();
    title->setTitle(titleName);

    Container *pContainer = new Container();
    pContainer->setLayout(StackLayout::create());

    ScrollView *scrollView = ScrollView::create().scrollMode(ScrollMode::Vertical);

    Label *mahalaLabel = new Label();
    Label *baniLabel = new Label();

    RequestBani *requestBani = new RequestBani();
    connect(requestBani,SIGNAL(raagAndMahalaComplete(const QString)),mahalaLabel,SLOT(setText(const QString)));
    connect(requestBani,SIGNAL(baniComplete(const QString)),baniLabel,SLOT(setText(const QString)));
    requestBani->getRequest();

    mahalaLabel->setHorizontalAlignment(HorizontalAlignment::Center);
    baniLabel->setHorizontalAlignment(HorizontalAlignment::Center);

    baniLabel->textStyle()->setTextAlign(TextAlign::Center);

    baniLabel->setMultiline(true);

    pContainer->add(mahalaLabel);
    pContainer->add(baniLabel);

    scrollView->setContent(pContainer);

    Page *page = new Page();
    page->setTitleBar(title);
    page->setContent(scrollView);

    Application::instance()->setMenuEnabled(false);
    Application::instance()->setScene(page);
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
