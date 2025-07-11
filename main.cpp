#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <random>

#include "src/const.cpp"

using namespace std;;

float ballSpeed;

// Fonction pour générer un nombre aléatoire dans une plage donnée
float randomFloat(float min, float max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

void resetBall(sf::Sprite &ball, sf::Vector2f &ballVelocity) {
    // Position aléatoire de la balle au centre de l'écran
    float startX = randomFloat(550.f, 650.f);
    float startY = randomFloat(350.f, 450.f);
    ball.setPosition(startX, startY);

    // Vitesse aléatoire de la balle vers la gauche ou la droite
    float directionX = randomFloat(0, 1) > 0.5 ? ballSpeed : -ballSpeed;
    float directionY = randomFloat(0, 1) > 0.5 ? ballSpeed : -ballSpeed;
    ballVelocity = {directionX, directionY};
}

void choix_difficulte() {
    int choix;
    bool choixValide = false;
    while (!choixValide) {
        try {
            cout << "Choisissez la difficulté : " << endl;
            cout << "1. Facile" << endl;
            cout << "2. Moyen" << endl;
            cout << "3. Difficile" << endl;
            cin >> choix;

            if (cin.fail()) {
                throw invalid_argument("Entrée invalide");
            }
            if (choix == 1) {
                ballSpeed = 200.f;
                choixValide = true;
            } else if (choix == 2) {
                ballSpeed = 300.f;
                choixValide = true;
            } else if (choix == 3) {
                ballSpeed = 400.f;
                choixValide = true;
            } else {
                throw invalid_argument("Choix invalide");
            }
        } catch (const invalid_argument &e) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << e.what() << ", veuillez réessayer." << endl;
        }
    }
}

void dessiner_les_elements(sf::RenderWindow &window, sf::Sprite &leftPaddle, sf::Sprite &rightPaddle, sf::Sprite *ball,
                           sf::Text &scoreText, sf::RectangleShape &quitButton, sf::Text &quitButtonText) {
    window.draw(leftPaddle);
    window.draw(rightPaddle);
    if (ball != nullptr) {
        window.draw(*ball);
    }
    window.draw(scoreText);
    window.draw(quitButton);
    window.draw(quitButtonText);
}

sf::Texture createSolidTexture(sf::Color color, float width, float height) {
    sf::Image image;
    image.create(width, height, color); // Crée une image de couleur solide

    sf::Texture texture;
    texture.loadFromImage(image); // Charge l'image dans une texture
    return texture;
}

