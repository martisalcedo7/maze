
import numpy as np
from utils import get_neighbors
from maps import map
from copy import deepcopy, copy
import cv2 as cv

# from img2array import map_from_image
# map = map_from_image("test.bmp")

from img2array_copy import map_from_image
map = map_from_image("test_2.png")

map_copy = deepcopy(map)
map_copy = np.where(map_copy == 1, 0, 255)
map_copy = map_copy.astype('uint8')
# print(type(map))

a_star = True

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
dist_f = [[float('inf')]*map_size[1]
          for _ in range(map_size[0])]
dist = [[float('inf')]*map_size[1]
        for _ in range(map_size[0])]  # np.ones_like(map) * np.inf
prev = [[None]*map_size[1]
        for _ in range(map_size[0])]
unvisited = [origin]  # deepcopy(all_nodes)
# print(unvisited)

target_found = False

dist[origin[0]][origin[1]] = 0
dist_f[origin[0]][origin[1]] = 10.0 * (abs(
    origin[0]-target[0])+abs(origin[1]-target[1]))

while True:
    min_dist = float('inf')
    node = []  # unvisited[0]
    id = 0
    for idx, nd in enumerate(unvisited):
        if a_star:
            d = dist_f[nd[0]][nd[1]]
        else:
            d = dist[nd[0]][nd[1]]
        if d < min_dist:
            min_dist = d
            node = nd
            id = idx

    if(node == target):
        target_found = True
        break

    if(not unvisited):
        break

    unvisited.pop(id)

    neighbors, costs = get_neighbors(node, all_nodes)
    # print(neighbors)

    for neighbor, cost in zip(neighbors, costs):
        new_dist = dist[node[0]][node[1]] + cost
        if new_dist < dist[neighbor[0]][neighbor[1]]:
            dist[neighbor[0]][neighbor[1]] = new_dist
            prev[neighbor[0]][neighbor[1]] = node
            if a_star:
                dist_f[neighbor[0]][neighbor[1]] = new_dist + \
                    10.0*(abs(neighbor[0]-target[0]) +
                          abs(neighbor[1]-target[1]))
            unvisited.append(neighbor)

    # Show algorithm step, not part of the algorithm
    map_copy[node[0]][node[1]] = 150
    cv.imshow("Display window", map_copy)
    k = cv.waitKey(1)


# Get path to target
path = []
if target_found:
    next = prev[target[0]][target[1]]
    cost = abs(target[0]-next[0])+abs(target[1]-next[1])
    path = [next]
    while next != origin:
        cost += abs(prev[next[0]][next[1]][0]-next[0]) + \
            abs(prev[next[0]][next[1]][1]-next[1])
        next = prev[next[0]][next[1]]
        path.append(next)
    path.pop(-1)

    print(cost)

    for node in path:
        print(node)


# Show result
map_copy[tuple(np.array(path).T)] = 70

# img = (map / map.max()) * 255
# img -= 255
# img = np.abs(img)
# img = img.astype('uint8')
dim = (800, 600)
# resize image
map_copy = cv.resize(map_copy, dim, interpolation=cv.INTER_AREA)
cv.imshow("Display window", map_copy)
k = cv.waitKey(0)
if k == ord("s"):
    cv.imwrite("dijkstra.png", map_copy)
