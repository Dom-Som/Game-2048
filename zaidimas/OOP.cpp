#include <iostream>
#include <iomanip>
#include <ctime>
#include <thread>
#include <vector>
#include <map>

using namespace std;

//Lentos kalse
class Board
{
public:
    vector<vector<int>> grid; // Lenta reprezentuojantis 2D vektorius
    int score; // Taskai
    bool gameOver; // Indikatoriu kai zaidimas baigiasi
    bool victory; // Indikatorius kai surenkamas laukelis su 2048

    // Konstruktoriu inicializuoja lenta
    Board() : grid(4, vector<int>(4, 0)), score(0), gameOver(false), victory(false)
    {
        addNewNumber(); // Prideda du skaitmenis lentoje
        addNewNumber();
    }

    // Metodas pridedanti skaiciu atsitiktineje vietoje
    void addNewNumber()
    {
        vector<pair<int, int>> emptyCells;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (grid[i][j] == 0)
                {
                    emptyCells.push_back(make_pair(i, j));
                }
            }
        }

        if (!emptyCells.empty())
        {
            int idx = rand() % emptyCells.size();
            int ei = emptyCells[idx].first;
            int ej = emptyCells[idx].second;

            int newValue = 2;
            if (rand() % 4 == 0)
            {
                newValue = 4;
            }

            grid[ei][ej] = newValue;
        }
    }

    // Metodas tikrinantis ar zaidimas laimetas
    void checkVictory()
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (grid[i][j] == 2048)
                {
                    victory = true;
                    return;
                }
            }
        }
    }

    // Metodas tikrinantis ar ejimas galimas
    bool movePossible(int i, int j, int nextI, int nextJ) const
    {
        if (nextI < 0 || nextJ < 0 || nextI >= 4 || nextJ >= 4)
            return false;
        if (grid[nextI][nextJ] == 0 || grid[nextI][nextJ] == grid[i][j])
            return true;
        return false;
    }

    // Metodas judinantis laukelius pasirinkta kryptimi
    void move(int direction)
    {
        const int dI[] = { 1, 0, -1, 0 };
        const int dJ[] = { 0, 1, 0, -1 };

        bool moved = false;
        int startI, startJ, stepI, stepJ;

        // Nustato pradine pozicija
        if (direction == 0) // Vertikalus judejimas
        {
            startI = 3;
            stepI = -1;
        }
        else
        {
            startI = 0;
            stepI = 1;
        }

        if (direction == 1) // Horizontalus judejimas
        {
            startJ = 3;
            stepJ = -1;
        }
        else
        {
            startJ = 0;
            stepJ = 1;
        }

        // Padaromas ejimas
        for (int i = startI; i >= 0 && i < 4; i += stepI)
        {
            for (int j = startJ; j >= 0 && j < 4; j += stepJ)
            {
                if (grid[i][j] != 0)
                {
                    int nextI = i + dI[direction];
                    int nextJ = j + dJ[direction];
                    while (movePossible(i, j, nextI, nextJ))
                    {
                        if (grid[nextI][nextJ] == grid[i][j])
                        {
                            grid[nextI][nextJ] *= 2;
                            score += grid[nextI][nextJ];
                            grid[i][j] = 0;
                            moved = true;
                            break;
                        }
                        else if (grid[nextI][nextJ] == 0)
                        {
                            grid[nextI][nextJ] = grid[i][j];
                            grid[i][j] = 0;
                            i = nextI;
                            j = nextJ;
                            nextI += dI[direction];
                            nextJ += dJ[direction];
                            moved = true;
                        }
                    }
                }
            }
        }

        // Jeigu ejimas pajudino laukelius tikrinama ar zaidimas pasibaige
        if (moved)
        {
            addNewNumber();//prideda skaiciu lentoje jei tai imanoma
            gameOver = !anyMovePossible();
        }
    }

    // Metodas tikrina ar ejimas imanomas
    bool anyMovePossible() const
    {
        const int dI[] = { 1, 0, -1, 0 };
        const int dJ[] = { 0, 1, 0, -1 };

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (grid[i][j] == 0)
                    return true;
                else
                    for (int d = 0; d < 4; ++d)
                        if (movePossible(i, j, i + dI[d], j + dJ[d]))
                            return true;
        return false;
    }

    // Operatoriaus << perkrovimas zaidimo spausdinimui ekrane
    friend ostream& operator<<(ostream& os, const Board& board)
    {
        system("cls");
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                if (board.grid[i][j] == 0)
                {
                    os << setw(4) << ".";
                }
                else
                {
                    os << setw(4) << board.grid[i][j];
                }
            }
            os << endl;
        }
        os << "n: New game, b: Auto-play, w: Up, d: Right, s: Down, a: Left, q: Quit" << endl;
        os << "Score: " << board.score << endl;

        if (board.victory)
        {
            os << "Congratulations! You've reached 2048!" << endl;
            os << "You can continue playing to improve your score." << endl;
        }

        if (board.gameOver)
        {
            os << "Game over! No more moves possible." << endl;
        }
        return os;
    }
};

// Klase zaidimo logikai
class Game
{
public:
    Board board;
    map<char, int> commands; // Komandu ir krypciu mapas

    // Konstruktoriu inicializuoti komandoms
    Game()
    {
        commands['s'] = 0;
        commands['d'] = 1;
        commands['w'] = 2;
        commands['a'] = 3;
    }

    // Metodas pradeti zaidima
    void start()
    {
        char input;
        while (true)
        {
            board.checkVictory(); 
            cout << board; 
            cin >> input;

            if (input == 'n')
                board = Board(); 
            else if (input == 'q')
                break;
            else if (input == 'b')
                autoPlay();
            else if (commands.count(input))
                board.move(commands[input]);

            if (board.gameOver)
            {
                if (input != 'b')
                    cout << "Game over! No more moves possible." << endl;
                break;
            }
        }
    }

    // Metodas ijungti automatini 'bota'
    void autoPlay()
    {
        while (!board.gameOver)
        {
            int direction = 0; 
            for (int i = 0; i < 20; i++) 
            {
                direction = 0;
                if (i % 2 == 0)
                    direction = 1;
                board.move(direction);

                this_thread::sleep_for(chrono::milliseconds(50));
                cout << board;
            }
            direction = 2;
            board.move(direction);
            this_thread::sleep_for(chrono::milliseconds(50));
            cout << board; 
        }
    }
};

int main()
{
    srand(time(0));
    Game game; // Sukuria nauja zaidima
    game.start(); // Pradeda nauja zaidima
    return 0;
}
