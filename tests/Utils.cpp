#include "Utils.h"

namespace csvpp::tests {
std::filesystem::path TestData::getBase() {
  return std::filesystem::path{TEST_FOLDER} / "data";
}

std::filesystem::path TestData::getFile(const std::string &fileName) {
  return TestData::getBase() / fileName;
}

void CsvMatrixProcessor::process(const Line &row) {
  auto &added = values.emplace_back();
  for (const auto &val : row) {
    added.emplace_back(std::string{val});
  }
}
} // namespace csvpp::tests
