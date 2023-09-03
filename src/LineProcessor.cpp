#include <csvpp/LineProcessor.h>

namespace csvpp {
void LineProcessor::addColumn(const std::string &column) {
  if (!fields_.has_value()) {
    fields_.emplace();
  }
  fields_->emplace_back(column);
}
} // namespace csvpp
