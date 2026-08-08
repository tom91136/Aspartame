#pragma once

#include <istream>
#include <string>
#include <utility>

#include "iterator_base.hpp"

namespace aspartame::details {

class istream_line_iterator : public fwd_iterator<istream_line_iterator, std::string, std::input_iterator_tag> {
  std::istream *input = nullptr;
  std::string current;
  char delimiter = '\n';

  void read() {
    if (!input || !std::getline(*input, current, delimiter)) input = nullptr;
  }

public:
  istream_line_iterator() = default;
  explicit istream_line_iterator(std::istream &in, char delimiter = '\n') : input(&in), delimiter(delimiter) { read(); }
  istream_line_iterator &operator++() {
    read();
    return *this;
  }
  [[nodiscard]] const std::string &operator*() { return current; }
  [[nodiscard]] bool operator==(const istream_line_iterator &that) const { return (!input) == (!that.input); }
};

class positioned_istream_line_iterator
    : public fwd_iterator<positioned_istream_line_iterator, std::pair<std::string, std::streampos>, std::input_iterator_tag> {
  std::istream *input = nullptr;
  std::pair<std::string, std::streampos> current;

  void read() {
    if (!input || !std::getline(*input, current.first)) input = nullptr;
    else {
      const auto state = input->rdstate();
      input->clear(state & ~std::ios_base::eofbit);
      current.second = input->tellg();
      input->clear(state);
    }
  }

public:
  positioned_istream_line_iterator() = default;
  explicit positioned_istream_line_iterator(std::istream &in) : input(&in) { read(); }
  positioned_istream_line_iterator &operator++() {
    read();
    return *this;
  }
  [[nodiscard]] const std::pair<std::string, std::streampos> &operator*() { return current; }
  [[nodiscard]] bool operator==(const positioned_istream_line_iterator &that) const { return (!input) == (!that.input); }
};

} // namespace aspartame::details
