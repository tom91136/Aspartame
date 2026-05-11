// Small end-to-end example: parse a CSV of daily sensor readings, compute
// per-sensor summary stats, and print a sorted report.
//
// Demonstrates strings (`lines`, `trim`, `split`, `is_blank`), containers
// (`collect`, `map`, `tail`, `group_map`, `to_vector`), folds (`fold_left`,
// `reduce`), and ordering (`sort_by`).

#include "aspartame/optional.hpp"
#include "aspartame/string.hpp"
#include "aspartame/vector.hpp"
#include "aspartame/view.hpp"

#include <cstdio>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace aspartame;

namespace {

constexpr const char *kCsv = R"(
SensorID,Day1,Day2,Day3,Day4
SensorA,100,150,130,160
SensorB,90,120,110,140
SensorC,200,230,210,205
SensorD,75,80,95,70
)";

struct Reading {
  std::string sensor;
  std::vector<double> values;
};

struct Summary {
  std::string sensor;
  double mean;
  double min;
  double max;
};

std::vector<std::string> parse_header(const std::string &line) { //
  return line ^ split(",");
}

std::optional<Reading> parse_row(const std::string &line) {
  const auto cols = line ^ split(",");
  if (cols.size() < 2) return std::nullopt;
  auto values = cols                                                     //
                | tail()                                                 //
                | map([](const std::string &c) { return std::stod(c); }) //
                | to_vector();
  return Reading{cols.front(), std::move(values)};
}

Summary summarise(const Reading &r) {
  const double sum = r.values ^ fold_left(0.0, [](double acc, double v) { return acc + v; });
  const auto min = r.values ^ reduce([](double a, double b) { return a < b ? a : b; }) ^ get_or_default();
  const auto max = r.values ^ reduce([](double a, double b) { return a > b ? a : b; }) ^ get_or_default();
  return Summary{r.sensor, sum / static_cast<double>(r.values.size()), min, max};
}

} // namespace

int main() {
  const auto rows = (kCsv ^ lines())                                                                            //
                    | collect([](auto l) { return l ^ is_blank() ? std::nullopt : std::optional{l ^ trim()}; }) //
                    | to_vector();
  if (rows.size() < 2) {
    std::fprintf(stderr, "csv has no data rows\n");
    return 1;
  }

  const auto header = parse_header(rows.front());
  const auto unsorted = rows                                                               //
                        | tail()                                                           //
                        | collect([](const std::string &line) { return parse_row(line); }) //
                        | map([](const Reading &r) { return summarise(r); })               //
                        | to_vector();
  const auto summaries = unsorted ^ sort_by([](const Summary &s) { return -s.mean; }); // descending by mean

  std::cout << "report (" << (header | drop(1) | mk_string(", ")) << ")\n";
  for (const auto &s : summaries) {
    std::cout << "  " << s.sensor    //
              << "  mean=" << s.mean //
              << "  min=" << s.min   //
              << "  max=" << s.max << "\n";
  }
  return 0;
}
