import sys

dirs6 = [
    (-1, 0, 0)
    (1, 0, 0)
    (0, -1, 0)
    (0, 1, 0)
    (0, 0, -1)
    (0, 0, 1)
]

if __name__ == '__main__':
    pos = (0, 0, 0)
    pathCost = 0
    with open(sys.argv[1]) as f:
        for line in f:
            d = dirs6[int(line)]
            pos = pos[0] + d[0], pos[1] + d[1], pos[2] + d[2]
            pathCost += 1
            print(pos)
        print("total cost:", pathCost)
