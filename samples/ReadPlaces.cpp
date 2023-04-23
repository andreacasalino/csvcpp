#include <csvpp/Read.h>
#include <csvpp/TypedProcessor.h>

#include <iostream>
#include <unordered_map>

/////////////////////////////////////////////////////////////////////////////////////////////////
// aim of the sample is to read a csv containg a list of cities, together with
// their coordinates
//
// The coordinates are parsed into a custom data. For this purpose a custom
// Converter must be defined
/////////////////////////////////////////////////////////////////////////////////////////////////

// example 51°30'11.0 N 0°01'11.9 W
struct Coordinate {
  struct Angle {
    std::uint8_t angle;
    std::uint8_t minute;
    float seconds;
  };

  struct Latitude {
    Angle angle;
    bool NordSud;
  };

  struct Longitude {
    Angle angle;
    bool EastWest;
  };

  Latitude latitude;
  Longitude longitude;
};

std::ostream &operator<<(std::ostream &stream, const Coordinate::Angle &a) {
  return stream << std::to_string(a.angle) << " o " << std::to_string(a.minute)
                << '\'' << std::to_string(a.seconds) << "\"";
}

std::ostream &operator<<(std::ostream &stream, const Coordinate &c) {
  stream << c.latitude.angle;
  if (c.latitude.NordSud) {
    stream << 'N';
  } else {
    stream << 'S';
  }
  stream << ' ';
  stream << c.longitude.angle;
  if (c.longitude.EastWest) {
    stream << 'E';
  } else {
    stream << 'W';
  }
  return stream;
}

namespace csvpp {
template <> struct Converter<Coordinate> {
  static Coordinate convert(const std::string_view &subject) {
    Coordinate res;
    std::size_t sep = subject.find(' ');
    res.latitude.angle = makeAngle(subject.substr(0, sep));
    res.latitude.NordSud = subject[sep + 1] == 'N';
    std::size_t secondBegin = sep + 3;
    sep = subject.find(' ', secondBegin);
    res.longitude.angle =
        makeAngle(subject.substr(secondBegin, sep - secondBegin));
    res.longitude.EastWest = subject[sep + 1] == 'E';
    return res;
  }

  static Coordinate::Angle makeAngle(const std::string_view &subject) {
    Coordinate::Angle res;
    std::size_t first_sep = subject.find('G');
    std::size_t second_sep = subject.find('\'');

    int angle = Converter<int>::convert(subject.substr(0, first_sep));
    res.angle = static_cast<std::uint8_t>(angle);

    int minute = Converter<int>::convert(
        subject.substr(first_sep + 1, second_sep - first_sep - 1));
    res.minute = static_cast<std::uint8_t>(minute);

    res.seconds = Converter<float>::convert(
        subject.substr(second_sep + 1, subject.size() - second_sep - 1));

    return res;
  }
};
} // namespace csvpp

int main() {
  const auto csvPath = std::string{ASSETS_FOLDER} + "/places.csv";

  // assumes by defult ',' as soparator and '"' as delimeter
  csvpp::Reader reader;

  std::unordered_map<std::string, Coordinate> places;

  csvpp::TypedProcessor<std::string, Coordinate>::process(
      reader, csvPath,
      [&places](std::string city, Coordinate coor) {
        places.emplace(city, coor);
      },
      "Cities", "Coordinates");

  // print the parsed map
  std::cout << "This is the list of cities red from the csv: " << std::endl;
  for (const auto &[city, coordinate] : places) {
    std::cout << "City: " << city << "  coordinates: " << coordinate
              << std::endl;
  }

  return EXIT_SUCCESS;
}
