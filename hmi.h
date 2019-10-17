#ifndef HMI_H
#define HMI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Hmi; }
QT_END_NAMESPACE

class Hmi : public QMainWindow
{
    Q_OBJECT

public:
    Hmi(QWidget *parent = nullptr);
    ~Hmi();
private:
    void setupGpio();

private:
    Ui::Hmi *ui;
};
#endif // HMI_H
