#pragma once

#include "SFML/Graphics/CircleShape.hpp"
#include "Vec2.hpp"
#include <SFML/Graphics.hpp>
// #include "Common.h"

class CTransform {
public:
  Vec2 pos = {0.0f, 0.0f};
  Vec2 velocity = {0.0f, 0.0f};
  float angle = 0.0f;

  CTransform(const Vec2 &p, const Vec2 &v, float a)
      : pos(p), velocity(v), angle(a){};
};

class CShape {
public:
  sf::CircleShape circle;

  CShape(float radius, int point, const sf::Color &fill,
         const sf::Color &outline, float thickness)
      : circle(radius, point) {
    circle.setFillColor(fill);
    circle.setOutlineColor(outline);
    circle.setOutlineThickness(thickness);
    circle.setOrigin(radius, radius);
  }
};

class CCollision {
public:
  float radius = 0.0f;
  CCollision(float r) : radius(r){};
};

class CScore {
public:
  int score = 0;
  CScore(int s) : score(s){};
};

class CLifespan {
public:
  int lifespan = 0;
  int frameCreated = 0;

  CLifespan(int duration, int created)
      : lifespan(duration), frameCreated(created){};
};

class CInput {
public:
  bool up = false;
  bool left = false;
  bool right = false;
  bool down = false;
  bool shoot = false;
  bool specialWeapon = false;

  CInput(){};
};
