#include <csvpp/Read.h>
#include <csvpp/String.h>

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

  std::vector<std::size_t> indices;
  std::size_t headersSize;
  {
    std::getline(stream, lineBuffer);
    auto raw = split(separator_, stringDelimiterKind_, lineBuffer);
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
    auto slices = split(separator_, stringDelimiterKind_, lineBuffer);
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
