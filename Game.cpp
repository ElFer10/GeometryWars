#include "Game.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/WindowStyle.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

template <class T> T random(T min, T max) {
  return (int)rand() % (int)max + min;
}

Game::Game(const std::string &config) { init(config); }

void Game::init(const std::string &config) {

  int winW, winH, fr, maxSc;
  int r, g, b;
  int fontSize;
  std::string fontName;

  winW = 300;
  winH = 300;

  std::string linea, tipo;
  std::ifstream configFile(config);

  if (configFile.is_open()) {
    while (getline(configFile, linea)) {
      std::stringstream ss(linea);
      ss >> tipo;
      if (tipo == "Window") {
        ss >> winW >> winH >> fr >> maxSc;
      } else if (tipo == "Font") {
        ss >> fontName >> fontSize >> r >> g >> b;
      } else if (tipo == "Player") {
        ss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.S >>
            m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >>
            m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >>
            m_playerConfig.OT >> m_playerConfig.V;
      } else if (tipo == "Enemy") {
        ss >> m_enemyConfig.SR >> m_enemyConfig.CR >> m_enemyConfig.SMIN >>
            m_enemyConfig.SMAX >> m_enemyConfig.OR >> m_enemyConfig.OG >>
            m_enemyConfig.OB >> m_enemyConfig.OT >> m_enemyConfig.VMIN >>
            m_enemyConfig.VMAX >> m_enemyConfig.L >> m_enemyConfig.SI;
      } else if (tipo == "Bullet") {
        ss >> m_bulletConfig.SR >> m_bulletConfig.CR >> m_bulletConfig.S >>
            m_bulletConfig.FR >> m_bulletConfig.FG >> m_bulletConfig.FB >>
            m_bulletConfig.OR >> m_bulletConfig.OG >> m_bulletConfig.OB >>
            m_bulletConfig.OT >> m_bulletConfig.V >> m_bulletConfig.L;
      }
    }
  } else {
    std::cerr << "Error al abrir la configuracion" << std::endl;
  }

  if (!m_font.loadFromFile(fontName)) {
    std::cerr << "No se cargó la fuente" << std::endl;
  }
  m_text.setFont(m_font);
  m_text.setCharacterSize(fontSize);
  m_text.setFillColor(sf::Color(r, g, b));

  m_text.setString("Score: " + std::to_string(m_score));

  auto form = maxSc == 1 ? sf::Style::Fullscreen : sf::Style::Default;

  m_window.create(sf::VideoMode(winW, winH), "GeometryWars", form);
  m_window.setFramerateLimit(fr);

  spawnPlayer();
}

void Game::run() {

  while (m_running) {
    m_entities.update();

    if (!m_paused) {
      sEnemySpawner();
      sMovement();
      sLifespan();
      sCollision();
    }

    sUserInput();
    sRender();

    m_text.setString("Score: " + std::to_string(m_score));
    m_currentFrame++;
  }
}

void Game::setPaused(bool paused) { m_paused = paused; }

// Respawn the player in the middle of the screen.
void Game::spawnPlayer() {
  auto entity = m_entities.addEntity("player");

  entity->cTransform = std::make_shared<CTransform>(
      Vec2(m_window.getSize().x / 2, m_window.getSize().y / 2),
      Vec2(m_playerConfig.S, m_playerConfig.S), 0.0f);

  entity->cInput = std::make_shared<CInput>();

  entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

  entity->cShape = std::make_shared<CShape>(
      m_playerConfig.SR, m_playerConfig.V,
      sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
      sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
      m_playerConfig.OT);

  m_player = entity;
}

