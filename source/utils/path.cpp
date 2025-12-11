#include <nebula/utils/path.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <limits.h>

std::filesystem::path nebula::get_executable_path() {
    std::vector<char> pathBuf(PATH_MAX);
    ssize_t count = readlink("/proc/self/exe", pathBuf.data(), pathBuf.size());
    if (count == -1) throw std::runtime_error("Could not determine executable path.");
    return std::filesystem::path(std::string(pathBuf.data(), count));
}

std::filesystem::path nebula::get_asset_dir() {
    auto exe_path = get_executable_path();
    auto root = exe_path.parent_path();
    return root / "assets";
}
