#ifndef HX711_H
#define HX711_H

#include <string>

class hx711
{
public:
    hx711();

private:
    [[ noreturn ]] void panic(const std::string& msg);
};

#endif // HX711_H
