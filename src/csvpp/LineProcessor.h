#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace csvpp {
using Line = std::vector<std::string_view>;

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

  LineProcessor(const std::string &lastField);

  template <typename... Columns>
  LineProcessor(const std::string &firstField, Columns &&...otherColumns)
      : LineProcessor{std::forward<Columns>(otherColumns)...} {
    addColumn<false>(firstField);
  }

  template <bool BackFront = true> void addColumn(const std::string &column) {
    if (!fields_.has_value()) {
      fields_.emplace();
    }
    if constexpr (BackFront) {
      fields_->emplace_back(column);
    } else {
      fields_->insert(fields_->begin(), column);
    }
  }

  // if empty, means take ALL columns from a line in the order they appear in
  // the csv
  std::optional<std::vector<std::string>> fields_;
};
} // namespace csvpp
