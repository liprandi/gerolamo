#ifndef PIINFO_H
#define PIINFO_H

#include <bcm_host.h>
#include <string>
#include <vector>

class PiInfo
{
public:
    PiInfo();
    ~PiInfo();

    inline bool isPi4() const {return m_bcm_host_model_pi4;}
    const std::string& model() const {return m_models[unsigned(m_bcm_host_model_type)];}

private:
    unsigned m_bcm_peripheral_address;
    unsigned m_bcm_peripheral_size;
    unsigned m_bcm_sdram_address;
    int m_bcm_host_model_type;
    bool m_bcm_host_model_pi4;
    bool m_bcm_host_fkms_active;
    bool m_bcm_host_kms_active;
public:
    static const std::vector<std::string> m_models;
};

#endif // GPIOUTIL_H
