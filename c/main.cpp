#include "map.h"
#include <array>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <string>
#include <vector>

#define E 0
#define W 1
#define O 2
#define T 3

std::vector<std::vector<uint8_t>> parseCSV() {
  std::ifstream data("../map.csv");
  std::string line;
  std::vector<std::vector<uint8_t>> parsedCsv;
  while (std::getline(data, line)) {
    std::stringstream lineStream(line);
    std::string cell;
    std::vector<uint8_t> parsedRow;
    while (std::getline(lineStream, cell, ',')) {
      parsedRow.push_back(static_cast<uint8_t>(stoi(cell)));
    }

    parsedCsv.push_back(parsedRow);
  }

  return parsedCsv;
}

const std::vector<std::vector<uint8_t>> map = parseCSV();

typedef struct Point {
  int32_t r;
  int32_t c;
} Point;

typedef struct Node {
  Point point;
  uint8_t type;
} Node;

uint32_t ravel(const Point point) { return point.c + point.r * MAP_COLUMNS; }

Point unravel(const int32_t pos) {
  int32_t r = (pos / MAP_COLUMNS);
  int32_t c = (pos % MAP_COLUMNS);
  Point point = {.r = r, .c = c};
  return point;
}

// Declare arrays
uint32_t distance_h[MAP_ROWS * MAP_COLUMNS];
uint32_t distance[MAP_ROWS * MAP_COLUMNS];
uint32_t previous[MAP_ROWS * MAP_COLUMNS];

const int8_t directions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                                 {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const uint8_t costs[8] = {10, 10, 10, 10, 14, 14, 14, 14};

int main() {

  bool a_star = true;

  uint32_t origin;
  uint32_t target;

  // Fill arrays and find target and origin
  uint32_t counter = 0;
  for (uint32_t r = 0; r < MAP_ROWS; ++r) {
    for (uint32_t c = 0; c < MAP_COLUMNS; ++c) {
      if (map[r][c] == O) {
        origin = counter;
      }
      if (map[r][c] == T) {
        target = counter;
      }
      previous[counter] = 0;
      distance[counter] = UINT32_MAX;
      distance_h[counter] = UINT32_MAX;
      counter++;
    }
  }

  // Set the distance of the origin node as zero
  distance[origin] = 0;
  distance_h[origin] = labs(unravel(origin).r - unravel(target).r) +
                       labs(unravel(origin).c - unravel(target).c);

  bool target_found = false;
  std::vector<uint32_t> visit = {origin};

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
    if (closest_node_idx == target) {
      target_found = true;
      std::cout << "Target found\n";
      break;
    }

    // Remove selected node from nodes to visit
    visit.erase(visit.begin() + index);

    // Get neighbors
    for (uint8_t i = 0; i < 8; ++i) {
      Point node = unravel(closest_node_idx);
      int32_t r = node.r + directions[i][0];
      int32_t c = node.c + directions[i][1];
      // Check if neighbor is valid
      if (r < 0 || r >= MAP_ROWS || c < 0 || c >= MAP_COLUMNS ||
          map[r][c] == W) {
        continue;
      }

      Point neighbor = {.r = r, .c = c};
      uint32_t neighbor_idx = ravel(neighbor);
      uint32_t new_dist = distance[closest_node_idx] + costs[i];
      if (new_dist < distance[neighbor_idx]) {
        distance[neighbor_idx] = new_dist;
        previous[neighbor_idx] = closest_node_idx;
        if (a_star) {
          distance_h[neighbor_idx] = new_dist + labs(r - unravel(target).r) +
                                     labs(c - unravel(target).c);
        }
        // Add neighbor to the nodes to visit
        visit.push_back(neighbor_idx);
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

  uint32_t prev_node = previous[target];
  while (prev_node != origin) {
    fprintf(fptr, "%i,%i\n", unravel(prev_node).r, unravel(prev_node).c);
    prev_node = previous[prev_node];
  }
  fclose(fptr);

  return 0;
}
