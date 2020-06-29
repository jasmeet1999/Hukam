/*
 * settingsPage.cpp
 *
 *  Created on: Jun 28, 2020
 *      Author: jasmeetsingh
 */

#include <src/settingsPage.hpp>

#include <bb/cascades/TitleBar>
#include <bb/cascades/ScrollView>
#include <bb/cascades/UIConfig>
#include <bb/cascades/StackLayout>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Header>
#include <bb/cascades/Label>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/Button>
#include <bb/cascades/Slider>
#include <bb/cascades/Divider>

using namespace bb::cascades;

SettingsPage::SettingsPage(AppSetting *appSetting):page(new Page())
{
    this->appSetting = appSetting;
}

Page* SettingsPage::build() {
    QString settingsTitleText = "Settings";
    TitleBar *settingsTitle = new TitleBar();
    settingsTitle->setTitle(settingsTitleText);

    ScrollView *sScrollView = ScrollView::create().scrollMode(ScrollMode::Vertical);
    sScrollView->setHorizontalAlignment(HorizontalAlignment::Fill);

    Container *sContainer = new Container();
    sContainer->setLayout(StackLayout::create());
    sContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    UIConfig *ui = sContainer->ui();
    sContainer->setBottomPadding(ui->du(2));

    Header *themeHeader = new Header();
    themeHeader->setTitle("Theme");
    Container *themeContainer = getThemeContainer();

    Header *displayHeader = new Header();
    displayHeader->setTitle("Display");
    Container *displayContainer = getDisplayContainer();

    Container *larivaarContainer = getLarivaarContainer();

    Divider *div = Divider::create();

    Container *resetContainer = getResetContainer();

    sContainer->add(themeHeader);
    sContainer->add(themeContainer);
    sContainer->add(displayHeader);
    sContainer->add(displayContainer);
    sContainer->add(larivaarContainer);
    sContainer->add(div);
    sContainer->add(resetContainer);

    sScrollView->setContent(sContainer);

    page->setContent(sScrollView);
    page->setTitleBar(settingsTitle);

    return page;
}

Container* SettingsPage::getThemeContainer() {
    Container *themeContainer = new Container();
    themeContainer->setLayout(DockLayout::create());
    themeContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    UIConfig *ui = themeContainer->ui();
    themeContainer->setTopPadding(ui->du(1));
    themeContainer->setRightPadding(ui->du(4));
    themeContainer->setLeftPadding(ui->du(4));

    Label *themeLabel = Label::create().text("Dark Theme");
    themeLabel->setHorizontalAlignment(HorizontalAlignment::Left);
    ToggleButton *themeSwitch = ToggleButton::create();
    themeSwitch->setChecked(appSetting->getThemeState());
    themeSwitch->setHorizontalAlignment(HorizontalAlignment::Right);
    connect(themeSwitch,SIGNAL(checkedChanged(bool)),appSetting,SLOT(themeChange(bool)));
    connect(appSetting,SIGNAL(resetControlsVisual(bool)),themeSwitch,SLOT(setChecked(bool)));

    themeContainer->add(themeLabel);
    themeContainer->add(themeSwitch);

    return themeContainer;
}

Container* SettingsPage::getDisplayContainer() {
    Container *displayContainer = new Container();
    displayContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    UIConfig *ui = displayContainer->ui();
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
    fontCheckBody->setText(checkBani);
    fontCheckTitle->textStyle()->setTextAlign(TextAlign::Center);
    fontCheckBody->textStyle()->setTextAlign(TextAlign::Center);

    fontCheckTitle->textStyle()->setFontSize(FontSize::PointValue);
    fontCheckTitle->textStyle()->setFontSizeValue(appSetting->getFontValue()+1.5);
    fontCheckBody->textStyle()->setFontSize(FontSize::PointValue);
    fontCheckBody->textStyle()->setFontSizeValue(appSetting->getFontValue());

    connect(appSetting,SIGNAL(fSizeT(float)),fontCheckTitle->textStyle(),SLOT(setFontSizeValue(float)));
    connect(appSetting,SIGNAL(fSize(float)),fontCheckBody->textStyle(),SLOT(setFontSizeValue(float)));

    slider->setRange(5,13);
    slider->setValue(appSetting->getFontValue());

    connect(slider,SIGNAL(valueChanged(float)),appSetting,SLOT(updateFontValue(float)));
    connect(appSetting,SIGNAL(resetControlsVisual(float)),slider,SLOT(setValue(float)));

    displayContainer->add(fontSize);
    displayContainer->add(slider);
    displayContainer->add(fontCheckTitle);
    displayContainer->add(fontCheckBody);

    return displayContainer;
}

Container* SettingsPage::getLarivaarContainer() {
    Container *larivaarContainer = new Container();
    larivaarContainer->setLayout(DockLayout::create());
    larivaarContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    UIConfig *ui = larivaarContainer->ui();
    larivaarContainer->setTopPadding(ui->du(1));
    larivaarContainer->setRightPadding(ui->du(4));
    larivaarContainer->setLeftPadding(ui->du(4));
    larivaarContainer->setBottomPadding(ui->du(1));

    ToggleButton *larivaarSwitch = ToggleButton::create();
    larivaarSwitch->setChecked(appSetting->getLarivaarState());
    larivaarSwitch->setHorizontalAlignment(HorizontalAlignment::Right);
    connect(larivaarSwitch,SIGNAL(checkedChanged(bool)),appSetting,SLOT(larivaarToggle(bool)));
    Label *larivaar = Label::create().text("Larivaar");
    larivaar->setHorizontalAlignment(HorizontalAlignment::Left);
    connect(appSetting,SIGNAL(resetControlsVisual(bool)),larivaarSwitch,SLOT(setChecked(bool)));

    larivaarContainer->add(larivaar);
    larivaarContainer->add(larivaarSwitch);

    return larivaarContainer;
}

Container* SettingsPage::getResetContainer() {
    Container *resetContainer = new Container();
    resetContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    Button *resetDefault = Button::create("Reset Default");
    resetDefault->setHorizontalAlignment(HorizontalAlignment::Fill);
    UIConfig *ui = resetContainer->ui();
    resetContainer->setLeftPadding(ui->du(2));
    resetContainer->setRightPadding(ui->du(2));
    resetContainer->setTopPadding(ui->du(2));
    resetContainer->setBottomPadding(ui->du(1));
    connect(resetDefault,SIGNAL(clicked()),appSetting,SLOT(resetSettings()));
    resetContainer->add(resetDefault);

    return resetContainer;
}

SettingsPage::~SettingsPage()
{
}

