#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace csvpp {
using Line = std::vector<std::string_view>;

Line split(char separator, bool stringDelimiterKind, std::string &line);

namespace detail {
template <typename First, typename... Args>
void join_(std::ostream &recipient, char, First &&last) {
  recipient << last;
}

template <typename First, typename... Args>
void join_(std::ostream &recipient, char separator, First &&first,
           Args &&...args) {
  recipient << first << separator;
  join_(recipient, separator, std::forward<Args>(args)...);
}
} // namespace detail

template <typename... Args>
void join(std::ostream &recipient, char separator, Args &&...args) {
  detail::join_(recipient, separator, std::forward<Args>(args)...);
}
} // namespace csvpp
