#include "debugwidget.h"
#include "ui_debugwidget.h"
#include "zcontrol.h"
#include <QTimer>
#include <QMessageBox>


DebugWidget::DebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugWidget)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    m_timer->start();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui->pbQuit, SIGNAL(pressed()), this, SLOT(quit()));
    connect(ui->le_dbg_targetStepper, SIGNAL(editingFinished()), this, SLOT(moveStepper()));
    if(ZControl::g_stepper)
    {
        ZControl::g_stepper->getParameters(m_stepperParam[0], m_stepperParam[1], m_stepperParam[2]);
    }
    else
    {
        for(int i = 0; i < 3; i++)
            m_stepperParam[i] = 0.;
    }
    ui->le_dbg_speed->setText(QString("%1").arg(m_stepperParam[0]));
    ui->le_dbg_accel->setText(QString("%1").arg(m_stepperParam[1]));
    ui->le_dbg_jerk->setText(QString("%1").arg(m_stepperParam[2]));
    connect(ui->le_dbg_speed, SIGNAL(editingFinished()), this, SLOT(setStepper()));
    connect(ui->le_dbg_accel, SIGNAL(editingFinished()), this, SLOT(setStepper()));
    connect(ui->le_dbg_jerk, SIGNAL(editingFinished()), this, SLOT(setStepper()));
}

DebugWidget::~DebugWidget()
{
    m_timer->stop();
    delete m_timer;
    delete ui;
}
void DebugWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    if(isVisible())
        updateHx711();
}

void DebugWidget::setStepper()
{
    m_stepperParam[0] = ui->le_dbg_speed->text().toDouble();
    m_stepperParam[1] = ui->le_dbg_accel->text().toDouble();
    m_stepperParam[2] = ui->le_dbg_jerk->text().toDouble();
    if(ZControl::g_stepper)
    {
        ZControl::g_stepper->setParameters(m_stepperParam[0], m_stepperParam[1], m_stepperParam[2]);
        ZControl::g_stepper->getParameters(m_stepperParam[0], m_stepperParam[1], m_stepperParam[2]);
        ui->le_dbg_speed->setText(QString("%1").arg(m_stepperParam[0]));
        ui->le_dbg_accel->setText(QString("%1").arg(m_stepperParam[1]));
        ui->le_dbg_jerk->setText(QString("%1").arg(m_stepperParam[2]));
    }
}
void DebugWidget::moveStepper()
{
    if(ZControl::g_stepper)
    {
        double position = ui->le_dbg_targetStepper->text().toDouble();
        if(!ZControl::g_stepper->go(position))
        {
            QMessageBox msgBox;
            msgBox.setText(QString("Posição %1 fora do alcance").arg(position));
            msgBox.exec();
        }
    }
}

void DebugWidget::quit()
{
    QApplication::quit();
}
void DebugWidget::updateHx711()
{
    if(ZControl::g_hx711)
    {
        unsigned count;
        unsigned errors;
        unsigned index;
        long long diff;
        long long minInterval_h = 1e9;
        long long maxInterval_h = 0;
        double avgInterval_h = 0;
        long long minInterval_l = 1e9;
        long long maxInterval_l = 0;
        double avgInterval_l = 0;
        int cntAverange_h = 0;
        int cntAverange_l = 0;


        auto data = ZControl::g_hx711->getStatistic(count, errors, index);
        for(unsigned i = index + 1; i != index && i != index - 1 && i != (index + Hx711::k_bufferSize - 1); i++)
        {
            i %= ((count < unsigned(Hx711::k_bufferSize)) ? count: unsigned(Hx711::k_bufferSize));
            for(int c = 0; c < 25; c++)
            {
                // signal high
                diff = int64_t(data[i][c * 2 + 1].tv_sec) - int64_t(data[i][c * 2].tv_sec);
                diff *= 1000L * 1000L * 1000L;
                diff += int64_t(data[i][c * 2 + 1].tv_nsec) - int64_t(data[i][c * 2].tv_nsec);
                if(diff < minInterval_h)
                    minInterval_h = diff;
                if(diff > maxInterval_h)
                    maxInterval_h = diff;
                avgInterval_h += diff;
                cntAverange_h++;
                // signal low
                if(c < (25 - 1))
                {
                    diff = int64_t(data[i][c * 2 + 2].tv_sec) - int64_t(data[i][c * 2 + 1].tv_sec);
                    diff *= 1000L * 1000L * 1000L;
                    diff += int64_t(data[i][c * 2 + 2].tv_nsec) - int64_t(data[i][c * 2 + 1].tv_nsec);
                    if(diff < minInterval_l)
                        minInterval_l = diff;
                    if(diff > maxInterval_l)
                        maxInterval_l = diff;
                    avgInterval_l += diff;
                    cntAverange_l++;
                }
            }
        }
        avgInterval_h /= cntAverange_h;
        avgInterval_l /= cntAverange_l;
        ui->lbl_dbg_cntHx711->setText(QString("%1").arg(count));
        ui->lbl_dbg_errHx711->setText(QString("%1").arg(errors));
        ui->lbl_dbg_avgHx711_h->setText(QString("%1 nsec").arg(avgInterval_h));
        ui->lbl_dbg_minHx711_h->setText(QString("%1 nsec").arg(minInterval_h));
        ui->lbl_dbg_maxHx711_h->setText(QString("%1 nsec").arg(maxInterval_h));
        ui->lbl_dbg_avgHx711_l->setText(QString("%1 nsec").arg(avgInterval_l));
        ui->lbl_dbg_minHx711_l->setText(QString("%1 nsec").arg(minInterval_l));
        ui->lbl_dbg_maxHx711_l->setText(QString("%1 nsec").arg(maxInterval_l));
    }
    if(ZControl::g_stepper)
    {
        double curr = ZControl::g_stepper->getPosition();
        ui->lbl_dbg_posStepper->setText(QString("%1").arg(curr));
        ui->dial_dbg_Stepper->setValue(int(curr * 10.));
    }
}
