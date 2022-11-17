
#include "map.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct Point {
  int r;
  int c;
} Point;

typedef struct Node {
  Point point;
  int type;
  int visited;
} Node;

int ravel(const Point point) { return point.c + point.r * MAP_COLUMNS; }

Point unravel(const int pos) {
  int r = (pos / MAP_COLUMNS);
  int c = (pos % MAP_COLUMNS);
  Point point = {.r = r, .c = c};
  return point;
}

// Declare arrays
int distance[MAP_ROWS * MAP_COLUMNS];
int previous[MAP_ROWS * MAP_COLUMNS];
Node nodes[MAP_ROWS * MAP_COLUMNS];

const int directions[8][2] = {{0, 1}, {1, 0},  {0, -1}, {-1, 0},
                              {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
const int costs[8] = {10, 10, 10, 10, 14, 14, 14, 14};

int main() {

  int origin = -1;
  int target = -1;

  uint64_t nodes_to_visit = 0;

  // Fill array of nodes nodes and find target and origin
  // Set the wall nodes as visited
  size_t counter = 0;
  for (size_t r = 0; r < MAP_ROWS; ++r) {
    for (size_t c = 0; c < MAP_COLUMNS; ++c) {
      int type = map[r][c];
      int visited = 0;
      if (type == O) {
        origin = counter;
      }
      if (type == T) {
        target = counter;
      }
      if (type == W) {
        visited = 1;
      }
      if (!visited) {
        nodes_to_visit++;
      }
      Point point = {.r = r, .c = c};
      Node node = {.point = point, .type = type, .visited = visited};
      nodes[counter] = node;
      previous[counter] = -1;
      distance[counter] = INT_MAX;
      counter++;
    }
  }

  // Set the distance of the origin node as zero
  distance[origin] = 0;

  uint64_t visited_nodes = 0;
  // Loop for the algorithm
  while (1) {
    // Find unvisited node with the shortest distance
    // Detect if there is no mode nodes to visit
    int unvisited_nodes = 0;
    Node closest_node;
    int closest_node_idx;
    int min_dist = INT_MAX;
    for (size_t i = 0; i < (MAP_ROWS * MAP_COLUMNS); ++i) {
      if (!nodes[i].visited) {
        unvisited_nodes++;
        if (distance[i] < min_dist) {
          min_dist = distance[i];
          closest_node = nodes[i];
          closest_node_idx = i;
        }
      }
    }
    visited_nodes++;
    printf("%li/%li\n", visited_nodes, nodes_to_visit);
    // printf("Min dist: %i\n", min_dist);
    // printf("Closest node: %i\n", closest_node_idx);
    // If there are no more nodes to visit brake while loop
    if (!unvisited_nodes || closest_node_idx == target) {
      break;
    }
    // Set the selected node as visited
    nodes[closest_node_idx].visited = 1;

    // Get neighbors
    size_t counter = 0;
    for (size_t i = 0; i < 8; ++i) {
      int r = closest_node.point.r + directions[i][0];
      int c = closest_node.point.c + directions[i][1];
      // Check if neighbor is valid
      if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLUMNS) {
        // printf("******\nCoordinates: %i, %i\n", r, c);
        Point point = {.r = r, .c = c};
        int pos = ravel(point);
        Node node = nodes[pos];
        // printf("Type: %i\n", node.type);
        if (node.type != W) {
          int new_dist = min_dist + costs[i];
          // printf("Distance: %i\n", distance[pos]);
          if (new_dist < distance[pos]) {

            distance[pos] = new_dist;
            previous[pos] = closest_node_idx;
            // printf("New_dist: %i\n", distance[pos]);
          }
          counter++;
        }
      }
    }
    // printf("-------------\n");
  }

  int prev_node = previous[target];
  while (prev_node != origin) {
    printf("%i -> (%i,%i)\n", prev_node, unravel(prev_node).r,
           unravel(prev_node).c);
    prev_node = previous[prev_node];
  }

  return 0;
}
