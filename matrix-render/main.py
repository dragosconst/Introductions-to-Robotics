import pygame
import numpy as np

CELLW, CELLH = 30, 30
DIMW, DIMH = CELLW * 8, CELLH * 8
matrix = np.zeros((8, 8))

def draw_matrix(window):
    for i, line in enumerate(matrix):
        for j, el in enumerate(line):
            x, y = j * CELLW, i * CELLH
            if el == 1:
                pygame.draw.rect(window, (255, 0, 0), (x, y, CELLW, CELLH))
                pygame.draw.rect(window, (0, 0, 0), (x, y, CELLW, CELLH), 1)
            else:
                pygame.draw.rect(window, (255, 255, 255), (x, y, CELLW, CELLH))
                pygame.draw.rect(window, (0, 0, 0), (x, y, CELLW, CELLH), 1)
    pygame.display.update()


def main():
    window = pygame.display.set_mode(size=(DIMW, DIMH))
    draw_matrix(window)

if __name__ == "__main__":
    while True:
        main()
        for event in pygame.event.get():
            if event.type == pygame.MOUSEBUTTONUP:
                mx, my = pygame.mouse.get_pos()
                cx, cy = mx // CELLH, my // CELLW
                matrix[cy, cx] = 1 if not matrix[cy, cx] else 0