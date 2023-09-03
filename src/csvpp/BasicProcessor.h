#pragma once

#include <csvpp/LineProcessor.h>

#include <algorithm>

namespace csvpp {
class BasicProcessor : public LineProcessor {
public:
  template <typename... Columns>
  BasicProcessor(Columns &&...columns)
      : LineProcessor{std::forward<Columns>(columns)...} {}

  struct FromIterableTag {};
  template <typename ColumnIter>
  BasicProcessor(FromIterableTag tag, ColumnIter begin, ColumnIter end) {
    std::for_each(begin, end, [this](const auto &field) {
      this->addColumn(std::string{field});
    });
  }

  BasicProcessor &addColumn(const std::string &column) {
    this->LineProcessor::addColumn(column);
    return *this;
  }
};
} // namespace csvpp
