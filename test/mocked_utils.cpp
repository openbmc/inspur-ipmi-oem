#include "mocked_utils.hpp"

namespace utils
{

static std::unique_ptr<MockedUtils> utils;
const UtilsInterface& getUtils()
{
    if (!utils)
    {
        utils = std::make_unique<MockedUtils>();
    }
    return *utils;
}

void freeUtils()
{
    utils.reset();
}

} // namespace utils
