#pragma once

#include <filesystem>
#include <string>

#include <csvpp/BasicProcessor.h>

namespace csvpp::tests {
struct TestData {
  static std::filesystem::path getBase();

  static std::filesystem::path getFile(const std::string &fileName);
};

class CsvMatrixProcessor : public BasicProcessor {
public:
  using BasicProcessor::BasicProcessor;

  std::vector<std::vector<std::string>> values;

protected:
  void process(const Line &row) override;
};
} // namespace csvpp::tests