void afficher_compte_a_rebours(sf::RenderWindow &window, sf::Font &font, sf::Sprite &leftPaddle,
                               sf::Sprite &rightPaddle, sf::Text &scoreText, sf::RectangleShape &quitButton,
                               sf::Text &quitButtonText) {
    sf::Text countdownText;
    countdownText.setFont(font);
    countdownText.setCharacterSize(48);
    countdownText.setFillColor(sf::Color::White);
    countdownText.setPosition(570.f, 350.f);

    for (int i = 3; i > 0; --i) {
        countdownText.setString(to_string(i));
        window.clear(sf::Color::Black);
        dessiner_les_elements(window, leftPaddle, rightPaddle, nullptr, scoreText, quitButton
                              , quitButtonText);
        window.draw(countdownText);
        window.display();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    choix_difficulte();
    // Créer une fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "Ping Pong Game");
    window.setFramerateLimit(60);


    // Créer des textures simples avec des couleurs différentes pour les paddles
    sf::Texture leftPaddleTexture = createSolidTexture(sf::Color::Green, paddleWidth, paddleHeight);
    sf::Texture rightPaddleTexture = createSolidTexture(sf::Color::Magenta, paddleWidth
                                                        , paddleHeight);
    sf::Texture ballTexture = createSolidTexture(sf::Color::Red, ballRadius * 2, ballRadius * 2);

    // Créer les sprites
    sf::Sprite leftPaddle(leftPaddleTexture);
    sf::Sprite rightPaddle(rightPaddleTexture);
    sf::Sprite ball(ballTexture);

    // Définir les positions initiales des sprites
    leftPaddle.setPosition(SCREENWIDTH / 11, SCREENHEIGHT / 2 - paddleHeight / 2);
    rightPaddle.setPosition(SCREENWIDTH / 1.1, SCREENHEIGHT / 2 - paddleHeight / 2);
    ball.setPosition(SCREENWIDTH / 2, SCREENHEIGHT / 2);

    sf::Vector2f ballVelocity(ballSpeed, ballSpeed);
    resetBall(ball, ballVelocity);

    // Scores
    int leftScore = 0;
    int rightScore = 0;
    sf::Font font;
    if (!font.loadFromFile("res/BungeeTint-Regular.ttf")) {
        cerr << "Erreur de chargement de la police" << endl;
        return -1;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setString(to_string(leftScore) + " - " + to_string(rightScore));
    scoreText.setPosition(static_cast<float>(SCREENWIDTH) / 2.f - scoreText.getLocalBounds().width / 2.f, SCREENHEIGHT / 20.f - scoreText.getLocalBounds().height / 2.f);

    // Créer le bouton Quitter
    sf::RectangleShape quitButton(sf::Vector2f(100.f, 40.f));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(1100.f, 20.f);

    sf::Text quitButtonText;
    quitButtonText.setFont(font);
    quitButtonText.setCharacterSize(16);
    quitButtonText.setString("Quitter");
    quitButtonText.setPosition(1110.f, 30.f);

    // Afficher la fenêtre et gérer les événements avant le compte à rebours
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (quitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x)
                                                              , static_cast<float>(mousePos.y))) {
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color::Black);
        dessiner_les_elements(window, leftPaddle, rightPaddle, &ball, scoreText, quitButton
                              , quitButtonText);
        window.display();

        // Afficher le compte à rebours
        afficher_compte_a_rebours(window, font, leftPaddle, rightPaddle, scoreText, quitButton
                                  , quitButtonText);
        break;
    }

    // Boucle principale du jeu
    sf::Clock clock;
    while (window.isOpen() && leftScore < 10 && rightScore < 10) {
        sf::Time deltaTime = clock.restart();

        // Gestion des événements
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (quitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x)
                                                              , static_cast<float>(mousePos.y))) {
                        window.close();
                    }
                }
            }
        }

        // Déplacement des raquettes
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && leftPaddle.getPosition().y > 0)
            leftPaddle.move(0.f, -paddleSpeed * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && leftPaddle.getPosition().y + paddleHeight < 800)
            leftPaddle.move(0.f, paddleSpeed * deltaTime.asSeconds());

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rightPaddle.getPosition().y > 0)
            rightPaddle.move(0.f, -paddleSpeed * deltaTime.asSeconds());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && rightPaddle.getPosition().y + paddleHeight < 800)
            rightPaddle.move(0.f, paddleSpeed * deltaTime.asSeconds());

        // Mise à jour de la position de la balle
        ball.move(ballVelocity * deltaTime.asSeconds());

        // Collision avec les murs (haut/bas)
        if (ball.getPosition().y <= 0 || ball.getPosition().y + ball.getGlobalBounds().height >= 800) {
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
            resetBall(ball, ballVelocity);
        } else if (ball.getPosition().x + ball.getGlobalBounds().width >= 1200) {
            leftScore++;
            resetBall(ball, ballVelocity);
        }

        // Affichage du score
        scoreText.setString(to_string(leftScore) + " - " + to_string(rightScore));
        scoreText.setPosition(static_cast<float>(SCREENWIDTH) / 2.f - scoreText.getLocalBounds().width / 2.f, 20.f);
        if (leftScore > rightScore) {
            scoreText.setFillColor(sf::Color::Green);
        } else if (leftScore < rightScore) {
            scoreText.setFillColor(sf::Color::Magenta);
        } else {
            scoreText.setFillColor(sf::Color::White);
        }

        // Effacer l'écran
        window.clear(sf::Color::Black);

        dessiner_les_elements(window, leftPaddle, rightPaddle, &ball, scoreText, quitButton, quitButtonText);

        // Afficher sur l'écran
        window.display();
    }
    return 0;
}
