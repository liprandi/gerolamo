#include "hx711.h"
#include <QDebug>

hx711::hx711()
{

}

// General purpose error message
// A real system would probably have a better error handling method...
void hx711::panic(const std::string& msg)
{
    qDebug() << msg.data();
    exit(1);
}
