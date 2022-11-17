
def get_neighbors(node, all_nodes):
    directions = [[0, 1], [1, 0], [0, -1], [-1, 0],
                  [1, 1], [1, -1], [-1, 1], [-1, -1]]
    distances = [1, 1, 1, 1, 1.41, 1.41, 1.41, 1.41]
    neighbors = []
    costs = []

    for dir, cost in zip(directions, distances):
        neighbor = [node[0] + dir[0], node[1] + dir[1]]
        if neighbor in all_nodes:
            neighbors.append(neighbor)
            costs.append(cost)

    return neighbors, costs
