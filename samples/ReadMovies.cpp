#include <csvpp/Read.h>
#include <csvpp/TypedProcessor.h>

#include <iostream>
#include <map>

/////////////////////////////////////////////////////////////////////////////////////////////////
// aim of the sample is to read a csv containg a list of movies, parse them and
// sort by score
/////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  const auto csvPath = std::string{ASSETS_FOLDER} + "/deniro.csv";

  // assumes by defult ',' as soparator and '"' as delimeter
  csvpp::Reader reader;

  std::multimap<int, std::pair<int, std::string>>
      movies; // <score, <year, title>>

  // fields in the csv are; Year <int>, Score <int>, Title <string>
  csvpp::TypedProcessor<int, int, std::string>::process(
      reader, csvPath,
      [&movies](int year, int score, std::string title) {
        movies.emplace(score, std::make_pair(year, title));
      },
      "Year", "Score", "Title");

  // print the parsed map
  std::cout << "This is what was red from the csv: " << std::endl;
  for (const auto &[score, data] : movies) {
    const auto &[year, title] = data;
    std::cout << "score: " << score << " title: \"" << title
              << "\" year: " << year << std::endl;
  }

  return EXIT_SUCCESS;
}
