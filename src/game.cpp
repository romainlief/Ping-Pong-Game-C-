#include "game.hpp"
#include "const.hpp"
#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <random>
#include <string>
#include <thread>

Game::Game(float dif) : leftScore(0), rightScore(0), window(), ballSpeed(dif) {
  if (!font.loadFromFile("res/BungeeTint-Regular.ttf")) {
    cerr << "Erreur de chargement de la police" << endl;
  }
  
  // Créer les textures et les assigner aux attributs de la classe
  leftPaddleTexture = createSolidTexture(firstPaddleColor, paddleWidth, paddleHeight);
  rightPaddleTexture = createSolidTexture(secondPaddleColor, paddleWidth, paddleHeight);
  ballTexture = createSolidTexture(ballColor, ballRadius * 2, ballRadius * 2);

  // Assigner les textures aux sprites
  leftPaddle.setTexture(leftPaddleTexture);
  rightPaddle.setTexture(rightPaddleTexture);
  ball.setTexture(ballTexture);

  // Définir les positions initiales des sprites
  leftPaddle.setPosition(SCREENWIDTH / 11, SCREENHEIGHT / 2 - paddleHeight / 2);
  rightPaddle.setPosition(SCREENWIDTH / 1.1, SCREENHEIGHT / 2 - paddleHeight / 2);
  ball.setPosition(SCREENWIDTH / 2, SCREENHEIGHT / 2);

  ballVelocity = sf::Vector2f(ballSpeed, ballSpeed);
  resetBall();

  scoreText.setFont(font);
  scoreText.setCharacterSize(24);
  scoreText.setString(to_string(leftScore) + " - " + to_string(rightScore));
  scoreText.setPosition(static_cast<float>(SCREENWIDTH) / 2.f -
                            scoreText.getLocalBounds().width / 2.f,
                        SCREENHEIGHT / 20.f -
                            scoreText.getLocalBounds().height / 2.f);

  quitButton.setSize(sf::Vector2f(100.f, 40.f));
  quitButton.setFillColor(sf::Color::Red);
  quitButton.setPosition(1100.f, 20.f);

  quitButtonText.setFont(font);
  quitButtonText.setCharacterSize(16);
  quitButtonText.setString("Quitter");
  quitButtonText.setPosition(1110.f, 30.f);
}

sf::Texture &Game::getRightPaddleTexture() { return rightPaddleTexture; }

sf::Texture &Game::getLeftPaddleTexture() { return leftPaddleTexture; }

sf::Texture &Game::getBallTexture() { return ballTexture; }

sf::RenderWindow &Game::getRenderWindow() { return window.getRenderWindow(); }

sf::Font &Game::getFont() { return font; }

sf::Text &Game::getScoreText() { return scoreText; }

sf::RectangleShape &Game::getQuitButton() { return quitButton; }

sf::Text &Game::getQuitButtonText() { return quitButtonText; }

sf::Sprite &Game::getLeftPaddle() { return leftPaddle; }

sf::Sprite &Game::getRightPaddle() { return rightPaddle; }

sf::Sprite &Game::getBall() { return ball; }

int Game::getLeftScore() const { return leftScore; }

void Game::incrementLeftScore() { leftScore++; }

int Game::getRightScore() const { return rightScore; }

void Game::incrementRightScore() { rightScore++; }

void Game::setBallPosition(const sf::Vector2f &position) {
  ball.setPosition(position);
}

// Fonction pour générer un nombre aléatoire dans une plage donnée
float randomFloat(float min, float max) {
  static random_device rd;
  static mt19937 gen(rd());
  uniform_real_distribution<> dis(min, max);
  return dis(gen);
}

void Game::resetBall() {
  // Position aléatoire de la balle au centre de l'écran
  float startX = randomFloat(550.f, 650.f);
  float startY = randomFloat(350.f, 450.f);
  ball.setPosition(startX, startY);

  // Vitesse aléatoire de la balle vers la gauche ou la droite
  float directionX = randomFloat(0, 1) > 0.5 ? ballSpeed : -ballSpeed;
  float directionY = randomFloat(0, 1) > 0.5 ? ballSpeed : -ballSpeed;
  ballVelocity = {directionX, directionY};
}

float Game::getBallSpeed() const { return ballSpeed; }

void Game::setBallSpeed(float speed) { ballSpeed = speed; }

sf::Vector2f Game::getBallVelocity() const { return ballVelocity; }

void Game::setBallVelocity(const sf::Vector2f &velocity) {
  ballVelocity = velocity;
}

void Game::drawElem(sf::RenderWindow &window, sf::Sprite &leftPaddle,
                    sf::Sprite &rightPaddle, sf::Sprite *ball,
                    sf::Text &scoreText, sf::RectangleShape &quitButton,
                    sf::Text &quitButtonText) {
  window.draw(leftPaddle);
  window.draw(rightPaddle);
  if (ball) {
    window.draw(*ball);
  }
  window.draw(scoreText);
  window.draw(quitButton);
  window.draw(quitButtonText);
}

