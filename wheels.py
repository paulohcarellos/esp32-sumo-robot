import numpy as np



def velocities(x, y):
    radius = (x*x + y*y) ** (1/2)

    left = 1 - x * 2
    right = 1 + x * 2

    left *= radius
    right *= radius

    left = 1 if left > 1 else left
    left = -1 if left < -1 else left

    right = 1 if right > 1 else right
    right = -1 if right < -1 else right

    if y < 0:
        return -right, -left

    return left, right

print(velocities(0.2, -1))