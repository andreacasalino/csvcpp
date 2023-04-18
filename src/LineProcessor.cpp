#include <csvpp/LineProcessor.h>

namespace csvpp {
LineProcessor::LineProcessor(const std::string &lastField) {
  addColumn(lastField);
}
} // namespace csvpp
