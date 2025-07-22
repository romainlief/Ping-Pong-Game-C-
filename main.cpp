#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <stdexcept>
#include <limits>

#include "src/game.hpp"

using namespace std;

float difChoice() {
    int choix;
    bool choixValide = false;
    float ballSpeed;
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
    return ballSpeed;
}

int main() {
    int dif = difChoice();
    Game game(dif);
    game.run();
    return 0;
}
