#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <csvpp/Write.h>

namespace csvpp::tests {
TEST_CASE("writeto csv", "[write]") {
  Writer<int, std::string, float> writer("temp.csv", "id", "name", "value");
  writer.add(32523, "Mario Rossi", 30.f);
  writer.add(12412, "Adelaide Verdi", -30.f);
  writer.add(12412, "PierErnesto Longhi", 5.7f);

  // TODO use reader and check everything was persisted into a file
}
} // namespace csvpp::tests
