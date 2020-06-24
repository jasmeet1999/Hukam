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

#include <bb/cascades/Menu>
#include <bb/cascades/ActionItem>
#include <bb/cascades/SettingsActionItem>
#include <bb/cascades/HelpActionItem>

#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>

#include <bb/cascades/Slider>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/Header>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Button>
#include <bb/cascades/Divider>

#include <bb/cascades/VisualStyle>
#include <bb/cascades/ThemeSupport>

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

    QSettings settings("jasmeetsingh","Hukam");

    // load user setting for font
    if (settings.value("data/fontSize").isNull())
        settings.setValue("data/fontSize",8);

    // load user setting for theme
    if (settings.value("data/theme").isNull())
        settings.setValue("data/theme","bright");

    // set font and theme
    fontValue = settings.value("data/fontSize").toInt();
    QString theme = settings.value("data/theme").toString();
    if (theme == "bright")
        themeState = false;
    if (theme == "dark")
        themeState = true;
    themeChange(themeState);

    mNavigationPane = new NavigationPane;

    // Enable Application Menu
    if (!Application::instance()->isMenuEnabled()) {
        Application::instance()->setMenuEnabled(true);
    }

    // Adding Menu items
    Menu *menu = Menu::create()
        .addAction(ActionItem::create().title("About").image("asset:///glyph/about.png"))
        .addAction(ActionItem::create().title("Feedback").image("asset:///glyph/feedback.png"))
        .settings(SettingsActionItem::create());

    connect(menu->actionAt(0),SIGNAL(triggered()),this,SLOT(aboutTriggered()));
    connect(menu->actionAt(1),SIGNAL(triggered()),this,SLOT(feedbackTriggered()));
    connect(menu->settingsAction(),SIGNAL(triggered()),this,SLOT(settingsTriggered()));

    // Creating title bar
    QString titleName = QString::fromUtf8("ਹੁਕਮਨਾਮਾ");
    TitleBar *title = new TitleBar();
    title->setTitle(titleName);

    // Root Scene for app
    ScrollView *scrollView = ScrollView::create().scrollMode(ScrollMode::Vertical);
    scrollView->setHorizontalAlignment(HorizontalAlignment::Fill);

    Container *pContainer = new Container();
    pContainer->setLayout(StackLayout::create());
    pContainer->setHorizontalAlignment(HorizontalAlignment::Fill);

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
    mahalaLabel->textStyle()->setFontSize(FontSize::PointValue);
    mahalaLabel->textStyle()->setFontSizeValue(fontValue+1.5);
    baniLabel->textStyle()->setFontSize(FontSize::PointValue);
    baniLabel->textStyle()->setFontSizeValue(fontValue);

    connect(this,SIGNAL(fSize(float)),mahalaLabel->textStyle(),SLOT(setFontSizeValue(float)));
    connect(this,SIGNAL(fSize(float)),baniLabel->textStyle(),SLOT(setFontSizeValue(float)));

    pContainer->add(mahalaLabel);
    pContainer->add(baniLabel);

    scrollView->setContent(pContainer);

    Page *page = new Page();
    page->setTitleBar(title);
    page->setContent(scrollView);

    mNavigationPane->push(page);

    Application::instance()->setMenu(menu);
    Application::instance()->setScene(mNavigationPane);
}

void ApplicationUI::aboutTriggered() {
    QString aboutTitleText = "About";
    TitleBar *aboutTitle = new TitleBar();
    aboutTitle->setTitle(aboutTitleText);
    Page *aboutPage = new Page;
    aboutPage->setTitleBar(aboutTitle);
    mNavigationPane->push(aboutPage);
}

void ApplicationUI::feedbackTriggered() {
    bb::system::InvokeManager invokeManager;
    bb::system::InvokeRequest request;

    QByteArray url = QUrl::toPercentEncoding("mailto:jasmeetsinghkhokhar.com?subject=Hukam Feedback");

    request.setTarget("sys.pim.uib.email.hybridcomposer");
    request.setAction("bb.action.OPEN");
    request.setMimeType("text/plain");
    request.setUri(QUrl::fromPercentEncoding(url));

    bb::system::InvokeTargetReply *reply = invokeManager.invoke(request);
}

