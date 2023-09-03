#pragma once

#include <csvpp/String.h>
#include <optional>

namespace csvpp {

class LineProcessor {
public:
  virtual ~LineProcessor() = default;

  virtual void process(const Line &row) = 0;

  // Portion of the fields that has to be processed.
  // null when all the fields are expected to be processed.
  const auto *expectedFields() const {
    return fields_.has_value() ? &fields_.value() : nullptr;
  }

protected:
  LineProcessor() = default;

  template <typename... Columns> LineProcessor(Columns &&...columns) {
    (addColumn(std::forward<Columns>(columns)), ...);
  }

  void addColumn(const std::string &column);

  // if empty, means take ALL columns from a line in the order they appear in
  // the csv
  std::optional<std::vector<std::string>> fields_;
};
} // namespace csvpp
