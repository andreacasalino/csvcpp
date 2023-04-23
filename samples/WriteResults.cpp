#include <csvpp/Write.h>

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////
// TODO
/////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  const std::string csvDestination = "results.csv";

  {
    csvpp::Writer<std::string, std::string, int, float> writer(
        csvDestination, "Name", "Surname", "age", "weight");

    // write row one by one
    writer.add("Paolo", "Rossi", 34, 81.3f);
    writer.add("Valeria", "Verdi", 25, 55.7f);
    writer.add("mario", "Neri", 21, 75.4f);
  }

  // writer went out of the scope, data was automatically flushed
  std::cout << "Data persisted into: " << csvDestination << std::endl;

  return EXIT_SUCCESS;
}
