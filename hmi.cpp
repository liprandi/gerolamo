#include "hmi.h"
#include "ui_hmi.h"


Hmi::Hmi(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Hmi)
{
    ui->setupUi(this);
}

Hmi::~Hmi()
{
}

