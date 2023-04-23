# csvpp

![binaries_compilation](https://github.com/andreacasalino/csvcpp/actions/workflows/runTests.yml/badge.svg)

- [What is this library about](#intro)
- [Content](#content)
- [Usage](#usage)
- [CMake support](#cmake-support)

## INTRO

Haven't left a **star** already? Do it now ;)!

**csvpp** is a **C++** library for csv manipulation.
You can use **csvpp** essentially in 2 ways:
- export typed data on a csv file, see [**Write.h**](./src/csvpp/Write.h) and [this](./samples/WriteResults.cpp) sample
- parse a csv file by reading row by row, see [**Read.h**](./src/csvpp/Read.h) and [this](./samples/ReadMovies.cpp) and [this other](./samples/ReadPlaces.cpp) samples

### CONTENT

 * the sources of the library are contained in the [src](./src) folder.

 * example of usage are contained in the [samples](./samples) folder.

## USAGE

Haven't left a **star** already? Do it now ;)!

### WRITE ON A CSV

Suppose you want to export some data ona csv file. Assume each row of the file you want to create contains:
- a **string** with the name of a person
- a **string** with the surname of a person
- an **int** representing his/her age
- a **float** representing his/her height in [cm]
To achieve this with **csvpp**, you would write something like this:

```cpp
#include <csvpp/Write.h>

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
```
after **writer** goes outof the scope, the stream to the file is closed and all rows are flushed into the file.

### READ FROM A CSV

Suppose you want to read the kind csv described in the previous Section.
What you need to do is to define a processing predicate, which is called for each line while iterating the csv.
Suppose you want to read the csv and convert the information into a map of person. What you would write is something like that:
```cpp
#include <csvpp/Read.h>
#include <csvpp/TypedProcessor.h>

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
```

Trivial types are by deafult converted. However, you can also define custom converters for complex ones, refer to  [this](./samples/ReadPlaces.cpp) sample.

## CMAKE SUPPORT
   
Haven't yet left a **star**? Do it now! ;).

To consume this library you can rely on [CMake](https://cmake.org).
More precisely, You can fetch this package and link to the **csvpp** library:
```cmake
include(FetchContent)
FetchContent_Declare(
csvpp
GIT_REPOSITORY https://github.com/andreacasalino/csvcpp
GIT_TAG        main
)
FetchContent_MakeAvailable(csvpp)
```

and then link to the **csvpp** library:
```cmake
target_link_libraries(${TARGET_NAME}
   csvpp
)
```
