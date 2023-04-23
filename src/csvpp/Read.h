#pragma once

#include <csvpp/LineProcessor.h>

namespace csvpp {
class Reader {
public:
  Reader() = default;

  void setSeparator(char sep);

  // true  uses "
  // false uses '
  void stringDelimiter(bool kind) { stringDelimiterKind_ = kind; };

  void read(const std::string &fileName, LineProcessor &proc) const;

private:
  char separator_ = ',';

  bool stringDelimiterKind_ = true;
};
} // namespace csvpp
