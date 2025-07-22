#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>

using namespace std;

class Game {

public:
  // #### Constructor and Destructor ####

  /**
   * @brief Construct a new Game object
   *
   */
  Game(int dif);

  // #### Getters and Setters ####

  sf::RenderWindow &getRenderWindow();

  sf::Font &getFont();

  sf::Text &getScoreText();

  sf::RectangleShape &getQuitButton();

  sf::Text &getQuitButtonText();

  sf::Sprite &getLeftPaddle();

  sf::Sprite &getRightPaddle();

  sf::Sprite &getBall();

  sf::Texture &getLeftPaddleTexture();

  sf::Texture &getRightPaddleTexture();

  sf::Texture &getBallTexture();

  int getLeftScore() const;

  int getRightScore() const;

  void incrementLeftScore();

  void incrementRightScore();

  void setBallPosition(const sf::Vector2f &position);

  void resetBall();

  float getBallSpeed() const;

  void setBallSpeed(float speed);

  sf::Vector2f getBallVelocity() const;

  void setBallVelocity(const sf::Vector2f &velocity);

  void drawElem(sf::RenderWindow &window, sf::Sprite &leftPaddle,
                sf::Sprite &rightPaddle, sf::Sprite *ball, sf::Text &scoreText,
                sf::RectangleShape &quitButton, sf::Text &quitButtonText);

  sf::Texture createSolidTexture(const sf::Color &color, float width,
                                 float height);

  void displayCountdown(sf::RenderWindow &window, sf::Font &font,
                        sf::Sprite &leftPaddle, sf::Sprite &rightPaddle,
                        sf::Text &scoreText, sf::RectangleShape &quitButton,
                        sf::Text &quitButtonText);

  void run();

private:
  sf::Font font;

  sf::Text scoreText;

  sf::RectangleShape quitButton;
  sf::Text quitButtonText;

  sf::Sprite leftPaddle;
  sf::Sprite rightPaddle;
  sf::Sprite ball;

  sf::Texture leftPaddleTexture;
  sf::Texture rightPaddleTexture;
  sf::Texture ballTexture;

  Window window;

  int leftScore = 0;
  int rightScore = 0;

  float ballSpeed;

  sf::Vector2f ballVelocity;

  float dif;
};

#endif // GAME_HPP