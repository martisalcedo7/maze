#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<uint8_t>> parse_map() {
  std::ifstream data("../map.csv");
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

const std::vector<std::vector<uint8_t>> map = parse_map();

const uint32_t map_rows = map.size();
const uint32_t map_cols = map[0].size();

typedef struct Point {
  int32_t r;
  int32_t c;
} Point;

uint32_t ravel(const Point point) { return point.c + point.r * map_cols; }

Point unravel(const int32_t pos) {
  int32_t r = (pos / map_cols);
  int32_t c = (pos % map_cols);
  Point point = {.r = r, .c = c};
  return point;
}

// Declare arrays
std::vector<uint32_t> distance_h((map_rows * map_cols), UINT32_MAX);
std::vector<uint32_t> distance((map_rows * map_cols), UINT32_MAX);
std::vector<uint32_t> previous((map_rows * map_cols));
std::vector<bool> in_visit((map_rows * map_cols), false);

const int8_t directions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                                 {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const uint8_t costs[8] = {10, 10, 10, 10, 14, 14, 14, 14};

int main(int argc, char *argv[]) {

  // Process input args
  const int32_t origin_r = std::stoi(argv[1]);
  const int32_t origin_c = std::stoi(argv[2]);
  const int32_t target_r = std::stoi(argv[3]);
  const int32_t target_c = std::stoi(argv[4]);

  // Define origin and target
  const Point origin = {.r = origin_r, .c = origin_c};
  const Point target = {.r = target_r, .c = target_c};
  const uint32_t origin_idx = ravel(origin);
  const uint32_t target_idx = ravel(target);

  bool a_star = true;

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
      break;
    }

    // Remove selected node from nodes to visit
    visit.erase(visit.begin() + index);
    in_visit[index] = false;

    // Get neighbors
    for (uint8_t i = 0; i < 8; ++i) {
      Point node = unravel(closest_node_idx);
      int32_t r = node.r + directions[i][0];
      int32_t c = node.c + directions[i][1];
      // Check if neighbor is valid
      if (r < 0 || r >= map_rows || c < 0 || c >= map_cols || map[r][c] == 1) {
        continue;
      }
      Point neighbor = {.r = r, .c = c};
      uint32_t neighbor_idx = ravel(neighbor);
      uint32_t new_dist = distance[closest_node_idx] + costs[i];
      if (new_dist < distance[neighbor_idx]) {
        distance[neighbor_idx] = new_dist;
        previous[neighbor_idx] = closest_node_idx;
        if (a_star) {
          distance_h[neighbor_idx] =
              new_dist + labs(r - target.r) + labs(c - target.c);
        }
        // Add neighbor to the nodes to visit if is not there
        if (!in_visit[neighbor_idx]) {
          visit.push_back(neighbor_idx);
          in_visit[neighbor_idx] = true;
        }
      }
    }
  }

  if (!target_found) {
    return 0;
  }

  FILE *fptr;

  // use appropriate location if you are using MacOS or Linux
  fptr = fopen("result.csv", "w");

  if (fptr == NULL) {
    printf("Error creating result file!");
    exit(1);
  }

  uint32_t prev_node = previous[target_idx];
  while (prev_node != origin_idx) {
    fprintf(fptr, "%i,%i\n", unravel(prev_node).r, unravel(prev_node).c);
    prev_node = previous[prev_node];
  }
  fclose(fptr);

  return 0;
}
