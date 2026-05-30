#include "raylib.h"

typedef enum {
  EMPTY,
  X,
  O,
} CellValue;

typedef enum { PLAYER_X_WINS, PLAYER_O_WINS, DRAW, IN_PROGRESS } GameState;

typedef struct {
  CellValue cells[9];
  GameState gameState;
  int cellResolution;

  CellValue currentPlayer;
  int win_conditions[8][3];

  Rectangle positions[9];
} GameData;

GameData CreateGameData(int cellResolution) {
  GameData gameData = {
      .gameState = IN_PROGRESS,
      .cellResolution = cellResolution,
      .currentPlayer = X,
      .win_conditions = {{0, 1, 2},
                         {3, 4, 5},
                         {6, 7, 8},
                         {0, 3, 6},
                         {1, 4, 7},
                         {2, 5, 8},
                         {0, 4, 8},
                         {2, 4, 6}},

  };

  for (int i = 0; i < 9; i++) {
    int x = (i % 3) * cellResolution;
    int y = (i / 3) * cellResolution;
    gameData.positions[i] = (Rectangle){x, y, cellResolution, cellResolution};
  }

  return gameData;
}

void game_update(GameData *gameData) {
  if (gameData->gameState == IN_PROGRESS) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      Vector2 mousePosition = GetMousePosition();
      int mx = (int)(mousePosition.x / (gameData->cellResolution));
      int my = (int)(mousePosition.y / (gameData->cellResolution));

      int idx = my * 3 + mx;

      if (gameData->cells[idx] == EMPTY) {
        CellValue _currentPlayer = gameData->currentPlayer;
        gameData->cells[idx] = _currentPlayer;
        gameData->currentPlayer = _currentPlayer == X ? O : X;
      }

      // check for winner

      for (int i = 0; i < 8; i++) {
        int xCounter = 0;
        int oCounter = 0;

        for (int j = 0; j < 3; j++) {
          int idx = gameData->win_conditions[i][j];
          CellValue cell = gameData->cells[idx];

          if (cell == X) {
            xCounter++;
          } else if (cell == O) {
            oCounter++;
          }
        }

        if (xCounter == 3) {
          gameData->gameState = PLAYER_X_WINS;
        }
        if (oCounter == 3) {
          gameData->gameState = PLAYER_O_WINS;
        }
      }

      // check for draw
      int emptyCounter = 0;

      for (int i = 0; i < 9; i++) {
        if (gameData->cells[i] == EMPTY) {
          emptyCounter++;
        }
      }

      if (emptyCounter == 0 && gameData->gameState == IN_PROGRESS) {
        gameData->gameState = DRAW;
      }
    }
  } else if (IsKeyPressed(KEY_R)) {
    gameData->gameState = IN_PROGRESS;

    for (int i = 0; i < 9; i++) {
      gameData->cells[i] = EMPTY;
    }
    gameData->currentPlayer = X;
  }
}

void DrawWinText(const char *text, Font font, int fontSize, int padding,
                 Color color) {
  Vector2 textSize = MeasureTextEx(font, text, fontSize, 1);
  textSize.x += padding;
  textSize.y += (int)(padding / 2);
  int posX = (GetRenderWidth() / 2) - (int)(textSize.x / 2);
  int posY = (GetRenderHeight() / 2) - (int)(textSize.y / 2);
  DrawRectangle(posX, posY, textSize.x, textSize.y, Fade(RAYWHITE, 0.8));

  textSize.x -= (int)(padding / 2);
  textSize.y -= (int)(padding / 2);
  posX = (GetRenderWidth() / 2) - (int)(textSize.x / 2);
  posY = (GetRenderHeight() / 2) - (int)(textSize.y / 2);
  DrawText(text, posX, posY, fontSize, color);
}

void render_update(GameData *gameData) {

  int width = GetRenderWidth();
  Font font = GetFontDefault();
  int fontSize = 160;
  int halfCellResolution = (gameData->cellResolution / 2);
  int xOffsetH = halfCellResolution - (MeasureText("X", fontSize) / 2);
  int oOffsetH = halfCellResolution - (MeasureText("O", fontSize) / 2);

  int xOffsetV =
      halfCellResolution - (MeasureTextEx(font, "X", fontSize, 1).y * 0.4);
  int oOffsetV =
      halfCellResolution - (MeasureTextEx(font, "O", fontSize, 1).y * 0.4);

  for (int i = 0; i < 9; i++) {
    CellValue cell = gameData->cells[i];
    Rectangle r = gameData->positions[i];

    int x = r.x;
    int y = r.y;
    int width = r.width;
    int height = r.height;

    if (cell == X) {
      DrawRectangle(x, y, width, height, RED);
      DrawText("X", x + xOffsetH, y + xOffsetV, fontSize, RAYWHITE);
    } else if (cell == O) {
      DrawRectangle(x, y, width, height, BLUE);
      DrawText("O", x + oOffsetH, y + oOffsetV, fontSize, RAYWHITE);
    } else {
      DrawRectangle(x, y, width, height, WHITE);
    }

    DrawRectangleLinesEx(r, 8, BLACK);
  }

  fontSize = 120;
  int padding = fontSize * 3;

  if (gameData->gameState == PLAYER_X_WINS) {
    const char *playerXWinsText = "Player X Wins!\nPress R to Reset...";
    DrawWinText(playerXWinsText, font, fontSize, padding, MAROON);
  } else if (gameData->gameState == PLAYER_O_WINS) {
    const char *playerOWinsText = "Player O Wins!\nPress R to Reset...";
    DrawWinText(playerOWinsText, font, fontSize, padding, DARKBLUE);
  } else if (gameData->gameState == DRAW) {
    const char *playerDrawText = "Draw!\nPress R to Reset...";
    DrawWinText(playerDrawText, font, fontSize, padding, DARKGRAY);
  } else if (gameData->gameState == IN_PROGRESS) {
    if (gameData->currentPlayer == X) {
      DrawText("X", width - (int)(fontSize * 1.25), (int)(fontSize * 0.5),
               fontSize, MAROON);
    } else if (gameData->currentPlayer == O) {
      DrawText("O", width - (int)(fontSize * 1.25), (int)(fontSize * 0.5),
               fontSize, DARKBLUE);
    }
  }
}

int main() {
  int width = 1920;
  int height = 1080;

  unsigned int configFlags = FLAG_FULLSCREEN_MODE;
  SetConfigFlags(configFlags);

  const char *title = "game";
  InitWindow(width, height, title);

  SetExitKey(KEY_Q);
  SetTargetFPS(60);

  GameData gameData = CreateGameData(height / 3);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_F)) {
      ToggleFullscreen();
    }
    game_update(&gameData);

    BeginDrawing();

    ClearBackground(SKYBLUE);

    render_update(&gameData);

    EndDrawing();
  }

  CloseWindow();
}

/* vim:set ts=3 sw=2 sts=2 et: */
