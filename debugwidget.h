#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QWidget>

namespace Ui {
class DebugWidget;
}

class DebugWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DebugWidget(QWidget *parent = nullptr);
    ~DebugWidget();

    void paintEvent(QPaintEvent *event);
public slots:
    void quit();
private:
    void updateHx711();

private:
    Ui::DebugWidget *ui;
    QTimer* m_timer;
};

#endif // DEBUGWIDGET_H