void ApplicationUI::settingsTriggered() {
    Divider *div = Divider::create();

    QString settingsTitleText = "Settings";
    TitleBar *settingsTitle = new TitleBar();
    settingsTitle->setTitle(settingsTitleText);

    Page *settingsPage = new Page;

    ScrollView *sScrollView = ScrollView::create().scrollMode(ScrollMode::Vertical);
    sScrollView->setHorizontalAlignment(HorizontalAlignment::Fill);

    Container *sContainer = new Container();
    sContainer->setLayout(StackLayout::create());
    UIConfig *ui = sContainer->ui();
    sContainer->setBottomPadding(ui->du(2));

    Header *theme = new Header();
    theme->setTitle("Theme");

    Container *themeContainer = new Container();
    themeContainer->setLayout(DockLayout::create());
    themeContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    themeContainer->setTopPadding(ui->du(1));
    themeContainer->setRightPadding(ui->du(4));
    themeContainer->setLeftPadding(ui->du(4));

    ToggleButton *themeSwitch = ToggleButton::create();
    themeSwitch->setChecked(themeState);
    themeSwitch->setHorizontalAlignment(HorizontalAlignment::Right);
    connect(themeSwitch,SIGNAL(checkedChanged(bool)),this,SLOT(themeChange(bool)));
    Label *themeLabel = Label::create().text("Dark Theme");
    themeLabel->setHorizontalAlignment(HorizontalAlignment::Left);

    themeContainer->add(themeLabel);
    themeContainer->add(themeSwitch);

    Header *display = new Header();
    display->setTitle("Display");

    Container *displayContainer = new Container();
    displayContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    displayContainer->setTopPadding(ui->du(1));
    displayContainer->setRightPadding(ui->du(4));
    displayContainer->setLeftPadding(ui->du(4));

    Label *fontSize = Label::create().text("Font Size");
    Slider *slider = Slider::create();
    slider->setTopPadding(ui->du(2));
    slider->setBottomPadding(ui->du(2));
    Label *fontCheckTitle = Label::create().text(QString::fromUtf8("ਸੂਹੀ ਮਹਲਾ ੫ ॥"));
    fontCheckTitle->setHorizontalAlignment(HorizontalAlignment::Center);
    Label *fontCheckBody = new Label();
    fontCheckBody->setHorizontalAlignment(HorizontalAlignment::Center);
    fontCheckBody->setMultiline(true);
    QString checkBani = QString::fromUtf8("ਸੰਤਾ ਕੇ ਕਾਰਜਿ ਆਪਿ ਖਲੋਇਆ ਹਰਿ ਕੰਮੁ ਕਰਾਵਣਿ ਆਇਆ ਰਾਮ ॥\n");
    checkBani.append(QString::fromUtf8("ਧਰਤਿ ਸੁਹਾਵੀ ਤਾਲੁ ਸੁਹਾਵਾ ਵਿਚਿ ਅੰਮ੍ਰਿਤ ਜਲੁ ਛਾਇਆ ਰਾਮ ॥\n"));
//    checkBani.append(QString::fromUtf8("ਅੰਮ੍ਰਿਤ ਜਲੁ ਛਾਇਆ ਪੂਰਨ ਸਾਜੁ ਕਰਾਇਆ ਸਗਲ ਮਨੋਰਥ ਪੂਰੇ ॥\n"));
//    checkBani.append(QString::fromUtf8("ਜੈ ਜੈ ਕਾਰੁ ਭਇਆ ਜਗ ਅੰਤਰਿ ਲਾਥੇ ਸਗਲ ਵਿਸੂਰੇ ॥\n"));
//    checkBani.append(QString::fromUtf8("ਪੂਰਨ ਪੁਰਖ ਅਚੁਤ ਅਬਿਨਾਸੀ ਜਸੁ ਵੇਦ ਪੁਰਾਣੀ ਗਾਇਆ ॥\n"));
//    checkBani.append(QString::fromUtf8("ਅਪਨਾ ਬਿਰਦੁ ਰਖਿਆ ਪਰਮੇਸਰਿ ਨਾਨਕ ਨਾਮੁ ਧਿਆਇਆ ॥੧॥"));
    fontCheckBody->setText(checkBani);
    fontCheckTitle->textStyle()->setTextAlign(TextAlign::Center);
    fontCheckBody->textStyle()->setTextAlign(TextAlign::Center);

    fontCheckTitle->textStyle()->setFontSize(FontSize::PointValue);
    fontCheckTitle->textStyle()->setFontSizeValue(fontValue+1.5);
    fontCheckBody->textStyle()->setFontSize(FontSize::PointValue);
    fontCheckBody->textStyle()->setFontSizeValue(fontValue);

    connect(this,SIGNAL(fSize(float)),fontCheckTitle->textStyle(),SLOT(setFontSizeValue(float)));
    connect(this,SIGNAL(fSize(float)),fontCheckBody->textStyle(),SLOT(setFontSizeValue(float)));

    slider->setRange(5,13);
    slider->setValue(fontValue);

    connect(slider,SIGNAL(valueChanged(float)),this,SLOT(updateValue(float)));



    Container *larivaarContainer = new Container();
    larivaarContainer->setLayout(DockLayout::create());
    larivaarContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    larivaarContainer->setTopPadding(ui->du(1));
    larivaarContainer->setRightPadding(ui->du(4));
    larivaarContainer->setLeftPadding(ui->du(4));
    larivaarContainer->setBottomPadding(ui->du(1));

    ToggleButton *larivaarSwitch = ToggleButton::create();
    //larivaarSwitch->setChecked(themeState);
    larivaarSwitch->setHorizontalAlignment(HorizontalAlignment::Right);
    //connect(larivaarSwitch,SIGNAL(checkedChanged(bool)),this,SLOT(themeChange(bool)));
    Label *larivaar = Label::create().text("Larivaar");
    larivaar->setHorizontalAlignment(HorizontalAlignment::Left);

    larivaarContainer->add(larivaar);
    larivaarContainer->add(larivaarSwitch);

    displayContainer->add(fontSize);
    displayContainer->add(slider);
    displayContainer->add(fontCheckTitle);
    displayContainer->add(fontCheckBody);

    Container *resetContainer = new Container();
    resetContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    Button *resetDefault = Button::create("Reset Default");
    resetDefault->setHorizontalAlignment(HorizontalAlignment::Fill);
    resetContainer->setLeftPadding(ui->du(2));
    resetContainer->setRightPadding(ui->du(2));
    resetContainer->setTopPadding(ui->du(2));
    resetContainer->setBottomPadding(ui->du(1));
    connect(resetDefault,SIGNAL(clicked()),this,SLOT(resetSettings()));
    resetContainer->add(resetDefault);

    sContainer->add(theme);
    sContainer->add(themeContainer);
    sContainer->add(display);
    sContainer->add(displayContainer);
    sContainer->add(larivaarContainer);
    sContainer->add(div);
    sContainer->add(resetContainer);

    sScrollView->setContent(sContainer);
    settingsPage->setContent(sScrollView);

    settingsPage->setTitleBar(settingsTitle);

    mNavigationPane->push(settingsPage);
}

void ApplicationUI::themeChange(bool state) {
    QSettings settings("jasmeetsingh","Hukam");
    themeState = state;
    if (state) {
        Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Dark);
        settings.setValue("data/theme","dark");
    }
    else {
        Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
        settings.setValue("data/theme","bright");
    }
}

void ApplicationUI::updateValue(float u_value) {
    int value = u_value;
    QSettings settings("jasmeetsingh","Hukam");
    settings.setValue("data/fontSize",value);
    fontValue = settings.value("data/fontSize").toInt();
    settings.sync();
    int a=0;
    a++;
    emit(fSize(fontValue));
}

void ApplicationUI::resetSettings() {
    QSettings settings("jasmeetsingh","Hukam");
    // reset theme settings
    settings.setValue("data/theme","bright");
    Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
    themeState = false;
    // reset font size
    settings.setValue("data/fontSize",8);
    fontValue = settings.value("data/fontSize").toInt();
    // Synchronize to default values
    settings.sync();
    emit(fSize(8));
    mNavigationPane->pop();
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
