#include "utils.hpp"

#include <phosphor-logging/log.hpp>

#include <algorithm>

using namespace phosphor::logging;

namespace utils
{

namespace // anonymous
{
constexpr auto MAPPER_BUSNAME = "xyz.openbmc_project.ObjectMapper";
constexpr auto MAPPER_PATH = "/xyz/openbmc_project/object_mapper";
constexpr auto MAPPER_INTERFACE = "xyz.openbmc_project.ObjectMapper";
} // namespace

const UtilsInterface& getUtils()
{
    static Utils utils;
    return utils;
}

std::string Utils::getService(sdbusplus::bus::bus& bus, const char* path,
                              const char* interface) const
{
    auto services = getServices(bus, path, interface);
    if (services.empty())
    {
        return {};
    }
    return services[0];
}

std::vector<std::string> Utils::getServices(sdbusplus::bus::bus& bus,
                                            const char* path,
                                            const char* interface) const
{
    auto mapper = bus.new_method_call(MAPPER_BUSNAME, MAPPER_PATH,
                                      MAPPER_INTERFACE, "GetObject");

    mapper.append(path, std::vector<std::string>({interface}));
    try
    {
        auto mapperResponseMsg = bus.call(mapper);

        std::vector<std::pair<std::string, std::vector<std::string>>>
            mapperResponse;
        mapperResponseMsg.read(mapperResponse);
        if (mapperResponse.empty())
        {
            log<level::ERR>("Error reading mapper response");
            throw std::runtime_error("Error reading mapper response");
        }
        std::vector<std::string> ret;
        for (const auto& i : mapperResponse)
        {
            ret.emplace_back(i.first);
        }
        return ret;
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        log<level::ERR>("GetObject call failed", entry("PATH=%s", path),
                        entry("INTERFACE=%s", interface));
        throw std::runtime_error("GetObject call failed");
    }
}

any Utils::getPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                           const char* path, const char* interface,
                           const char* propertyName) const
{
    auto method = bus.new_method_call(service, path,
                                      "org.freedesktop.DBus.Properties", "Get");
    method.append(interface, propertyName);
    try
    {
        PropertyType value{};
        auto reply = bus.call(method);
        reply.read(value);
        return any(value);
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        log<level::ERR>("GetProperty call failed", entry("PATH=%s", path),
                        entry("INTERFACE=%s", interface),
                        entry("PROPERTY=%s", propertyName));
        throw std::runtime_error("GetProperty call failed");
    }
}

void Utils::setPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                            const char* path, const char* interface,
                            const char* propertyName, ValueType&& value) const
{
    auto method = bus.new_method_call(service, path,
                                      "org.freedesktop.DBus.Properties", "Set");
    method.append(interface, propertyName, value);
    try
    {
        bus.call_noreply(method);
    }
    catch (const sdbusplus::exception::SdBusError& ex)
    {
        log<level::ERR>("SetProperty call failed", entry("PATH=%s", path),
                        entry("INTERFACE=%s", interface),
                        entry("PROPERTY=%s", propertyName));
        throw std::runtime_error("SetProperty call failed");
    }
}

} // namespace utils
