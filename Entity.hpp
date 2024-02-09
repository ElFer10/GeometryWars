#pragma once

#include "Components.hpp"
#include <memory>
#include <string>

class Entity {
  bool m_active = true;
  size_t m_id = 0;
  std::string m_tag = "Default";

public:
  // Component pointers
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CShape> cShape;
  std::shared_ptr<CCollision> cCollision;
  std::shared_ptr<CInput> cInput;
  std::shared_ptr<CScore> cScore;
  std::shared_ptr<CLifespan> cLifespan;

  Entity(const size_t id, const std::string &tag) : m_id(id), m_tag(tag) {}

  bool isActive() const { return m_active; }
  const std::string &tag() const { return m_tag; }
  const size_t id() const { return m_id; }

  void destroy();
};
