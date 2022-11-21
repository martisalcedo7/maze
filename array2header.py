from img2array import map_from_image
from maps import map
import numpy as np
import sys
np.set_printoptions(threshold=sys.maxsize)

map_2 = np.array([[0, 0, 0, 0, 0, 0, 0, 3, 0],
                  [0, 1, 1, 1, 0, 1, 0, 0, 0],
                  [0, 0, 0, 0, 0, 0, 0, 0, 0],
                  [0, 0, 0, 1, 1, 1, 1, 0, 0],
                  [0, 2, 0, 0, 0, 0, 0, 0, 0]])


map = map_from_image("test.bmp")


def array_to_header(map):
    rows, cols = map.shape

    str_map = "#include <stdint.h>\n"
    str_map += "#ifndef MAP_H\n#define MAP_H\n"
    str_map += "#define E 0\n#define W 1\n#define O 2\n#define T 3\n"
    str_map += "#define MAP_ROWS " + str(rows) + "u\n"
    str_map += "#define MAP_COLUMNS " + str(cols) + "u\n"
    str_map += "#define VISITABLE_NODES " + \
        str(np.count_nonzero(map != 1)) + "u\n"
    str_map += "const uint8_t map[MAP_ROWS][MAP_COLUMNS] = {\n"

    for row in map:
        str_row = str(row)
        str_row = str_row.replace("0", "E")
        str_row = str_row.replace("1", "W")
        str_row = str_row.replace("2", "O")
        str_row = str_row.replace("3", "T")
        str_row = str_row.replace("[", "{").replace("]", "}").replace(" ", ",")
        str_row += ",\n"
        str_map += str_row

    str_map = str_map[:-2]
    str_map += "};\n"
    str_map += "#endif\n"

    path = "c/map.h"
    with open(path, "w") as header:
        header.write(str_map)

    # print(str_map)

    # return str_map


if __name__ == "__main__":
    array_to_header(map)
