#include <csvpp/Read.h>

#include <fstream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace csvpp {
void Reader::setSeparator(char sep) {
  if ((sep == '"') || (sep == '\'')) {
    std::stringstream err;
    err << sep
        << " must be reserved for string delimiterand can't be used as "
           "separator";
    throw std::runtime_error{err.str()};
  }
  separator_ = sep;
}

namespace {
struct StringDelimiters {
  std::size_t front;
  std::size_t back;
};

std::optional<StringDelimiters> next_string(std::string &line, std::size_t pos,
                                            char delimiter) {
  auto front = line.find(delimiter, pos);
  if (std::string::npos == front) {
    return std::nullopt;
  }
  auto back = line.find(delimiter, front + 1);
  if (std::string::npos == front) {
    throw std::runtime_error{"Wasn't able to find string end delimiter"};
  }
  return StringDelimiters{front, back};
}

void strip(std::string_view &subject, char delimiter) {
  std::size_t strippedL = 0, strippedR = 0;
  for (auto it = subject.begin(); it != subject.end(); ++it) {
    if (*it != ' ') {
      break;
    }
    ++strippedL;
  }
  if (strippedL < subject.size()) {
    for (auto it = subject.rbegin(); it != subject.rend(); ++it) {
      if (*it != ' ') {
        break;
      }
      ++strippedR;
    }
  }
  if (delimiter == subject[strippedL]) {
    ++strippedL;
    ++strippedR;
  }
  subject = std::string_view{subject.data() + strippedL,
                             subject.size() - strippedL - strippedR};
}

Line split(std::string &line, char sep, char delimiter) {
  Line res;
  std::size_t pos = 0;
  auto nextString = next_string(line, pos, delimiter);
  while (pos < line.size()) {
    std::size_t sep_begin = line.find(sep, pos);

    if (sep_begin == std::string::npos) {
      sep_begin = line.size();
    }

    if (nextString.has_value()) {
      if ((nextString->front < sep_begin) && (sep_begin < nextString->back)) {
        pos = sep_begin + 1;
        continue;
      }
      if (sep_begin > nextString->back) {
        nextString = next_string(line, sep_begin, delimiter);
      }
    }

    auto &added =
        res.emplace_back(std::string_view{line.data() + pos, sep_begin - pos});
    strip(added, delimiter);

    pos = sep_begin + 1;
  }
  return res;
}

std::vector<std::size_t> parse_headers(const Line &raw,
                                       const std::vector<std::string> &fields) {
  std::vector<std::size_t> res;
  std::unordered_map<std::string, std::size_t> headersMap;
  for (std::size_t pos = 0; pos < raw.size(); ++pos) {
    std::string header{raw[pos]};
    if (headersMap.find(header) != headersMap.end()) {
      std::stringstream err;
      err << "Headers must be unique, " << header
          << " and possibly others are repeated instead";
      throw std::runtime_error{err.str()};
    }
    headersMap.emplace(header, pos);
  }
  for (const auto &field : fields) {
    auto it = headersMap.find(field);
    if (it == headersMap.end()) {
      std::stringstream err;
      err << field << "; is a non existing header" << std::endl;
      throw std::runtime_error{err.str()};
    }
    res.emplace_back(it->second);
  }
  return res;
}
} // namespace

void Reader::read(const std::string &fileName, LineProcessor &proc) const {
  std::ifstream stream{fileName};
  if (!stream.is_open()) {
    std::stringstream err;
    err << fileName << " is an invalid path" << std::endl;
    throw std::runtime_error{err.str()};
  }
  std::string lineBuffer;

  char stringDelimiter = stringDelimiterKind_ ? '"' : '\'';

  std::vector<std::size_t> indices;
  std::size_t headersSize;
  {
    std::getline(stream, lineBuffer);
    auto raw = split(lineBuffer, separator_, stringDelimiter);
    headersSize = raw.size();
    if (auto fields = proc.expectedFields(); fields != nullptr) {
      indices = parse_headers(raw, *fields);
    }
  }

  while (!stream.eof()) {
    std::getline(stream, lineBuffer);
    if (lineBuffer.empty()) {
      continue;
    }
    auto slices = split(lineBuffer, separator_, stringDelimiter);
    if (slices.size() != headersSize) {
      throw std::runtime_error{"found at least one line with a different "
                               "number of elements w.r.t. to headers"};
    }
    if (!indices.empty()) {
      Line sortedSlices;
      sortedSlices.reserve(indices.size());
      for (const auto &index : indices) {
        sortedSlices.emplace_back(slices[index]);
      }
      slices = std::move(sortedSlices);
    }
    proc.process(slices);
  }
}
} // namespace csvpp
