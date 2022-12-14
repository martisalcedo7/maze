
#include "map.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Point {
  int32_t r;
  int32_t c;
} Point;

typedef struct Node {
  Point point;
  uint8_t type;
  bool visited;
} Node;

// typedef struct VNode {
//   Point point;
//   uint8_t type;
//   bool visited;
// } VNode;

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
      bool visited = true;
      if (type == O) {
        origin = counter;
        visited = false;
      }
      if (type == T) {
        target = counter;
      }
      if (type == W) {
        visited = true;
      } else {
        nodes_to_visit++;
      }
      Point point = {.r = r, .c = c};
      Node node = {.point = point, .type = type, .visited = visited};
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

  uint32_t visited_nodes = 0;
  bool target_found = false;
  // Loop for the algorithm
  while (1) {
    // Find unvisited node with the shortest distance
    // Detect if there is no mode nodes to visit
    uint32_t unvisited_nodes = 0;
    Node closest_node;
    uint32_t closest_node_idx;
    uint32_t min_dist = UINT32_MAX;
    for (size_t i = 0; i < (MAP_ROWS * MAP_COLUMNS); ++i) {
      if (!nodes[i].visited) {
        unvisited_nodes++;
        uint32_t dist;
        if (a_star) {
          dist = distance_h[i];
        } else {
          dist = distance[i];
        }
        if (dist < min_dist) {
          min_dist = dist;
          closest_node = nodes[i];
          closest_node_idx = i;
        }
      }
    }
    visited_nodes++;
    if (visited_nodes % 10000 == 0) {
      printf("%i/%i\n", visited_nodes, nodes_to_visit);
    }
    // printf("Min dist: %i\n", min_dist);
    // printf("Closest node: %i\n", closest_node_idx);
    // If there are no more nodes to visit brake while loop
    if (!unvisited_nodes) {
      printf("Target not found\n");
      break;
    }

    if (closest_node_idx == target) {
      target_found = true;
      printf("Target found\n");
      break;
    }
    // Set the selected node as visited
    nodes[closest_node_idx].visited = true;

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
            nodes[pos].visited = false;
            // printf("New_dist: %i\n", distance[pos]);
          }
        }
      }
    }
    // printf("-------------\n");
  }

  // if (!target_found) {
  //   return 0;
  // }

  // FILE *fptr;

  // // use appropriate location if you are using MacOS or Linux
  // fptr = fopen("/home/ms/Desktop/maze-main/c/result.txt", "w");

  // if (fptr == NULL) {
  //   printf("Error!");
  //   exit(1);
  // }

  // uint32_t prev_node = previous[target];
  // while (prev_node != origin) {
  //   printf("%i -> (%i,%i)\n", prev_node, unravel(prev_node).r,
  //          unravel(prev_node).c);
  //   fprintf(fptr, "[%i,%i]\n", unravel(prev_node).r, unravel(prev_node).c);
  //   prev_node = previous[prev_node];
  // }
  // fclose(fptr);

  return 0;
}
