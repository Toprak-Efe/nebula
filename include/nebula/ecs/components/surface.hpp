#pragma once 

#include <functional>

namespace nebula::data {

struct Surface {
    std::function<void(void)> render;
}; // struct Surface

} // namespace nebula::data