sf::Texture Game::createSolidTexture(const sf::Color &color, float width,
                                     float height) {
  sf::Image image;
  image.create(width, height, color); // Crée une image de couleur solide

  sf::Texture texture;
  texture.loadFromImage(image); // Charge l'image dans une texture
  return texture;
}

void Game::displayCountdown(sf::RenderWindow &window, sf::Font &font,
                            sf::Sprite &leftPaddle, sf::Sprite &rightPaddle,
                            sf::Text &scoreText, sf::RectangleShape &quitButton,
                            sf::Text &quitButtonText) {
  sf::Text countdownText;
  countdownText.setFont(font);
  countdownText.setCharacterSize(48);
  countdownText.setFillColor(sf::Color::White);
  countdownText.setPosition(570.f, 350.f);

  for (int i = 3; i > 0; --i) {
    countdownText.setString(std::to_string(i));
    window.clear(sf::Color::Black);
    drawElem(window, leftPaddle, rightPaddle, nullptr, scoreText, quitButton,
             quitButtonText);
    window.draw(countdownText);
    window.display();
    this_thread::sleep_for(chrono::seconds(1));
  }
}

void Game::run() {
  while (window.getRenderWindow().isOpen()) {
    sf::Event event{};
    while (window.getRenderWindow().pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.getRenderWindow().close();
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          sf::Vector2i mousePos = sf::Mouse::getPosition(window.getRenderWindow());
          if (quitButton.getGlobalBounds().contains(
                  static_cast<float>(mousePos.x),
                  static_cast<float>(mousePos.y))) {
            window.getRenderWindow().close();
          }
        }
      }
    }

    window.getRenderWindow().clear(sf::Color::Black);
    drawElem(window.getRenderWindow(), leftPaddle, rightPaddle, &ball, scoreText, quitButton,
             quitButtonText);
    window.getRenderWindow().display();

    // Afficher le compte à rebours
    displayCountdown(window.getRenderWindow(), font, leftPaddle, rightPaddle, scoreText,
                     quitButton, quitButtonText);
    break;
  }
  sf::Clock clock;
  while (window.getRenderWindow().isOpen() && leftScore < maxScore &&
         rightScore < maxScore) {
    sf::Time deltaTime = clock.restart();

    // Gestion des événements
    sf::Event event{};
    while (window.getRenderWindow().pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.getRenderWindow().close();
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          sf::Vector2i mousePos =
              sf::Mouse::getPosition(window.getRenderWindow());
          if (quitButton.getGlobalBounds().contains(
                  static_cast<float>(mousePos.x),
                  static_cast<float>(mousePos.y))) {
            window.getRenderWindow().close();
          }
        }
      }
    }

    // Déplacement des raquettes
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) &&
        leftPaddle.getPosition().y > 0)
      leftPaddle.move(0.f, -paddleSpeed * deltaTime.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
        leftPaddle.getPosition().y + paddleHeight < SCREENHEIGHT)
      leftPaddle.move(0.f, paddleSpeed * deltaTime.asSeconds());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        rightPaddle.getPosition().y > 0)
      rightPaddle.move(0.f, -paddleSpeed * deltaTime.asSeconds());
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
        rightPaddle.getPosition().y + paddleHeight < SCREENHEIGHT)
      rightPaddle.move(0.f, paddleSpeed * deltaTime.asSeconds());

    // Mise à jour de la position de la balle
    ball.move(ballVelocity * deltaTime.asSeconds());

    // Collision avec les murs (haut/bas)
    if (ball.getPosition().y <= 0 ||
        ball.getPosition().y + ball.getGlobalBounds().height >= SCREENHEIGHT) {
      ballVelocity.y = -ballVelocity.y;
    }

    // Collision avec les raquettes
    if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds())) {
      ballVelocity.x = abs(ballVelocity.x);
    }
    if (ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds())) {
      ballVelocity.x = -abs(ballVelocity.x);
    }

    // Gestion des scores et remise à zéro de la balle
    if (ball.getPosition().x <= 0) {
      rightScore++;
      resetBall();
    } else if (ball.getPosition().x + ball.getGlobalBounds().width >=
               SCREENWIDTH) {
      leftScore++;
      resetBall();
    }

    // Affichage du score
    scoreText.setString(to_string(leftScore) + " - " + to_string(rightScore));
    scoreText.setPosition(static_cast<float>(SCREENWIDTH) / 2.f -
                              scoreText.getLocalBounds().width / 2.f,
                          20.f);
    if (leftScore > rightScore) {
      scoreText.setFillColor(sf::Color::Green);
    } else if (leftScore < rightScore) {
      scoreText.setFillColor(sf::Color::Magenta);
    } else {
      scoreText.setFillColor(sf::Color::White);
    }

    window.getRenderWindow().clear(sf::Color::Black);
    drawElem(window.getRenderWindow(), leftPaddle, rightPaddle, &ball,
             scoreText, quitButton, quitButtonText);
    window.getRenderWindow().display();
  }
}