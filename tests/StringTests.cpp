#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <csvpp/String.h>

#include <sstream>

namespace csvpp::tests {
std::vector<std::string> convert(const Line &line) {
  std::vector<std::string> res;
  for (const auto &w : line) {
    res.emplace_back(w.data(), 0, w.size());
  }
  return res;
}

TEST_CASE("split easy", "[string]") {
  SECTION("many slices") {
    std::string line = "aa,bbb,cc,dd";
    auto splitted = split(',', true, line);
    CHECK(convert(splitted) ==
          std::vector<std::string>{"aa", "bbb", "cc", "dd"});
  }

  SECTION("one single slice") {
    std::string line = "aa";
    auto splitted = split(',', true, line);
    CHECK(convert(splitted) == std::vector<std::string>{"aa"});
  }
}

TEST_CASE("split with indivisable", "[string]") {
  std::string line = R"(aa,'bbb, bb',cc,dd)";
  auto splitted = split(',', false, line);
  CHECK(convert(splitted) ==
        std::vector<std::string>{"aa", R"(bbb, bb)", "cc", "dd"});
}

TEST_CASE("join", "[string]") {
  std::ostringstream buff;

  SECTION("many slices") {
    SECTION("only strings") {
      join(buff, '-', "aa", "bbb", "cc");
      CHECK(buff.str() == "aa-bbb-cc");
    }

    SECTION("many types") {
      join(buff, '-', 0, "bbb", 2.5f);
      CHECK(buff.str() == "0-bbb-2.5");
    }
  }

  SECTION("one single slice") {
    join(buff, '-', "aa");
    CHECK(buff.str() == "aa");
  }
}
} // namespace csvpp::tests
