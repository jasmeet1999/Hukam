/*
 * AppSetting.cpp
 *
 *  Created on: Jun 28, 2020
 *      Author: jasmeetsingh
 */

#include <src/AppSetting.hpp>

#include <bb/cascades/Application>

#include <bb/cascades/VisualStyle>
#include <bb/cascades/ThemeSupport>

using namespace bb::cascades;

AppSetting::AppSetting(QObject *parent):QObject(parent)
{
    QSettings settings("jasmeetsingh","Hukam");

    if (settings.value("data/theme").isNull())
        settings.setValue("data/theme","bright");

    if (settings.value("data/fontSize").isNull())
          settings.setValue("data/fontSize",8);

    if (settings.value("data/larivaar").isNull())
          settings.setValue("data/larivaar","off");

    fontValue = settings.value("data/fontSize").toInt();

    QString theme = settings.value("data/theme").toString();
    if (theme == "bright")
        themeState = false;
    if (theme == "dark")
        themeState = true;

    QString larivaar = settings.value("data/larivaar").toString();
    if (larivaar == "off")
        larivaarState = false;
    if (larivaar == "on")
        larivaarState = true;

    themeChange(themeState);
}

void AppSetting::themeChange(bool state) {
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

void AppSetting::updateFontValue(float u_value) {
    int value = u_value;
    QSettings settings("jasmeetsingh","Hukam");
    settings.setValue("data/fontSize",value);
    fontValue = settings.value("data/fontSize").toInt();
    settings.sync();
    emit(fSize(fontValue));
    emit(fSizeT(fontValue+1.5));
}

void AppSetting::larivaarToggle(bool state) {
    QSettings settings("jasmeetsingh","Hukam");
    larivaarState = state;
    if (state)
        settings.setValue("data/larivaar","on");
    else
        settings.setValue("data/larivaar","off");
    emit(updateBani(larivaarState));
}

void AppSetting::resetSettings() {
    QSettings settings("jasmeetsingh","Hukam");
    // reset theme settings
    settings.setValue("data/theme","bright");
    Application::instance()->themeSupport()->setVisualStyle(VisualStyle::Bright);
    setThemeState(false);
    // reset font size
    settings.setValue("data/fontSize",8);
    fontValue = settings.value("data/fontSize").toInt();
    settings.setValue("data/larivaar","off");
    larivaarState = false;
    // Synchronize to default values
    settings.sync();
    emit(resetControlsVisual(getThemeState()));
    emit(resetControlsVisual(fontValue));
    emit(fSize(8));
    emit(fSizeT(9.5));
    //mNavigationPane->pop();
}

bool AppSetting::getThemeState() {
    return themeState;
}

void AppSetting::setThemeState(bool themeState) {
    this->themeState = themeState;
}

bool AppSetting::getLarivaarState() {
    return larivaarState;
}

void AppSetting::setLarivaarState(bool larivaarState) {
    this->larivaarState = larivaarState;
}

float AppSetting::getFontValue() {
    return fontValue;
}

void AppSetting::setFontValue(float fontValue) {
    this->fontValue = fontValue;
}

AppSetting::~AppSetting()
{}

