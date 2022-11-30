#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>

typedef struct Point {
  int32_t r;
  int32_t c;
} Point;

std::vector<std::vector<uint8_t>> parse_map(const std::string path);

uint32_t ravel(const Point point, const uint32_t cols);

Point unravel(const int32_t pos, const uint32_t cols);

void generate_result_file(const std::string path,
                          const std::vector<uint32_t> &previous,
                          const uint32_t target_idx, const uint32_t origin_idx,
                          const uint32_t cols);

#endif
