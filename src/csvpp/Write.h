#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace csvpp {
template <typename... Ts> class Writer {
public:
  template <typename... Headers>
  Writer(const std::string &fileName, Headers &&...fields) : stream_(fileName) {
    if (!stream_.is_open()) {
      std::stringstream err;
      err << fileName << " is an invalid path" << std::endl;
      throw std::runtime_error{err.str()};
    }
    // TODO static assert fields size is same of Ts
    header(std::forward<Headers>(fields)...);
  }

  void add(const Ts &...values) {
    stream_ << '\n';
    this->add_<Ts...>(values...);
  }

protected:
  template <typename T, typename... Others>
  void add_(const T &front, const Others &...values) {
    stream_ << front << ',';
    this->add_<Others...>(values...);
  }

  template <typename T> void add_(const T &front) { stream_ << front; }

  template <typename... Others>
  void header(const std::string &front, Others &&...others) {
    stream_ << front << ',';
    this->header(std::forward<Others>(others)...);
  }

  void header(const std::string &front) { stream_ << front; }

  std::ofstream stream_;
};
} // namespace csvpp
