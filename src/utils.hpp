#pragma once

#include <sdbusplus/bus.hpp>

#include <experimental/any>
#include <string>
#include <vector>

namespace utils
{

class UtilsInterface;

// Due to a libstdc++ bug, we got compile error using std::any with gmock.
// A temporary workaround is to use std::experimental::any.
// See details in https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90415
using std::experimental::any;
using std::experimental::any_cast;

using ValueType =
    std::variant<bool, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t,
                 uint64_t, double, std::string>;
/**
 * @brief Get the implementation of UtilsInterface
 */
const UtilsInterface& getUtils();

/** @brief Get service name from object path and interface
 *
 * @param[in] bus          - The Dbus bus object
 * @param[in] path         - The Dbus object path
 * @param[in] interface    - The Dbus interface
 *
 * @return The name of the service
 */
std::string getService(sdbusplus::bus::bus& bus, const char* path,
                       const char* interface);

/** @brief Get all the service names from object path and interface
 *
 * @param[in] bus          - The Dbus bus object
 * @param[in] path         - The Dbus object path
 * @param[in] interface    - The Dbus interface
 *
 * @return The name of the services
 */
std::vector<std::string> getServices(sdbusplus::bus::bus& bus, const char* path,
                                     const char* interface);

/** @brief The template function to get property from the requested dbus path
 *
 * @param[in] bus          - The Dbus bus object
 * @param[in] service      - The Dbus service name
 * @param[in] path         - The Dbus object path
 * @param[in] interface    - The Dbus interface
 * @param[in] propertyName - The property name to get
 *
 * @return The value of the property
 */
template <typename T>
T getProperty(sdbusplus::bus::bus& bus, const char* service, const char* path,
              const char* interface, const char* propertyName);

template <typename T>
void setProperty(sdbusplus::bus::bus& bus, const char* service,
                 const char* path, const char* interface,
                 const char* propertyName, const T& value);

/**
 * @brief The interface for utils
 */
class UtilsInterface
{
  public:
    // For now the code needs to get property for Present and Version
    using PropertyType = std::variant<std::string, bool>;

    virtual ~UtilsInterface() = default;

    virtual std::string getService(sdbusplus::bus::bus& bus, const char* path,
                                   const char* interface) const = 0;

    virtual std::vector<std::string>
        getServices(sdbusplus::bus::bus& bus, const char* path,
                    const char* interface) const = 0;

    virtual any getPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                                const char* path, const char* interface,
                                const char* propertyName) const = 0;

    virtual void setPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                                 const char* path, const char* interface,
                                 const char* propertyName,
                                 ValueType&& value) const = 0;

    template <typename T>
    T getProperty(sdbusplus::bus::bus& bus, const char* service,
                  const char* path, const char* interface,
                  const char* propertyName) const
    {
        any result =
            getPropertyImpl(bus, service, path, interface, propertyName);
        auto value = any_cast<PropertyType>(result);
        return std::get<T>(value);
    }

    template <typename T>
    void setProperty(sdbusplus::bus::bus& bus, const char* service,
                     const char* path, const char* interface,
                     const char* propertyName, const T& value) const
    {
        ValueType v(value);
        setPropertyImpl(bus, service, path, interface, propertyName,
                        std::move(v));
    }
};

class Utils : public UtilsInterface
{
  public:
    std::string getService(sdbusplus::bus::bus& bus, const char* path,
                           const char* interface) const override;

    std::vector<std::string> getServices(sdbusplus::bus::bus& bus,
                                         const char* path,
                                         const char* interface) const override;

    any getPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                        const char* path, const char* interface,
                        const char* propertyName) const override;

    void setPropertyImpl(sdbusplus::bus::bus& bus, const char* service,
                         const char* path, const char* interface,
                         const char* propertyName,
                         ValueType&& value) const override;
};

inline std::string getService(sdbusplus::bus::bus& bus, const char* path,
                              const char* interface)
{
    return getUtils().getService(bus, path, interface);
}

inline std::vector<std::string> getServices(sdbusplus::bus::bus& bus,
                                            const char* path,
                                            const char* interface)
{
    return getUtils().getServices(bus, path, interface);
}

template <typename T>
T getProperty(sdbusplus::bus::bus& bus, const char* service, const char* path,
              const char* interface, const char* propertyName)
{
    return getUtils().getProperty<T>(bus, service, path, interface,
                                     propertyName);
}

template <typename T>
void setProperty(sdbusplus::bus::bus& bus, const char* service,
                 const char* path, const char* interface,
                 const char* propertyName, const T& value)
{
    return getUtils().setProperty<T>(bus, service, path, interface,
                                     propertyName, value);
}

} // namespace utils
