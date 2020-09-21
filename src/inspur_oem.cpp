#include "inspur_oem.hpp"

#include <ipmid/api.h>

namespace ipmi
{

static void registerOEMFunctions() __attribute__((constructor));

ipmi_ret_t ipmiOemInspurAssetInfo(ipmi_netfn_t netfn, ipmi_cmd_t cmd,
                                  ipmi_request_t request,
                                  ipmi_response_t response,
                                  ipmi_data_len_t data_len,
                                  ipmi_context_t context)
{
    // TODO: handle the oem command
    fprintf(stderr,
            "netfn 0x%02x, cmd 0x%02x, request %p, response %p, data_len %p, "
            "context %p\n",
            netfn, cmd, request, response, static_cast<void*>(data_len),
            context);
    return {};
}

void registerOEMFunctions(void)
{
    ipmi_register_callback(NETFN_OEM_INSPUR, CMD_OEM_ASSET_INFO, nullptr,
                           ipmiOemInspurAssetInfo, SYSTEM_INTERFACE);
}

} // namespace ipmi
