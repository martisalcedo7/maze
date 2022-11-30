#include "utils.hpp"

#include <fstream>
#include <sstream>

std::vector<std::vector<uint8_t>> parse_map(const std::string path) {
  std::ifstream data(path);
  std::string line;
  std::vector<std::vector<uint8_t>> parsedCsv;
  while (std::getline(data, line)) {
    std::stringstream lineStream(line);
    std::string cell;
    std::vector<uint8_t> parsedRow;
    while (std::getline(lineStream, cell, ',')) {
      parsedRow.push_back(static_cast<uint8_t>(std::stoi(cell)));
    }

    parsedCsv.push_back(parsedRow);
  }

  return parsedCsv;
}

uint32_t ravel(const Point point, const uint32_t cols) {
  return point.c + point.r * cols;
}

Point unravel(const int32_t pos, const uint32_t cols) {
  int32_t r = (pos / cols);
  int32_t c = (pos % cols);
  Point point = {.r = r, .c = c};
  return point;
}

void generate_result_file(const std::string path,
                          const std::vector<uint32_t> &previous,
                          const uint32_t target_idx, const uint32_t origin_idx,
                          const uint32_t cols) {
  std::ofstream result_file(path, std::ios::out | std::ios::trunc);
  if (result_file.is_open()) {
    uint32_t prev_node = previous[target_idx];
    while (prev_node != origin_idx) {
      result_file << unravel(prev_node, cols).r << ","
                  << unravel(prev_node, cols).c << "\n";
      prev_node = previous[prev_node];
    }
    result_file.close();
  } else {
    std::cout << "Unable to open file!";
  }
}
