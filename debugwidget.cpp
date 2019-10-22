#include "debugwidget.h"
#include "ui_debugwidget.h"
#include "zcontrol.h"
#include <QTimer>


DebugWidget::DebugWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugWidget)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(500);
    m_timer->start();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(ui->pbQuit, SIGNAL(pressed()), this, SLOT(quit()));
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
        for(unsigned i = index + 1; i != index - 1 && i != (index + Hx711::k_bufferSize - 1); i++)
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
}