void Game::spawnEnemy() {

  std::srand(std::time(NULL));
  auto enemy = m_entities.addEntity("enemy");

  float vel = std::rand() % (int)m_enemyConfig.SMAX + m_enemyConfig.SMIN;
  int px = (std::rand() % ((int)m_window.getSize().x - m_enemyConfig.SR) +
            m_enemyConfig.SR);
  int py = (std::rand() % ((int)m_window.getSize().y - m_enemyConfig.SR) +
            m_enemyConfig.SR);

  enemy->cTransform =
      std::make_shared<CTransform>(Vec2(px, py), Vec2(vel, vel), 0.0f);

  int vertices = std::rand() % m_enemyConfig.VMAX + m_enemyConfig.VMIN;

  enemy->cScore = std::make_shared<CScore>(vertices * 100);
  enemy->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);
  enemy->cShape = std::make_shared<CShape>(
      m_enemyConfig.SR, vertices,
      sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255),
      sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
      m_enemyConfig.OT);

  m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity) {

  size_t vertices = entity->cShape->circle.getPointCount();

  Vec2 parentPosition = entity->cTransform->pos;
  Vec2 normalizedParentPosition = parentPosition;
  normalizedParentPosition.normalize();

  sf::Color parentColor = entity->cShape->circle.getFillColor();
  sf::Color parentBorder = entity->cShape->circle.getOutlineColor();
  float parentThicknes = entity->cShape->circle.getOutlineThickness();

  float smallEnemyRadius = static_cast<float>(m_enemyConfig.SR) / 2;
  float smallEnemyCollision = static_cast<float>(m_enemyConfig.CR) / 2;
  float angulo = 0;

  for (int i = 0; i < vertices; i++) {
    auto smallEnemy = m_entities.addEntity("smallenemy");

    smallEnemy->cShape = std::make_shared<CShape>(
        smallEnemyRadius, vertices, parentColor, parentBorder, parentThicknes);
    smallEnemy->cScore = std::make_shared<CScore>(entity->cScore->score * 2);
    smallEnemy->cCollision = std::make_shared<CCollision>(smallEnemyCollision);
    smallEnemy->cLifespan =
        std::make_shared<CLifespan>(m_enemyConfig.L, m_currentFrame);

    float rads = angulo * M_PI / 180;

    Vec2 velocity = {std::cos(rads) * normalizedParentPosition.x +
                         std::sin(rads) * normalizedParentPosition.y,
                     std::sin(rads) * normalizedParentPosition.x -
                         std::cos(rads) * normalizedParentPosition.y};

    velocity.normalize();
    Vec2 nuevaVel = {velocity.x * entity->cTransform->velocity.x,
                     velocity.y * entity->cTransform->velocity.y};

    smallEnemy->cTransform =
        std::make_shared<CTransform>(parentPosition, nuevaVel, 0);
    angulo += 360.0f / vertices;
  }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 &target) {

  auto bullet = m_entities.addEntity("bullet");
  bullet->cShape = std::make_shared<CShape>(
      m_bulletConfig.SR, m_bulletConfig.V,
      sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB),
      sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB),
      m_bulletConfig.OT);

  bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

  bullet->cLifespan =
      std::make_shared<CLifespan>(m_bulletConfig.L, m_currentFrame);

  Vec2 distancia{target.x - entity->cTransform->pos.x,
                 target.y - entity->cTransform->pos.y};
  float angulo = std::atan2f(distancia.y, distancia.x);

  Vec2 velocity{m_bulletConfig.S * std::cos(angulo),
                m_bulletConfig.S * std::sin(angulo)};

  bullet->cTransform =
      std::make_shared<CTransform>(m_player->cTransform->pos, velocity, 0);
}

void Game::sMovement() {

  for (auto e : m_entities.getEntities()) {
    if (e->tag() == "player") {
      sf::Vector2f pos = m_player->cShape->circle.getPosition();
      sf::Vector2 win = m_window.getSize();

      if (pos.y > m_playerConfig.SR + (m_playerConfig.OT)) {
        if (m_player->cInput->up) {
          m_player->cTransform->pos.y -= m_player->cTransform->velocity.y;
        }
      }

      if (pos.y < win.y - m_playerConfig.SR - m_playerConfig.OT) {
        if (m_player->cInput->down) {
          m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
        }
      }

      if (pos.x > m_playerConfig.SR + (m_playerConfig.OT)) {
        if (m_player->cInput->left) {
          m_player->cTransform->pos.x -= m_player->cTransform->velocity.x;
        }
      }
      if (pos.x < win.x - m_playerConfig.SR - m_playerConfig.OT) {
        if (m_player->cInput->right) {
          m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
        }
      }
    } else {

      if (e->tag() == "enemy") {
        if (e->cTransform->pos.x < 0 ||
            e->cTransform->pos.x > m_window.getSize().x) {
          e->cTransform->velocity.x *= -1;
        }

        if (e->cTransform->pos.y < 0 ||
            e->cTransform->pos.y > m_window.getSize().y) {
          e->cTransform->velocity.y *= -1;
        }
      }

      e->cTransform->pos += e->cTransform->velocity;
    }
  }
}

