#include "utils.hpp"
#include <array>

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

  // Declare arrays
  std::vector<uint32_t> distance_h((map_rows * map_cols), UINT32_MAX);
  std::vector<uint32_t> distance((map_rows * map_cols), UINT32_MAX);
  std::vector<uint32_t> previous((map_rows * map_cols));
  std::vector<bool> in_visit((map_rows * map_cols), false);
  // Neighbor directions
  const std::array<std::array<int8_t, 2>, 8> directions = {
      {{0, 1},
       {1, 0},
       {0, -1},
       {-1, 0}}}; //, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
  const std::array<uint8_t, 8> costs = {10, 10, 10, 10}; //, 14, 14, 14, 14};

  // Set the distance of the origin node as zero or heuristic for a star
  distance[origin_idx] = 0;
  distance_h[origin_idx] =
      labs(origin.r - target.r) + labs(origin.c - target.c);

  bool target_found = false;
  std::vector<uint32_t> visit = {origin_idx};
  in_visit[origin_idx] = true;
  // Loop for the algorithm
  while (1) {

    // Detect if there is no more nodes to visit
    if (visit.empty()) {
      std::cout << "Target not found\n";
      break;
    }

    // Find unvisited node with the shortest distance
    uint32_t closest_node_idx = 0;
    uint32_t min_dist = UINT32_MAX;
    uint32_t index = 0;
    for (uint32_t i = 0; i < visit.size(); ++i) {
      uint32_t dist = distance_h[visit[i]];
      if (dist < min_dist) {
        min_dist = dist;
        closest_node_idx = visit[i];
        index = i;
      }
    }

    // Detect if Target has been reached
    if (closest_node_idx == target_idx) {
      target_found = true;
      std::cout << "Target found\n";
      generate_result_file(result_path, previous, target_idx, origin_idx,
                           map_cols);
      break;
    }

    // Remove selected node from nodes to visit
    visit.erase(visit.begin() + index);
    in_visit[index] = false;

    // Get neighbors
    for (uint8_t i = 0; i < directions.size(); ++i) {
      Point node = unravel(closest_node_idx, map_cols);
      int32_t r = node.r + directions[i][0];
      int32_t c = node.c + directions[i][1];
      // Check if neighbor is valid
      if (r < 0 || r >= map_rows || c < 0 || c >= map_cols || map[r][c] == 1) {
        continue;
      }
      Point neighbor = {.r = r, .c = c};
      uint32_t neighbor_idx = ravel(neighbor, map_cols);
      uint32_t new_dist = distance[closest_node_idx] + costs[i];
      // If new dist from closest node smaller, the update positions and
      // previous
      if (new_dist < distance[neighbor_idx]) {
        distance[neighbor_idx] = new_dist;
        previous[neighbor_idx] = closest_node_idx;
        distance_h[neighbor_idx] =
            new_dist + labs(r - target.r) + labs(c - target.c);
        // Add neighbor to the nodes to visit if is not there
        if (!in_visit[neighbor_idx]) {
          visit.push_back(neighbor_idx);
          in_visit[neighbor_idx] = true;
        }
      }
    }
  }

  return 0;
}
