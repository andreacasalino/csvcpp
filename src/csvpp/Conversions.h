#pragma once

#include <string>
#include <string_view>

namespace csvpp {
template <typename T> struct Converter {};

template <> struct Converter<std::string> {
  static std::string convert(const std::string_view &subject) {
    return std::string{subject};
  }
};

template <> struct Converter<int> {
  static int convert(const std::string_view &subject) {
    return std::atoi(subject.data()); // TODO improve this
  }
};

template <> struct Converter<double> {
  static double convert(const std::string_view &subject) {
    return std::atof(subject.data()); // TODO improve this
  }
};

template <> struct Converter<float> {
  static float convert(const std::string_view &subject) {
    return static_cast<float>(std::atof(subject.data())); // TODO improve this
  }
};

// TODO other converters

} // namespace csvpp
