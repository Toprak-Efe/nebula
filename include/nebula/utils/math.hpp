#pragma once

#include <array>

namespace nebula::data {

template<std::size_t T>
using Vec = std::array<float, T>;

using Vec3 = Vec<3>;
using Vec4 = Vec<4>;

template<std::size_t T1, std::size_t T2>
using Mat = std::array<Vec<T2>, T1>;

using Mat3 = Mat<3,3>;
using Mat4 = Mat<4,4>;

} // namespace nebula::data
