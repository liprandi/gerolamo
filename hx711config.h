#ifndef HX711CONFIG_H
#define HX711CONFIG_H

#include <QWidget>

namespace Ui {
class Hx711Config;
}

class Hx711Config : public QWidget
{
    Q_OBJECT

public:
    explicit Hx711Config(QWidget *parent = nullptr);
    ~Hx711Config();
public slots:
    void gpioclkChanged(const QString& text);
    void gpiodata1Changed(const QString& text);
    void gpiodata2Changed(const QString& text);
    void gpiodata3Changed(const QString& text);
    void offsetGainChanged();

private:
    Ui::Hx711Config *ui;
};

#endif // HX711CONFIG_H
