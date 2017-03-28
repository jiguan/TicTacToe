#pragma once
#include <set>

enum class Player : char
{
    None = 0,
    Computer = 1,
    User = 2,
};

typedef std::array<char, 9> Status;

struct Cell
{
    int row;
    int col;

    Cell(int r = INT_MAX, int c = INT_MAX) : row{ r }, col{ c } {};

    bool isValid() const { return row != INT_MAX && col != INT_MAX; }
};

class Game
{
private:
    bool started;
    Status status;
    std::set<Status> strategy;

    std::set<Status> lookupStrategy() const;
    Status lookupMove() const;

    bool isWinning(Status const & status, Player const player) const;
    bool isFull(Status const & status) const
    {
        return 0 == std::count_if(std::begin(status), std::end(status), [](int const mark) {return mark == 0; });
    }
public:
    Game();

    void start(Player const player);
    bool move(Cell const cell, Player const player);
    Cell move(Player const player);

    bool isStarted() const { return started; }
    bool isVictory(Player const player) const { return isWinning(status, player); }
    bool isFinished() const
    {
        return isFull(status) || isVictory(Player::User) || isVictory(Player::Computer);
    }
    Status const getStatus() const { return status; }
    std::pair<Cell, Cell> const getWinningLine() const;
};