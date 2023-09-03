#pragma once

#include <csvpp/String.h>

#include <fstream>
#include <sstream>
#include <type_traits>

namespace csvpp {
template <typename... Ts> class Writer {
public:
  template <typename... Headers>
  Writer(const std::string &fileName, Headers &&...fields) : stream_(fileName) {
    static_assert(sizeof...(Headers) == sizeof...(Ts),
                  "number of fields should be equal to number of types");
    if (!stream_.is_open()) {
      std::stringstream err;
      err << fileName << " is an invalid path" << std::endl;
      throw std::runtime_error{err.str()};
    }
    join(stream_, ',', std::forward<Headers>(fields)...);
  }

  void add(const Ts &...values) {
    stream_ << '\n';
    join(stream_, ',', values...);
  }

protected:
  std::ofstream stream_;
};
} // namespace csvpp
