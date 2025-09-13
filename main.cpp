#include <raylib.h>
#include <algorithm>
#include <iterator>

constexpr int WINDOW_WIDTH  = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int CELL_SIZE     = 10;

constexpr int GRID_WIDTH  = WINDOW_WIDTH / CELL_SIZE;
constexpr int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;
constexpr int GRID_SIZE   = GRID_WIDTH * GRID_HEIGHT;

enum Cell {
  Air, Sand, Water
};

Cell grid[GRID_SIZE]   = { Air };
Cell nextGrid[GRID_SIZE] = { Air };

void update() {
		std::copy(std::begin(grid), std::end(grid), std::begin(nextGrid));

		for (int i = GRID_SIZE - 1; i >= 0; i--) {
				if (grid[i] == Air) continue;

				int below      = i + GRID_WIDTH;
				int belowLeft  = below - 1;
				int belowRight = below + 1;

				bool atBottom = below >= GRID_SIZE;

				if (!atBottom && grid[below] == Air) {
						nextGrid[i] = Air;
						nextGrid[below] = Sand;
			    } else if (!atBottom && (i % GRID_WIDTH != 0) && grid[belowLeft] == Air) {
						nextGrid[i] = Air;
                        nextGrid[belowLeft] = Sand;                                                
				} else if (!atBottom && ((i + 1) % GRID_WIDTH != 0) && grid[belowRight] == Air) {
						nextGrid[i] = Air;
						nextGrid[belowRight] = Sand;
				}
	    }

		std::copy(std::begin(nextGrid), std::end(nextGrid), std::begin(grid));
}

void Draw(int mouseX, int mouseY) {
		BeginDrawing();
		ClearBackground(BLACK);

		for (int i = 0; i < GRID_SIZE; i++) {
				if (grid[i] == Sand) {
						int x = (i % GRID_WIDTH) * CELL_SIZE;
						int y = (i / GRID_WIDTH) * CELL_SIZE;
						DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, YELLOW);
				}
		}

		DrawRectangle(mouseX * CELL_SIZE, mouseY * CELL_SIZE, CELL_SIZE, CELL_SIZE, {255, 255, 255, 100});

		EndDrawing(); 
}

int main() {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Falling Sand");


	SetTargetFPS(180);

	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();
		int mouseX = std::clamp(static_cast<int>(mousePos.x / CELL_SIZE), 0, GRID_WIDTH - 1);
		int mouseY = std::clamp(static_cast<int>(mousePos.y / CELL_SIZE), 0, GRID_HEIGHT - 1);
		int mouseIndex = mouseY * GRID_WIDTH + mouseX;

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))  grid[mouseIndex] = Sand;
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) grid[mouseIndex] = Air;

		Draw(mouseX, mouseY);
	}

	CloseWindow();
	return 0;
}
