#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace inspur
{

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

enum class bios_version_devname
{
    BIOS = 0,
    ME = 1,
    IE = 2,
    PCH = 3,
    BOARD = 4,
    MRC = 5,
    CUSTOM_ID = 6,
    PCH_STEPPING = 7,
};

constexpr std::array<std::string_view, 8> bios_devname{
    "BIOS", "ME", "IE", "PCH", "BOARD", "MRC", "CUSTOM_ID", "PCH_STEPPING",
};

} // namespace inspur
