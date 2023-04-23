#include <csvpp/Write.h>

#include <csvpp/Read.h>
#include <csvpp/TypedProcessor.h>

#include <unordered_map>

int main() {
  {
    std::string destination = "some/path/foo.csv";
    csvpp::Writer<std::string, std::string, int, float> writer{
        // destination file
        destination,
        // headers
        "name", "surname", "age", "height"};

    // write the file row by row
    writer.add("Paolo", "Rossi", 24, 180.5f);
    writer.add("Luigi", "Neri", 32, 176.8f);
    // and so on
  }

  {
    std::string location = "some/path/foo.csv";
    csvpp::Reader
        reader; // default separator is assuemd to be ',' but you can change it

    std::unordered_map<std::string, std::pair<int, float>>
        people; //<name-surname, <age, height>>

    csvpp::TypedProcessor<std::string, std::string, int, float>::process(
        reader,
        // source file
        location,
        // process action, called for each line
        [&people](std::string name, std::string surname, int age,
                  float height) {
          people.emplace(name + "-" + surname, std::make_pair(age, height));
        },
        // headers
        "name", "surname", "age", "height");
  }

  return EXIT_SUCCESS;
}
