#include "sdbus_wrapper.hpp"

#include <ipmid/api.h>

sdbusplus::bus::bus& getBus()
{
    static sdbusplus::bus::bus bus(ipmid_get_sd_bus_connection());
    return bus;
}
