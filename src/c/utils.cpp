#include "utils.hpp"
#include <array>
#include <fstream>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
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

void get_path(const uint32_t origin_idx, const uint32_t target_idx,
              const std::vector<uint32_t> &previous,
              std::vector<uint32_t> &path) {

  uint32_t prev_node = previous[target_idx];
  while (prev_node != origin_idx) {
    path.push_back(prev_node);
    prev_node = previous[prev_node];
  }
}

uint32_t heuristic(const Point &origin, const Point &target) {
  return labs(origin.r - target.r) + labs(origin.c - target.c);
}

bool a_star(const uint32_t origin_idx, const uint32_t target_idx,
            const std::vector<std::vector<uint8_t>> &map,
            std::vector<uint32_t> &distance, std::vector<uint32_t> &previous,
            bool visualization) {

  // Get map sizes
  const uint32_t map_rows = map.size();
  const uint32_t map_cols = map[0].size();

  // Calculate origin and target indexes
  // const uint32_t origin_idx = ravel(origin, map_cols);
  // const uint32_t target_idx = ravel(target, map_cols);
  const Point origin = unravel(origin_idx, map_cols);
  const Point target = unravel(target_idx, map_cols);

  // Define arrays
  distance.assign((map_rows * map_cols), UINT32_MAX);
  previous.assign((map_rows * map_cols), 0);
  std::vector<uint32_t> distance_h((map_rows * map_cols), UINT32_MAX);
  std::vector<bool> in_visit((map_rows * map_cols), false);

  // Define neighbor directions and costs
  const std::array<std::array<int8_t, 2>, 8> directions = {
      {{0, 1},
       {1, 0},
       {0, -1},
       {-1, 0}}}; //, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}};
  const std::array<uint8_t, 8> costs = {10, 10, 10, 10}; //, 14, 14, 14, 14};

  // Set the distance of the origin node as zero and heuristic for a_star
  distance[origin_idx] = 0;
  distance_h[origin_idx] = heuristic(origin, target);

  bool target_found = false;
  std::vector<uint32_t> visit = {origin_idx};
  in_visit[origin_idx] = true;

  // Plot visited pixel in the image
  cv::Mat test(map_rows, map_cols, CV_8UC1);
  for (uint32_t i = 0; i < map_rows; i++) {
    for (uint32_t j = 0; j < map_cols; j++) {
      if (!map[i][j]) {
        test.at<uchar>(i, j) = 255;
      }
    }
  }
  cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
  cv::Mat dst;
  uint32_t counter = 0;

  // Loop for the algorithm
  while (true) {

    // Detect if there is no more nodes to visit
    if (visit.empty()) {
      break;
    }

    // Find unvisited node with the shortest heuristic
    uint32_t closest_node_idx = 0;
    uint32_t min_dist = UINT32_MAX;
    uint32_t visit_index = 0;
    for (uint32_t i = 0; i < visit.size(); ++i) {
      uint32_t dist = distance_h[visit[i]];
      if (dist < min_dist) {
        min_dist = dist;
        closest_node_idx = visit[i];
        visit_index = i;
      }
    }

    // Detect if Target has been reached
    if (closest_node_idx == target_idx) {
      target_found = true;
      break;
    }

    // Remove selected node from nodes to visit
    visit.erase(visit.begin() + visit_index);
    in_visit[closest_node_idx] = false;

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
        distance_h[neighbor_idx] = new_dist + heuristic(neighbor, target);
        // Add neighbor to the nodes to visit if is not there
        if (!in_visit[neighbor_idx]) {
          visit.push_back(neighbor_idx);
          in_visit[neighbor_idx] = true;
        }
      }
    }

    // Plot visited pixel in the image
    Point pixel = unravel(closest_node_idx, test.cols);
    test.at<uchar>(pixel.r, pixel.c) = 125;
    if (counter % 100 == 0) {
      cv::resize(test, dst, cv::Size(), 0.5, 0.5, cv::INTER_AREA);
      cv::imshow("Display Image", dst);
      cv::waitKey(1);
      counter = 0;
    }
    counter++;
  }

  // Plot visited pixel in the image
  cv::destroyAllWindows();

  return target_found;
}
