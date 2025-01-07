import pygame
import sys

pygame.init()

WIDTH, HEIGHT = 400, 400
CELL_SIZE = WIDTH // 8

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
GREEN = (0, 128, 0)

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Bitboard Visualizer")

def draw_bitboard(bitboard):
    screen.fill(WHITE)
    for row in range(8):
        for col in range(8):
            bit_index = row * 8 + col
            bit_set = (bitboard >> bit_index) & 1
            color = GREEN if bit_set else BLACK
            pygame.draw.rect(screen, color, pygame.Rect(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE))
            pygame.draw.rect(screen, WHITE, pygame.Rect(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE, CELL_SIZE), 1)

try:
    bitboard = int(input("Enter bitboard: "))
except ValueError:
    print("Incorrect value")
    bitboard = 15

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = pygame.mouse.get_pos()
            col, row = x // CELL_SIZE, y // CELL_SIZE
            bit_index = row * 8 + col
            bitboard ^= (1 << bit_index)  
            print(f"New bitboard: {bitboard}")  

    draw_bitboard(bitboard)
    pygame.display.flip()
