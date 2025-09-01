#include "raylib.h"
#include "utils.hpp"
#include <string>

class Player {
  Rectangle rect = {0, 0, 8, 24};
  KeyboardKey keybinds[2] = {KEY_W, KEY_S};

  float speed = 50;

public:
  void draw() { DrawRectangleRec(rect, RED); }

  void update(float dt) {
    // up
    if (IsKeyDown(keybinds[0])) {
      rect.y -= speed * dt;
    }
    // down
    else if (IsKeyDown(keybinds[1])) {
      rect.y += speed * dt;
    }
  }

  Player(Rectangle rect, KeyboardKey keybinds[2]) : rect(rect) {
    for (int i = 0; i < 2; i++) {
      this->keybinds[i] = keybinds[i];
    }
  }

  Rectangle &getRect() { return rect; }
};

class ScoreManager {
  int p1score = 0;
  int p2score = 0;

public:
  void resetScores() {
    p1score = 0;
    p2score = 0;
  }

  int getScoreP1() { return p1score; }
  int getScoreP2() { return p2score; }

  void setScoreP1(int n) { p1score = n; }
  void setScoreP2(int n) { p2score = n; }
};

int main() {
  Player player1 =
      Player((Rectangle){21, 170, 8, 24}, (KeyboardKey[2]){KEY_W, KEY_S});
  Player player2 =
      Player((Rectangle){71, 170, 8, 24}, (KeyboardKey[2]){KEY_UP, KEY_DOWN});

  ScoreManager scores;

  InitWindow(800, 600, "Space Race");

  SetTargetFPS(60);

  SetWindowState(FLAG_WINDOW_RESIZABLE);

  RenderTexture2D target = LoadRenderTexture(100, 200);

  while (!WindowShouldClose()) {
    BeginTextureMode(target);
    ClearBackground(BLACK);
    player1.draw();
    player2.draw();

    std::string p1score_text = std::to_string(scores.getScoreP1());
    std::string p2score_text = std::to_string(scores.getScoreP2());

    DrawText(p1score_text.c_str(),
             40 - (MeasureText(p1score_text.c_str(), 12) / 2), 170, 12,
             scores.getScoreP1() >= scores.getScoreP2() ? GREEN : RED);
    DrawText(p2score_text.c_str(),
             60 - (MeasureText(p2score_text.c_str(), 12) / 2), 170, 12,
             scores.getScoreP1() <= scores.getScoreP2() ? GREEN : RED);

    EndTextureMode();

    BeginDrawing();
    ClearBackground(WHITE);
    DrawRenderTexture(target, 150, 200);
    EndDrawing();

    float dt = GetFrameTime();

    player1.update(dt);
    player2.update(dt);

    if (player1.getRect().y <= 0) {
      player1.getRect().y = 170;
      scores.setScoreP1(scores.getScoreP1() + 1);
    }
    if (player2.getRect().y <= 0) {
      player2.getRect().y = 170;
      scores.setScoreP2(scores.getScoreP2() + 1);
    }
  }

  CloseWindow();
}
