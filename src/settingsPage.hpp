/*
 * settingsPage.hpp
 *
 *  Created on: Jun 28, 2020
 *      Author: jasmeetsingh
 */

#ifndef SETTINGSPAGE_HPP_
#define SETTINGSPAGE_HPP_

#include <bb/cascades/Page>
#include <bb/cascades/Container>

#include "AppSetting.hpp"

class SettingsPage: public bb::cascades::Page
{
public:
    SettingsPage(AppSetting*);
    virtual ~SettingsPage();
    Page* build();

private:
    Page *page;
    AppSetting *appSetting;
    bb::cascades::Container* getThemeContainer();
    bb::cascades::Container* getDisplayContainer();
    bb::cascades::Container* getLarivaarContainer();
    bb::cascades::Container* getResetContainer();
};

#endif /* SETTINGSPAGE_HPP_ */