void Game::sLifespan() {
  for (auto e : m_entities.getEntities()) {
    if (!e->cLifespan) {
      continue;
    }
    auto color = e->cShape->circle.getFillColor();
    auto colorBorder = e->cShape->circle.getOutlineColor();
    int alpha = 255;
    int remaining = m_currentFrame - e->cLifespan->frameCreated;

    if (remaining <= e->cLifespan->lifespan) {
      alpha =
          (int)(255 - ((float)remaining / (float)e->cLifespan->lifespan) * 255);

      e->cShape->circle.setFillColor(
          {color.r, color.g, color.b, (uint8_t)alpha});
      e->cShape->circle.setOutlineColor(
          {colorBorder.r, colorBorder.g, colorBorder.b, (uint8_t)alpha});
    }
    if (remaining >= e->cLifespan->lifespan) {
      e->destroy();
    }
  }
}

void Game::sCollision() {
  for (auto e : m_entities.getEntities("enemy")) {
    if (m_player) {
      const float d = m_player->cTransform->pos.dist(e->cTransform->pos);

      if (d < m_player->cCollision->radius + e->cCollision->radius) {
        m_player->destroy();
        m_score /= 2;
        e->destroy();
        spawnPlayer();
      }
    }
    for (auto b : m_entities.getEntities("bullet")) {
      const float d = b->cTransform->pos.dist(e->cTransform->pos);

      if (d < b->cCollision->radius + e->cCollision->radius) {
        spawnSmallEnemies(e);
        m_score += m_enemyConfig.SR;

        e->destroy();
        b->destroy();
      }
    }
  }

  for (auto bullet : m_entities.getEntities("bullet")) {
    for (auto smallEnemy : m_entities.getEntities("smallenemy")) {
      const float d = bullet->cTransform->pos.dist(smallEnemy->cTransform->pos);

      if (d < bullet->cCollision->radius + smallEnemy->cCollision->radius) {
        m_score += smallEnemy->cScore->score;
        bullet->destroy();
        smallEnemy->destroy();
      }
    }
  }
}

void Game::sEnemySpawner() {

  if (m_currentFrame - m_lastEnemySpawnTime > m_spawnInterval) {
    spawnEnemy();
  }
}

void Game::sRender() {

  m_window.clear();

  for (auto e : m_entities.getEntities()) {
    e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
    e->cTransform->angle += 1.f;
    e->cShape->circle.setRotation(e->cTransform->angle);
    m_window.draw(e->cShape->circle);
  }

  m_window.draw(m_text);
  m_window.display();
}

void Game::sUserInput() {

  sf::Event event;
  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      m_running = false;
    }

    // Este evento arranca cuando se presiona una tecla
    if (event.type == sf::Event::KeyPressed) {

      switch (event.key.code) {
      case sf::Keyboard::W: {
        m_player->cInput->up = true;
      }; break;
      case sf::Keyboard::S: {
        m_player->cInput->down = true;
      }; break;
      case sf::Keyboard::A: {
        m_player->cInput->left = true;
      }; break;
      case sf::Keyboard::D: {
        m_player->cInput->right = true;
      }; break;
      case sf::Keyboard::P: {
        setPaused(!m_paused);
        std::cout << "Paused: " << m_paused << std::endl;
      } break;
      case sf::Keyboard::Escape: {
        m_running = false;
      } break;
      }
    }

    // Este evento arranca cuando se suelta una tecla
    if (event.type == sf::Event::KeyReleased) {

      switch (event.key.code) {
      case sf::Keyboard::W: {
        m_player->cInput->up = false;
      }; break;
      case sf::Keyboard::S: {
        m_player->cInput->down = false;
      }; break;
      case sf::Keyboard::A: {
        m_player->cInput->left = false;
      }; break;
      case sf::Keyboard::D: {
        m_player->cInput->right = false;
      }; break;
      }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
      if (event.mouseButton.button == sf::Mouse::Left) {
        m_player->cInput->shoot = true;
        spawnBullet(
            m_player,
            Vec2(static_cast<float>(sf::Mouse::getPosition(m_window).x),
                 static_cast<float>(sf::Mouse::getPosition(m_window).y)));
      }
      if (event.mouseButton.button == sf::Mouse::Right) {
        std::cout << "Mouse right pressed at: " << event.mouseButton.x << ", "
                  << event.mouseButton.y << std::endl;
        // TODO: spawn super power
      }
    }
  }
}
