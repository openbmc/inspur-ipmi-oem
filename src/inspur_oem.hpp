#pragma once
#include <cstdint>

enum ipmi_inspur_net_fns
{
    NETFN_OEM_INSPUR = 0x3c,
};

enum inspur_oem_cmds
{
    CMD_OEM_ASSET_INFO = 0x01,
};

struct AssetInfoHeader
{
    uint8_t rwFlag;
    uint8_t deviceType;
    uint8_t infoType;
    uint8_t maskAllPresentLen;
    uint8_t enableStatus;
    uint8_t maskPresent;
    uint8_t maskAllPresent;
    uint8_t allInfoDone;
    uint16_t totalMessageLen;
} __attribute__((packed));
