#include "hx711config.h"
#include "ui_hx711config.h"
#include <zcontrol.h>

Hx711Config::Hx711Config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Hx711Config)
{
    ui->setupUi(this);
    ui->cb_gpioclk->setCurrentText(QString("GPIO%1").arg(ZControl::g_config->m_hx711.gpioclk, 2, 10, QChar('0')));
    ui->cb_gpiodata1->setCurrentText(QString("GPIO%1").arg(ZControl::g_config->m_hx711.gpiodata1, 2, 10, QChar('0')));
    ui->cb_gpiodata2->setCurrentText(QString("GPIO%1").arg(ZControl::g_config->m_hx711.gpiodata2, 2, 10, QChar('0')));
    ui->cb_gpiodata3->setCurrentText(QString("GPIO%1").arg(ZControl::g_config->m_hx711.gpiodata3, 2, 10, QChar('0')));
    ui->le_offset1->setText(QString("%1").arg(ZControl::g_config->m_hx711.offset1));
    ui->le_offset2->setText(QString("%1").arg(ZControl::g_config->m_hx711.offset2));
    ui->le_offset3->setText(QString("%1").arg(ZControl::g_config->m_hx711.offset3));
    ui->le_gain1->setText(QString("%1").arg(ZControl::g_config->m_hx711.gain1));
    ui->le_gain2->setText(QString("%1").arg(ZControl::g_config->m_hx711.gain2));
    ui->le_gain3->setText(QString("%1").arg(ZControl::g_config->m_hx711.gain3));
    connect(ui->cb_gpioclk, SIGNAL(currentTextChanged(const QString&)), this, SLOT(gpioclkChanged(const QString&)));
    connect(ui->cb_gpiodata1, SIGNAL(currentTextChanged(const QString&)), this, SLOT(gpiodata1Changed(const QString&)));
    connect(ui->cb_gpiodata2, SIGNAL(currentTextChanged(const QString&)), this, SLOT(gpiodata2Changed(const QString&)));
    connect(ui->cb_gpiodata3, SIGNAL(currentTextChanged(const QString&)), this, SLOT(gpiodata3Changed(const QString&)));
    connect(ui->le_offset1, SIGNAL(editingFinished()), this, SLOT(offsetGainChanged()));
    connect(ui->le_offset2, SIGNAL(editingFinished()), this, SLOT(offsetGainChanged()));
    connect(ui->le_offset3, SIGNAL(editingFinished()), this, SLOT(offsetGainChanged()));
    connect(ui->le_gain1, SIGNAL(editingFinished()), this, SLOT(offsetGainChanged()));
    connect(ui->le_gain2, SIGNAL(editingFinished()), this, SLOT(offsetGainChanged()));
    connect(ui->le_gain3, SIGNAL(editingFinished()), this, SLOT(offsetGainChanged()));
}

Hx711Config::~Hx711Config()
{
    delete ui;
}

void Hx711Config::gpioclkChanged(const QString& text)
{
    if(text.length() == 6 && text.contains("GPIO"))
        ZControl::g_config->m_hx711.gpioclk = text.mid(4, 2).toInt();
}
void Hx711Config::gpiodata1Changed(const QString& text)
{
    if(text.length() == 6 && text.contains("GPIO"))
        ZControl::g_config->m_hx711.gpiodata1 = text.mid(4, 2).toInt();
}
void Hx711Config::gpiodata2Changed(const QString& text)
{
    if(text.length() == 6 && text.contains("GPIO"))
        ZControl::g_config->m_hx711.gpiodata2 = text.mid(4, 2).toInt();
}
void Hx711Config::gpiodata3Changed(const QString& text)
{
    if(text.length() == 6 && text.contains("GPIO"))
        ZControl::g_config->m_hx711.gpiodata3 = text.mid(4, 2).toInt();
}
void Hx711Config::offsetGainChanged()
{

}
