/*
 * AppSetting.hpp
 *
 *  Created on: Jun 28, 2020
 *      Author: jasmeetsingh
 */

#ifndef APPSETTING_HPP_
#define APPSETTING_HPP_

#include <QtCore/QObject>

class AppSetting: public QObject
{
    Q_OBJECT
public:
    AppSetting(QObject *parent = 0);
    virtual ~AppSetting();
    bool getThemeState();
    void setThemeState(bool);
    bool getLarivaarState();
    void setLarivaarState(bool);
    float getFontValue();
    void setFontValue(float);


Q_SIGNALS:
    void fSize(float);
    void fSizeT(float);
    void resetControlsVisual(bool);
    void resetControlsVisual(float);
    void updateBani(bool);

public slots:
    void themeChange(bool);
    void updateFontValue(float);
    void larivaarToggle(bool);
    void resetSettings();

private:
    bool themeState;
    bool larivaarState;
    float fontValue;
};

#endif /* APPSETTING_HPP_ */
