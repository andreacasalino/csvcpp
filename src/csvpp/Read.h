#pragma once

#include <csvpp/LineProcessor.h>

namespace csvpp {
void read(const std::string &fileName, LineProcessor &proc, char separator);

void read(const std::string &fileName, LineProcessor &proc,
          const std::string &separator = ",");
} // namespace csvpp
