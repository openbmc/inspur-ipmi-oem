#include "config.h"

#include "inspur_oem.hpp"

#include "sdbus_wrapper.hpp"
#include "utils.hpp"

#include <ipmid/api.h>

#include <phosphor-logging/log.hpp>

#include <optional>

namespace
{
constexpr auto FIRMWARE_TYPE_OFFSET = 0;
constexpr auto FIRMWARE_TYPE_SIZE = 1;
constexpr auto FIRMWARE_VERSION_OFFSET =
    FIRMWARE_TYPE_OFFSET + FIRMWARE_TYPE_SIZE;
constexpr auto FIRMWARE_VERSION_SIZE = 15;
constexpr auto FIRMWARE_BUILDTIME_OFFSET =
    FIRMWARE_VERSION_OFFSET + FIRMWARE_VERSION_SIZE;
constexpr auto FIRMWARE_BUILDTIME_SIZE = 20;
constexpr auto FIRMWARE_MIN_SIZE = FIRMWARE_BUILDTIME_OFFSET;

static_assert(FIRMWARE_VERSION_OFFSET == 1);
static_assert(FIRMWARE_BUILDTIME_OFFSET == 16);
} // namespace

namespace ipmi
{

#define UNUSED(x) (void)(x)

using namespace phosphor::logging;
using namespace inspur;

static void registerOEMFunctions() __attribute__((constructor));
static auto& bus = getBus();

struct ParsedAssetInfo
{
    uint8_t rwFlag;
    uint8_t deviceType;
    uint8_t infoType;
    uint8_t maskAllPresentLen;
    std::vector<uint8_t> enableStatus;
    std::vector<uint8_t> maskPresent;
    std::vector<uint8_t> maskAllPresent;
    uint8_t allInfoDone;
    uint16_t totalMessageLen;
    std::vector<uint8_t> data;
};

void dumpAssetInfo(const ParsedAssetInfo& info)
{
    fprintf(stderr,
            "AssetInfo: rw %d, deviceType 0x%02x, infoType 0x%02x, "
            "maskAllPresentLen %u\n",
            info.rwFlag, info.deviceType, info.infoType,
            info.maskAllPresentLen);
    fprintf(stderr, "enableStatus ");
    for (const auto& d : info.enableStatus)
    {
        fprintf(stderr, "0x%02x ", d);
    }
    fprintf(stderr, "\nmaskPresent ");
    for (const auto& d : info.maskPresent)
    {
        fprintf(stderr, "0x%02x ", d);
    }
    fprintf(stderr, "\nmaskAllPresent ");
    for (const auto& d : info.maskAllPresent)
    {
        fprintf(stderr, "0x%02x ", d);
    }
    fprintf(stderr, "\nallInfoDone %d, totalMessageLen %u\n", info.allInfoDone,
            info.totalMessageLen);
    fprintf(stderr, "data: ");
    for (const auto& d : info.data)
    {
        fprintf(stderr, "0x%02x ", d);
    }
    fprintf(stderr, "\n");
}

std::optional<ParsedAssetInfo> parseAssetInfo(const AssetInfoHeader* h)
{
    auto len = h->maskAllPresentLen;
    if (len == 0)
    {
        // This is invalid
        return {};
    }

    ParsedAssetInfo info;
    info.rwFlag = h->rwFlag;
    info.deviceType = h->deviceType;
    info.infoType = h->infoType;
    info.maskAllPresentLen = len;
    info.enableStatus.resize(len);
    info.maskPresent.resize(len);
    info.maskAllPresent.resize(len);
    const uint8_t* p = &h->enableStatus;
    memcpy(info.enableStatus.data(), p, len);
    p += len;
    memcpy(info.maskPresent.data(), p, len);
    p += len;
    memcpy(info.maskAllPresent.data(), p, len);
    p += len;
    info.allInfoDone = *p++;
    info.totalMessageLen = *reinterpret_cast<const uint16_t*>(p);
    p += sizeof(uint16_t);
    auto dataLen = info.totalMessageLen - (sizeof(AssetInfoHeader) + 3 * len);
    info.data.resize(dataLen);
    memcpy(info.data.data(), p, dataLen);

    //    dumpAssetInfo(info);
    return info;
}

void parseBIOSInfo(const std::vector<uint8_t>& data)
{
    if (data.size() < FIRMWARE_MIN_SIZE)
    {
        return;
    }
    bios_version_devname dev = static_cast<bios_version_devname>(data[0]);
    std::string version{data.data() + FIRMWARE_VERSION_OFFSET,
                        data.data() + FIRMWARE_VERSION_SIZE};
    std::string buildTime;
    if (dev == bios_version_devname::BIOS)
    {
        buildTime.assign(reinterpret_cast<const char*>(
                             data.data() + FIRMWARE_BUILDTIME_OFFSET),
                         FIRMWARE_BUILDTIME_SIZE);

        // Set BIOS version
        auto service = utils::getService(bus, BIOS_OBJPATH, VERSION_IFACE);
        utils::setProperty(bus, service.c_str(), BIOS_OBJPATH, VERSION_IFACE,
                           VERSION, version);
    }

    printf("Dev %s, version %s, build time %s\n",
           bios_devname[static_cast<int>(dev)].data(), version.c_str(),
           buildTime.c_str());
}

ipmi_ret_t ipmiOemInspurAssetInfo(ipmi_netfn_t /* netfn */,
                                  ipmi_cmd_t /* cmd */, ipmi_request_t request,
                                  ipmi_response_t response,
                                  ipmi_data_len_t /* data_len */,
                                  ipmi_context_t /* context */)
{
    auto header = reinterpret_cast<AssetInfoHeader*>(request);
    uint8_t* res = reinterpret_cast<uint8_t*>(response);
    UNUSED(res);

    auto info = parseAssetInfo(header);
    auto deviceType = info->deviceType;
    if (deviceType != 0x05)
    {
        log<level::WARNING>("Device type not supported yet");
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    // For now we only support BIOS type
    parseBIOSInfo(info->data);

    return IPMI_CC_OK;
}

void registerOEMFunctions(void)
{
    ipmi_register_callback(NETFN_OEM_INSPUR, CMD_OEM_ASSET_INFO, nullptr,
                           ipmiOemInspurAssetInfo, SYSTEM_INTERFACE);
}

} // namespace ipmi
