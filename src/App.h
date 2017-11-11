#pragma once

#include "AppBase.h"


class App : public AppBase
{
    Q_OBJECT;

private:
    void  _preMain() override;
};


inline
void App::_preMain()
{
};
