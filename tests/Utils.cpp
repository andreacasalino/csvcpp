#include "Utils.h"

namespace csvpp::tests {
std::string TestData::getBase() { return std::string{TEST_FOLDER} + "/data"; }

std::string TestData::getFile(const std::string &fileName) {
  return TestData::getBase() + "/" + fileName;
}

void CsvMatrixProcessor::process(const Line &row) {
  auto &added = values.emplace_back();
  for (const auto &val : row) {
    added.emplace_back(std::string{val});
  }
}
} // namespace csvpp::tests
