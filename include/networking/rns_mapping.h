#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include <mutex>

std::pair<std::string, std::string> get_config_and_hash(const std::string &identifier);
