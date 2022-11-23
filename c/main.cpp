#include "map.h"
#include <iostream>
#include <limits.h>
#include <vector>

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
Node nodes[MAP_ROWS * MAP_COLUMNS];

const int8_t directions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                                 {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const uint8_t costs[8] = {10, 10, 10, 10, 14, 14, 14, 14};

int main() {

  bool a_star = true;

  uint32_t origin;
  uint32_t target;

  uint32_t nodes_to_visit = 0;

  // Fill array of nodes nodes and find target and origin
  // Set the wall nodes as visited
  uint32_t counter = 0;
  for (uint32_t r = 0; r < MAP_ROWS; ++r) {
    for (uint32_t c = 0; c < MAP_COLUMNS; ++c) {
      uint8_t type = map[r][c];
      if (type == O) {
        origin = counter;
      }
      if (type == T) {
        target = counter;
      }
      if (type != W) {
        nodes_to_visit++;
      }
      Point point;
      point.r = r;
      point.c = c;
      Node node = {.point = point, .type = type};
      nodes[counter] = node;
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
    // Find unvisited node with the shortest distance
    // Detect if there is no mode nodes to visit
    if (visit.empty()) {
      std::cout << "Target not found\n";
      break;
    }

    uint32_t unvisited_nodes = 0;
    Node closest_node;
    uint32_t closest_node_idx;
    uint32_t min_dist = UINT32_MAX;
    uint32_t index = 0;
    for (uint32_t i = 0; i < visit.size(); ++i) {
      uint32_t dist;
      dist = distance_h[visit[i]];
      if (dist < min_dist) {
        min_dist = dist;
        closest_node = nodes[visit[i]];
        closest_node_idx = visit[i];
        index = i;
      }
    }

    if (closest_node_idx == target) {
      target_found = true;
      std::cout << "Target found\n";
      break;
    }
    // Set the selected node as visited
    visit.erase(visit.begin() + index);

    // Get neighbors
    for (uint8_t i = 0; i < 8; ++i) {
      int32_t r = closest_node.point.r + directions[i][0];
      int32_t c = closest_node.point.c + directions[i][1];
      // Check if neighbor is valid
      if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLUMNS) {
        // printf("******\nCoordinates: %i, %i\n", r, c);
        Point point = {.r = r, .c = c};
        uint32_t pos = ravel(point);
        Node node = nodes[pos];
        // printf("Type: %i\n", node.type);
        if (node.type != W) {
          uint32_t new_dist = distance[closest_node_idx] + costs[i];
          // printf("Distance: %i\n", distance[pos]);
          if (new_dist < distance[pos]) {
            distance[pos] = new_dist;
            previous[pos] = closest_node_idx;
            if (a_star) {
              distance_h[pos] = new_dist + labs(r - unravel(target).r) +
                                labs(c - unravel(target).c);
            }
            visit.push_back(pos);
            // printf("New_dist: %i\n", distance[pos]);
          }
        }
      }
    }
    // printf("-------------\n");
  }

  if (!target_found) {
    return 0;
  }

  FILE *fptr;

  // use appropriate location if you are using MacOS or Linux
  fptr = fopen("/home/ms/Desktop/maze-main/c/result.txt", "w");

  if (fptr == NULL) {
    printf("Error!");
    exit(1);
  }

  uint32_t prev_node = previous[target];
  while (prev_node != origin) {
    // printf("%i -> (%i,%i)\n", prev_node, unravel(prev_node).r,
    //        unravel(prev_node).c);
    fprintf(fptr, "[%i,%i]\n", unravel(prev_node).r, unravel(prev_node).c);
    prev_node = previous[prev_node];
  }
  fclose(fptr);

  return 0;
}
