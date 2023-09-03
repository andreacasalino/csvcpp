#include <csvpp/String.h>

#include <sstream>

namespace csvpp {
namespace {
struct IndivisableString {
  static IndivisableString next(char stringDelimiter, const std::string &line,
                                std::size_t pos) {
    auto begin = line.find(stringDelimiter, pos);
    if (begin == std::string::npos) {
      return IndivisableString{};
    }
    auto end = line.find(stringDelimiter, begin + 1);
    if (end == std::string::npos) {
      std::stringstream msg;
      msg << line << " contains unbalanced string delimiter "
          << stringDelimiter;
      throw std::runtime_error{msg.str()};
    }
    return IndivisableString{begin, end};
  }

  operator bool() const { return end != 0; }

  bool isInside(std::size_t pos) const {
    return (pos != std::string::npos) && (begin <= pos) && (pos < end);
  }

  std::size_t begin = 0;
  std::size_t end = 0;
};

std::string_view extract(std::string &line, char stringDelimiterKind,
                         std::size_t begin, std::size_t end) {
  std::string_view res{line.data() + begin, end - begin};
  // strip
  while (!res.empty()) {
    if (res.front() == ' ') {
      res.remove_prefix(1);
      continue;
    }
    break;
  }
  while (!res.empty()) {
    if ((res.back() == ' ') || (res.back() == '\n')) {
      res.remove_suffix(1);
      continue;
    }
    break;
  }
  if (res.front() == stringDelimiterKind) {
    res.remove_prefix(1);
  }
  if (res.back() == stringDelimiterKind) {
    res.remove_suffix(1);
  }
  return res;
}
} // namespace

Line split(char separator, bool stringDelimiterKind, std::string &line) {
  char stringDelimiter = stringDelimiterKind ? '"' : '\'';
  Line res;
  auto indivisable = IndivisableString::next(stringDelimiter, line, 0);
  std::size_t cursor = 0;
  while (cursor < line.size()) {
    auto next = line.find(separator, cursor);
    if (indivisable) {
      while (indivisable.isInside(next)) {
        next = line.find(separator, next + 1);
      }
    }

    if (next == std::string::npos) {
      res.emplace_back(extract(line, stringDelimiter, cursor, line.size()));
      break;
    }
    res.emplace_back(extract(line, stringDelimiter, cursor, next));

    cursor = next + 1;
    if (indivisable && (cursor >= indivisable.end)) {
      indivisable = IndivisableString::next(stringDelimiter, line, cursor);
    }
  }
  return res;
}
} // namespace csvpp
