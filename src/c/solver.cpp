#include "utils.hpp"

#include <chrono>

// External libs

int main(int argc, char *argv[]) {

  // Process input args
  const std::string map_path = argv[1];
  const std::string result_path = argv[2];
  const int32_t origin_r = std::stoi(argv[3]);
  const int32_t origin_c = std::stoi(argv[4]);
  const int32_t target_r = std::stoi(argv[5]);
  const int32_t target_c = std::stoi(argv[6]);

  // Load map
  const std::vector<std::vector<uint8_t>> map = parse_map(map_path);
  const uint32_t map_rows = map.size();
  const uint32_t map_cols = map[0].size();

  // Define origin and target
  const Point origin = {.r = origin_r, .c = origin_c};
  const Point target = {.r = target_r, .c = target_c};
  const uint32_t origin_idx = ravel(origin, map_cols);
  const uint32_t target_idx = ravel(target, map_cols);

  // Declare result arrays
  std::vector<uint32_t> distance;
  std::vector<uint32_t> previous;

  auto t1 = std::chrono::high_resolution_clock::now();
  bool target_found = a_star(origin_idx, target_idx, map, distance, previous);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

  if (target_found) {

    std::cout << "Path found in: " << ms_int.count() << "ms\n";

    // std::vector<uint32_t> path;
    // get_path(origin_idx, target_idx, previous, path);
    generate_result_file(result_path, previous, target_idx, origin_idx,
                         map_cols);
  }

  return 0;
}
