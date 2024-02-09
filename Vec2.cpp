#include "Vec2.hpp"
#include <cmath>
#include <iostream>

bool Vec2::operator==(const Vec2 &rhs) const {

  return (x == rhs.x) && (y == rhs.y);
}
bool Vec2::operator!=(const Vec2 &rhs) const {

  return (x != rhs.x) || (y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const {
  return Vec2(x + rhs.x, x + rhs.y);
}
Vec2 Vec2::operator-(const Vec2 &rhs) const {
  return Vec2(x - rhs.x, y - rhs.y);
}
Vec2 Vec2::operator/(const float val) const {
  if (val <= 0)
    return Vec2(0, 0);

  return Vec2(x / val, y / val);
}
Vec2 Vec2::operator*(const float val) const { return Vec2(val * x, val * y); }

void Vec2::operator+=(const Vec2 &rhs) {
  x += rhs.x;
  y += rhs.y;
}
void Vec2::operator-=(const Vec2 &rhs) {
  x -= rhs.x;
  y -= rhs.y;
}
void Vec2::operator/=(const float val) {
  if (val == 0) {
    x = 0;
    y = 0;
  }

  x /= val;
  y /= val;
}
void Vec2::operator*=(const float val) {
  x *= val;
  y *= val;
}

float Vec2::dist(const Vec2 &rhs) const {
  float dx = rhs.x - x;
  float dy = rhs.y - y;

  return sqrtf(dx * dx + dy * dy);
}

std::ostream &operator<<(std::ostream &o, const Vec2 &v) {
  return (o << "V(" << v.x << ", " << v.y << ")"
            << "\n");
}

float Vec2::getAngle(const Vec2 &target) const {

  return std::atan2(target.y - y, target.x - x);

}

void Vec2::normalize() { 
  x = x/length();
  y = y/length();
}

Vec2 Vec2::getVelocity(float speed,float angle) const
{
    return {speed *cosf(angle),speed * sinf(angle)} ;
}

float Vec2::length() const { return sqrtf(x * x + y * y); }
