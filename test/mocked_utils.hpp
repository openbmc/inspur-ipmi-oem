#pragma once

#include "utils.hpp"

#include <gmock/gmock.h>

namespace utils
{

class MockedUtils : public UtilsInterface
{
  public:
    virtual ~MockedUtils() = default;

    MOCK_CONST_METHOD3(getService,
                       std::string(sdbusplus::bus::bus& bus, const char* path,
                                   const char* interface));

    MOCK_CONST_METHOD3(getServices,
                       std::vector<std::string>(sdbusplus::bus::bus& bus,
                                                const char* path,
                                                const char* interface));

    MOCK_CONST_METHOD5(getPropertyImpl,
                       any(sdbusplus::bus::bus& bus, const char* service,
                           const char* path, const char* interface,
                           const char* propertyName));

    MOCK_CONST_METHOD6(setPropertyImpl,
                       void(sdbusplus::bus::bus& bus, const char* service,
                            const char* path, const char* interface,
                            const char* propertyName, ValueType&& value));
};

const UtilsInterface& getUtils();

void freeUtils();

} // namespace utils
