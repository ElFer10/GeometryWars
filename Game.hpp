#pragma once
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"

#include <SFML/Graphics.hpp>
#include <memory>

struct PlayerConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
  float S;
};
struct EnemyConfig {
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
  float SMIN, SMAX;
};
struct BulletConfig {
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S;
};

class Game {
  sf::RenderWindow m_window;
  bool m_paused = false;
  bool m_running = true;
  EntityManager m_entities;
  sf::Font m_font;
  sf::Text m_text;
  PlayerConfig m_playerConfig;
  EnemyConfig m_enemyConfig;
  BulletConfig m_bulletConfig;
  int m_score = 0;
  int m_spawnInterval = 300; // 5 segundos ¿?
  int m_lastEnemySpawnTime = 0;
  int m_currentFrame = 0;
  sf::Clock m_clock;

  std::shared_ptr<Entity> m_player;

  void init(const std::string &config);
  void setPaused(bool paused);

  void spawnPlayer();
  void spawnEnemy();
  void spawnSmallEnemies(std::shared_ptr<Entity> entity);
  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &mousePos);
  void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

  // Systems
  void sMovement();
  void sLifespan();
  void sUserInput();
  void sRender();
  void sEnemySpawner();
  void sCollision();

public:
  Game(const std::string &config);
  void run();
};
