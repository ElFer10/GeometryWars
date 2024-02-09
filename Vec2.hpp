#pragma once
#include <iostream>

class Vec2 {
public:
  float x, y;

  Vec2() : x(0), y(0) {}
  Vec2(float x, float y) : x(x), y(y) {}

  bool operator==(const Vec2 &rhs) const;
  bool operator!=(const Vec2 &rhs) const;

  Vec2 operator+(const Vec2 &rhs) const;
  Vec2 operator-(const Vec2 &rhs) const;
  Vec2 operator/(const float val) const;
  Vec2 operator*(const float val) const;

  void operator+=(const Vec2 &rhs);
  void operator-=(const Vec2 &rhs);
  void operator/=(const float val);
  void operator*=(const float val);

  float dist(const Vec2 &rhs) const;
  float getAngle(const Vec2 &target) const;
  void normalize();
  float length() const;
  Vec2 getVelocity(float speed, float angle) const;

  friend std::ostream &operator<<(std::ostream &o, const Vec2 &v);
};
