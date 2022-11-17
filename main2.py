import numpy as np
from utils import get_neighbors
from maps import map
from copy import deepcopy, copy
import cv2 as cv

from img2array import map_from_image

map = map_from_image("test.bmp")
# map_copy = deepcopy(map)
# print(type(map))

map_size = list(map.shape)
origin = None
target = None
all_nodes = []
for x in range(map_size[0]):
    for y in range(map_size[1]):
        if map[x, y] != 1:
            all_nodes.append([x, y])
        if map[x, y] == 2:
            origin = [x, y]
        if map[x, y] == 3:
            target = [x, y]
# print(origin)
# print(target)

# Algorithm
dist = [[float('inf')]*map_size[1]
        for _ in range(map_size[0])]  # np.ones_like(map) * np.inf
prev = [[None]*map_size[1]
        for _ in range(map_size[0])]
unvisited = deepcopy(all_nodes)
visited = []

dist[origin[0]][origin[1]] = 0

while unvisited:

    min_dist = dist[unvisited[0][0]][unvisited[0][1]]
    node = unvisited[0]
    id = 0
    for idx, nd in enumerate(unvisited):
        if dist[nd[0]][nd[1]] < min_dist:
            min_dist = dist[nd[0]][nd[1]]
            node = nd
            id = idx
    unvisited.pop(id)
    visited.append(node)

    if(node[0] == target[0] and node[1] == target[1]):
        break

    neighbors, costs = get_neighbors(node, all_nodes)

    for neighbor, cost in zip(neighbors, costs):
        new_dist = dist[node[0]][node[1]] + cost
        if new_dist < dist[neighbor[0]][neighbor[1]]:
            dist[neighbor[0]][neighbor[1]] = new_dist
            prev[neighbor[0]][neighbor[1]] = node

    # Show algorithm step, not part of the algorithm
    # map_copy[tuple(np.array(visited).T)] = 4
    # img = (map_copy / map_copy.max()) * 255
    # img -= 255
    # img = np.abs(img)
    # img = img.astype('uint8')
    # dim = (800, 600)
    # # resize image
    # resized = cv.resize(img, dim, interpolation=cv.INTER_AREA)
    # cv.imshow("Display window", resized)
    # k = cv.waitKey(1)


# Get path to target
next = prev[target[0]][target[1]]
path = [next]
while next != origin:
    next = prev[next[0]][next[1]]
    path.append(next)
path.pop(-1)

for node in path:
    print(node)


# Show result
# map[tuple(np.array(path).T)] = 4

# img = (map / map.max()) * 255
# img -= 255
# img = np.abs(img)
# img = img.astype('uint8')
# dim = (800, 600)
# # resize image
# resized = cv.resize(img, dim, interpolation=cv.INTER_AREA)
# cv.imshow("Display window", resized)
# k = cv.waitKey(0)
# if k == ord("s"):
#     cv.imwrite("starry_night.png", resized)
