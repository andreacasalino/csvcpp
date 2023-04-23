#pragma once

#include <csvpp/LineProcessor.h>

namespace csvpp {
class Reader {
public:
  Reader() = default;

  void setSeparator(char sep) { separator_ = sep; }

  // true  uses "
  // false uses '
  void stringDelimiter(char delim) { stringDelimiterKind_ = delim; }

  void read(const std::string &fileName, LineProcessor &proc) const;

private:
  char separator_ = ',';

  bool stringDelimiterKind_ = true;
};
} // namespace csvpp
