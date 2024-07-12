#include <iostream>
#include<utility>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

class DataHang {
private:
    string secretWord;
    string hint;

public:
    DataHang(const string& word, const string& hint) : secretWord(word), hint(hint) {}

    const string& getSecretWord() const {
        return secretWord;
    }

    const string& getHint() const {
        return hint;
    }
};

class abstractHang {
public:
    virtual void display() = 0;
};

class HangmanWord : public abstractHang {
private:
    vector<bool> guessedLetters;
    DataHang data;

public:
    HangmanWord(const string& secretWord, const string& hint) : data(secretWord, hint) {
        guessedLetters.resize(secretWord.length(), false);
    }

    bool guessed(char letter) {
        bool found = false;
        for (int i = 0; i < data.getSecretWord().length(); ++i) {
            if (tolower(data.getSecretWord()[i]) == tolower(letter)) {
                guessedLetters[i] = true;
                found = true;
            }
        }
        return found;
    }

    bool guessed() const {
        for (bool guessed : guessedLetters) {
            if (!guessed) return false;
        }
        return true;
    }

    void display() {
        for (int i = 0; i < data.getSecretWord().length(); ++i) {
            if (guessedLetters[i])
                cout << data.getSecretWord()[i] << " ";
            else
                cout << "_ ";
        }
        cout << endl;
    }
};


class Hangmanfigure {
public:
    static void drawfigure(int incorrectguesses) {
        cout << "Hangman Figure: " << endl;
        switch (incorrectguesses) {
        case 0:
            cout<<"no body parts"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            break;
        case 1:
            cout<<"head"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "  O   |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            break;
        case 2:
            cout<<"body"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "  O   |" << endl;
            cout << "  |   |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            break;
        case 3:
            cout<<"left-hand"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "  O   |" << endl;
            cout << " /|   |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            break;
        case 4:
        cout<<"right-hand"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "  O   |" << endl;
            cout << " /|\\  |" << endl;
            cout << "      |" << endl;
            cout << "      |" << endl;
            break;
        case 5:
        cout<<"left-leg"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "  O   |" << endl;
            cout << " /|\\  |" << endl;
            cout << " /    |" << endl;
            cout << "      |" << endl;
            break;
        case 6:
        cout<<"right-leg"<<endl;
            cout << "  +---+" << endl;
            cout << "  |   |" << endl;
            cout << "  O   |" << endl;
            cout << " /|\\  |" << endl;
            cout << " / \\  |" << endl;
            cout << "      |" << endl;
            break;
        }
    }
};

bool isValidInput(char ch) {
    return isalpha(ch);
}

pair<string, string> getRandomWordAndHintFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    vector<pair<string, string>> wordHintPairs;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string word, hint;
        if (getline(iss, word, ':') && getline(iss, hint)) {
            wordHintPairs.emplace_back(word, hint);
        }
    }

    file.close();

    srand(static_cast<unsigned>(time(nullptr)));
    int randomIndex = rand() % wordHintPairs.size();
    return wordHintPairs[randomIndex];
}

void displayMenu() {
    cout << "========== Hangman Game Menu ==========" << endl;
    cout << "1. Start a New Game" << endl;
    cout << "2. View Instructions" << endl;
    cout << "3. Exit" << endl;
    cout << "=======================================" << endl;
}


void startGame() {
    string filename = "wordlist.txt";
    pair<string, string> wordHintPair = getRandomWordAndHintFromFile(filename);
    string word = wordHintPair.first;
    string hint = wordHintPair.second;
    cout << "Hint: " << hint << endl;
    cout << "Secret word selected." << endl;

    HangmanWord hangmanword(word, hint);
    const int maxIncorrectGuesses = 6;
    int incorrectguesses = 0;

    while (incorrectguesses < maxIncorrectGuesses && !hangmanword.guessed()) {
        cout << "Secret Word: ";
        hangmanword.display();

        char guess;
        cout << "Enter a letter: ";
        cin >> guess;
        guess = tolower(guess);

        if (!isValidInput(guess)) {
            cout << "Invalid input! Please enter an alphabetical character." << endl;
            continue;
        }

        if (!hangmanword.guessed(guess)) {
            incorrectguesses++;
            cout << "Incorrect guess! You have " << maxIncorrectGuesses - incorrectguesses << " guesses left.\n";
        } else {
            cout << "Correct guess!" << endl;
        }

        Hangmanfigure::drawfigure(incorrectguesses);
    }

    if (hangmanword.guessed()) {
        cout << "Congratulations! You guessed the word: " << word << std::endl;
    } else {
        cout << "You've run out of guesses! The word was: " << word << std::endl;
    }
}

int main() {
    int choice;
    bool exitGame = false;

    do {
        displayMenu();
        cout << "Enter your choice: (1,2 or 3)";
        cin >> choice;

        switch (choice) {
        case 1:
            startGame();
            break;
        case 2:
            cout << "Instructions:" << endl;
            cout << "1. Guess letters to reveal the secret word." << endl;
            cout << "2. You have a maximum of 6 incorrect guesses." << endl;
            cout << "3. Good luck!" << endl;
            break;
        case 3:
            exitGame = true;
            break;
        default:
            cout << "Invalid choice! Please enter a valid option." << endl;
        }
    } while (!exitGame);

    cout << "Thanks for playing Hangman!" << endl;

    return 0;
}
