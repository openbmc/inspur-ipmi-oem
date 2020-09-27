#include "sdbus_wrapper.hpp"

#include <sdbusplus/test/sdbus_mock.hpp>

#include <memory>

// To support ipmid_get_sd_bus_connection, we have to make the sdbusMock global
static std::unique_ptr<sdbusplus::SdBusMock> sdbusMock;
static std::unique_ptr<sdbusplus::bus::bus> mockedBus;

sdbusplus::bus::bus& getBus()
{
    if (!sdbusMock)
    {
        sdbusMock = std::make_unique<sdbusplus::SdBusMock>();
    }
    mockedBus = std::make_unique<sdbusplus::bus::bus>(
        sdbusplus::get_mocked_new(sdbusMock.get()));
    return *mockedBus.get();
}

void clearMockedBus()
{
    sdbusMock.reset();
    mockedBus.reset();
}
