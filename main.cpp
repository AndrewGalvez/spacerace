#include "raylib.h"
#include "utils.hpp"
#include <string>
#include <vector>

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
      if (rect.y + rect.height < 200)
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

class Asteroid {
public:
  Rectangle rect = {0, 0, 0, 0};
  Color clr = WHITE;
  int dir = 1;
  float speed = 25;

  void draw() { DrawRectangleRec(rect, clr); }

  void update(float dt) { rect.x += dir * (speed * dt); }

  Asteroid(Rectangle rect, int dir, float speed)
      : rect(rect), dir(dir), speed(speed) {}
};

int main() {
  Player player1 =
      Player((Rectangle){50 - 10, 170, 8, 24}, (KeyboardKey[2]){KEY_W, KEY_S});
  Player player2 =
      Player((Rectangle){52, 170, 8, 24}, (KeyboardKey[2]){KEY_UP, KEY_DOWN});

  ScoreManager scores;

  std::vector<Asteroid> asteroids;

  InitWindow(800, 600, "Space Race");

  SetTargetFPS(60);

  SetWindowState(FLAG_WINDOW_RESIZABLE);

  RenderTexture2D target = LoadRenderTexture(100, 200);

  int asteroidSpawnIntervalBase = 100;
  int asteroidSpawnInterval = asteroidSpawnIntervalBase;
  int asteroidSpawnIntervalMin = 25;
  int minAsteroidSize = 10;
  int maxAsteroidSize = 20;

  int frame = 0;

  InitAudioDevice();

  Sound death = LoadSound("death.wav");
  Sound explosion = LoadSound("explosion.wav");

  while (!WindowShouldClose()) {
    BeginTextureMode(target);
    ClearBackground(BLACK);
    player1.draw();
    player2.draw();

    std::string p1score_text = std::to_string(scores.getScoreP1());
    std::string p2score_text = std::to_string(scores.getScoreP2());

    DrawText(p1score_text.c_str(),
             35 - (MeasureText(p1score_text.c_str(), 12) / 2), 170, 12,
             scores.getScoreP1() >= scores.getScoreP2() ? GREEN : RED);
    DrawText(p2score_text.c_str(),
             65 - (MeasureText(p2score_text.c_str(), 12) / 2), 170, 12,
             scores.getScoreP1() <= scores.getScoreP2() ? GREEN : RED);

    for (Asteroid &a : asteroids) {
      a.draw();
    }

    EndTextureMode();

    BeginDrawing();
    ClearBackground(WHITE);
    DrawRenderTexture(target, 150, 200);
    EndDrawing();

    float dt = GetFrameTime();
    frame++;

    if (frame >= asteroidSpawnInterval) {
      frame = 0;
      asteroidSpawnInterval -=
          asteroidSpawnInterval > asteroidSpawnIntervalMin ? 1 : 0;

      int dirs[2] = {-1, 1};
      int dir = dirs[GetRandomValue(0, 1)];

      float speed = GetRandomValue(25, 30);

      const int x = dir != -1 ? -100 : 300;
      const int y = GetRandomValue(0, 150);

      const int w = GetRandomValue(minAsteroidSize, maxAsteroidSize);
      const int h = GetRandomValue(minAsteroidSize, maxAsteroidSize);

      asteroids.push_back(
          Asteroid({static_cast<float>(x), static_cast<float>(y),
                    static_cast<float>(w), static_cast<float>(h)},
                   dir, speed));
    }

    for (Asteroid &a : asteroids) {
      a.update(dt);

      if (CheckCollisionRecs(a.rect, player1.getRect())) {
        player1.getRect().y = 170;
        PlaySound(death);
        if (asteroidSpawnInterval == asteroidSpawnIntervalMin)
          asteroidSpawnInterval = asteroidSpawnIntervalBase;
      }
      if (CheckCollisionRecs(a.rect, player2.getRect())) {
        player2.getRect().y = 170;
        PlaySound(death);
        if (asteroidSpawnInterval == asteroidSpawnIntervalMin)
          asteroidSpawnInterval = asteroidSpawnIntervalBase;
      }
    }

    player1.update(dt);
    player2.update(dt);

    if (player1.getRect().y <= 0) {
      player1.getRect().y = 170;
      scores.setScoreP1(scores.getScoreP1() + 1);
      PlaySound(explosion);
    }
    if (player2.getRect().y <= 0) {
      player2.getRect().y = 170;
      scores.setScoreP2(scores.getScoreP2() + 1);
      PlaySound(explosion);
    }
  }

  UnloadSound(death);
  CloseAudioDevice();
  CloseWindow();
}
