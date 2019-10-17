#include "hmi.h"
#include "ui_hmi.h"
#include <wiringPi.h>

Hmi::Hmi(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Hmi)
{
    if (wiringPiSetup () < 0)
        exit(-1);
    ui->setupUi(this);
    setupGpio();
}

Hmi::~Hmi()
{
    delete ui;
}

void Hmi::setupGpio()
{
    pinMode (14, OUTPUT) ;         // aka BCM_GPIO pin 17

    for(int i = 0; i < 25; i++)
    {
        digitalWrite (14, 1) ;       // On
        delayMicroseconds(1) ;       // uS
        digitalWrite (14, 0) ;       // Off
        delayMicroseconds(1) ;       // uS
    }
}
