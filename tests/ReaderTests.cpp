#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <csvpp/Read.h>
#include <csvpp/TypedProcessor.h>

#include "Utils.h"

#include <memory>

namespace csvpp::tests {
TEST_CASE("parse simple file", "[read]") {
  SECTION("ALL fields") {
    CsvMatrixProcessor proc;
    Reader{}.read(TestData::getFile("simple.csv"), proc);

    CHECK(proc.values == std::vector<std::vector<std::string>>{
                             {"foo", "Paolo", "bla"}, {"bla", "Rossi", "foo"}});
  }

  SECTION("subset of fields") {
    std::unique_ptr<CsvMatrixProcessor> proc;

    SECTION("variadic fields list") {
      proc = std::make_unique<CsvMatrixProcessor>("bb", "aa");
    }

    SECTION("fields by iterators") {
      std::vector<std::string> fields = std::vector<std::string>{"bb", "aa"};
      proc = std::make_unique<CsvMatrixProcessor>(
          BasicProcessor::FromIterableTag{}, fields.begin(), fields.end());
    }

    Reader{}.read(TestData::getFile("simple.csv"), *proc);

    CHECK(proc->values == std::vector<std::vector<std::string>>{
                              {"bla", "foo"}, {"foo", "bla"}});
  }
}

TEST_CASE("typed processor", "[read]") {
  using People = std::unordered_map<int, std::pair<std::string, float>>;
  People parsed;

  SECTION("as object") {
    class Processor : public TypedProcessor<std::string, int, float> {
    public:
      using TypedProcessor<std::string, int, float>::TypedProcessor;

      People people;

    protected:
      void process_(std::string name, int id, float value) override {
        people.emplace(id, std::make_pair(name, value));
      }
    };

    Processor proc("name", "id", "value");
    Reader{}.read(TestData::getFile("people.csv"), proc);
    parsed = std::move(proc.people);
  }

  SECTION("as lambda") {
    TypedProcessor<std::string, int, float>::process(
        Reader{}, TestData::getFile("people.csv"),
        [&parsed](std::string name, int id, float value) {
          parsed.emplace(id, std::make_pair(name, value));
        },
        "name", "id", "value");
  }

  static const People expected = People{{2231, {"Paolo Rossi", 23.76f}},
                                        {6237, {"Mario Verdi", -14.9f}},
                                        {3447, {"Emilia Neri", 10.21f}}};

  for (const auto &[key, expected_val] : expected) {
    auto parsed_it = parsed.find(key);
    REQUIRE(parsed_it != parsed.end());
    const auto &parsed_val = parsed_it->second;
    CHECK(expected_val.first == parsed_val.first);
    CHECK(std::abs(expected_val.second - parsed_val.second) < 0.01f);
  }
}

} // namespace csvpp::tests

namespace {
struct Person {
  std::string name;
  std::string surname;
};

bool operator==(const Person &a, const Person &b) {
  return a.name == b.name && a.surname == b.surname;
}
} // namespace

namespace csvpp {
template <> struct Converter<Person> {
  static Person convert(const std::string_view &subject) {
    Person res;
    auto sep_pos = subject.find('-');
    res.name = std::string{subject.data(), 0, sep_pos};
    res.surname =
        std::string{subject.data(), sep_pos + 1, subject.size() - sep_pos - 1};
    return res;
  }
};
} // namespace csvpp

namespace csvpp::tests {

TEST_CASE("typed processor, custom type conversion", "[read]") {
  using Data = std::vector<std::pair<Person, int>>;

  Data data;

  TypedProcessor<Person, int>::process(
      Reader{}, TestData::getFile("custom_type.csv"),
      [&data](Person person, int age) {
        data.emplace_back(std::make_pair(person, age));
      },
      "people", "age");

  Data data_expected = Data{{Person{"Mario", "Rossi"}, 35},
                            {Person{"Franca", "Verdi"}, 29},
                            {Person{"Pippo", "Longhi"}, 53},
                            {Person{"Veronica", "Neri"}, 31}};

  CHECK(data == data_expected);
}

} // namespace csvpp::tests
