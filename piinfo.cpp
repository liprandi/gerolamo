#include "piinfo.h"

const std::vector<std::string> PiInfo::m_models =
{
    "BOARD_TYPE_MODELA",
    "BOARD_TYPE_MODELB",
    "BOARD_TYPE_MODELAPLUS",
    "BOARD_TYPE_MODELBPLUS",
    "BOARD_TYPE_PI2MODELB",
    "BOARD_TYPE_ALPHA",
    "BOARD_TYPE_CM",
    "BOARD_TYPE_CM2",
    "BOARD_TYPE_PI3MODELB",
    "BOARD_TYPE_PI0",
    "BOARD_TYPE_CM3",
    "BOARD_TYPE_CUSTOM",
    "BOARD_TYPE_PI0W",
    "BOARD_TYPE_PI3MODELBPLUS",
    "BOARD_TYPE_PI3MODELAPLUS",
    "BOARD_TYPE_FPGA",
    "BOARD_TYPE_CM3PLUS",
    "BOARD_TYPE_PI4MODELB"
};

PiInfo::PiInfo()
{
    bcm_host_init();
    m_bcm_peripheral_address = bcm_host_get_peripheral_address();
    m_bcm_peripheral_size = bcm_host_get_peripheral_size();
    m_bcm_sdram_address = bcm_host_get_sdram_address();
    m_bcm_host_model_type = bcm_host_get_model_type();
    m_bcm_host_fkms_active = bcm_host_is_fkms_active() != 0;
    m_bcm_host_kms_active = bcm_host_is_kms_active() != 0;
    m_bcm_host_model_pi4 = bcm_host_is_model_pi4() != 0;
}

PiInfo::~PiInfo()
{
    bcm_host_deinit();
}

