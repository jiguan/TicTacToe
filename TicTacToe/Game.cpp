#include "pch.h"
#include "Game.h"
#include <assert.h>

using std::begin;
using std::end;
using std::copy;
using std::pair;
using std::inserter;
using std::make_pair;

Status const emptyBoard = { 0 };

// Better to check the folder and make sure files exist.
Status const strategyX[] =
{
#include "strategy_x.h";
};

Status const strategyO[] =
{
#include "strategy_o.h";
};

Game::Game() : started(false)
{
    status.assign(0);
};

bool Game::isWinning(Status const & status, Player const player) const
{
    auto mark = static_cast<char>(player);
    return
        (status[0] == mark && status[1] == mark && status[2] == mark) ||
        (status[3] == mark && status[4] == mark && status[5] == mark) ||
        (status[6] == mark && status[7] == mark && status[8] == mark) ||
        (status[0] == mark && status[4] == mark && status[8] == mark) ||
        (status[2] == mark && status[4] == mark && status[6] == mark) ||
        (status[0] == mark && status[3] == mark && status[6] == mark) ||
        (status[1] == mark && status[4] == mark && status[7] == mark) ||
        (status[2] == mark && status[5] == mark && status[8] == mark);
}

// If one wins, then display a line covering three valid marks.
pair<Cell, Cell> const Game::getWinningLine() const
{
    char mark;
    if (isVictory(Player::Computer))
        mark = static_cast<char>(Player::Computer);
    else if (isVictory(Player::User))
        mark = static_cast<char>(Player::User);
    else
        mark = static_cast<char>(Player::None);

    // Not a draw. 
    if (mark != 0)
    {
        if (status[0] == mark && status[1] == mark && status[2] == mark)
            return make_pair(Cell(0, 0), Cell(0, 2));
        if (status[3] == mark && status[4] == mark && status[5] == mark)
            return make_pair(Cell(1, 0), Cell(1, 2));
        if (status[6] == mark && status[7] == mark && status[8] == mark)
            return make_pair(Cell(2, 0), Cell(2, 2));
        if (status[0] == mark && status[4] == mark && status[8] == mark)
            return make_pair(Cell(0, 0), Cell(2, 2));
        if (status[2] == mark && status[4] == mark && status[6] == mark)
            return make_pair(Cell(0, 2), Cell(2, 0));
        if (status[0] == mark && status[3] == mark && status[6] == mark)
            return make_pair(Cell(0, 0), Cell(2, 0));
        if (status[1] == mark && status[4] == mark && status[7] == mark)
            return make_pair(Cell(0, 1), Cell(2, 1));
        if (status[2] == mark && status[5] == mark && status[8] == mark)
            return make_pair(Cell(0, 2), Cell(2, 2));
        throw std::runtime_error("At least one victroy case should be found");
    }

    return make_pair(Cell(), Cell());
}

void Game::start(Player const player)
{
    strategy.clear();
    if (player == Player::Computer)
        copy(begin(strategyX), end(strategyX), inserter(strategy, begin(strategy)));
    else if (player == Player::User)
        copy(begin(strategyO), end(strategyO), inserter(strategy, begin(strategy)));

    // Reset the status array to initial state.
    status.assign(0);
    started = true;
}

// Return whether the move is successful.
bool Game::move(Cell const cell, Player const player)
{
    // Check the cell is not occupied. 
    if (status[cell.row * 3 + cell.col] == 0)
    {
        status[cell.row * 3 + cell.col] = static_cast<char>(player);

        if (isVictory(player))
            started = false;
        return true;
    }
    return false;
}

std::set<Status> Game::lookupStrategy() const
{
    std::set<Status> next_substrategy;
    for (auto const & s : strategy)
    {
        bool match = true;
        for (int i{ 0 }; i < 9; ++i)
        {
            if (s[i] < status[i]) {
                match = false;
                break;
            }
        }

        if (match)
            next_substrategy.insert(s);
    }
    return next_substrategy;
}

Status Game::lookupMove() const
{
    Status bestMove = { 0 };
    for (auto const & s : strategy)
    {
        int diff = 0;
        for (int i{ 0 }; i < 9 && diff <= 1; ++i)
        {
            if (s[i] > status[i])
                ++diff;
        }

        if (diff == 1)
        {
            bestMove = s;
            if (isWinning(bestMove, Player::Computer))
                break;
        }
    }

    assert(bestMove != emptyBoard);
    return bestMove;
}

Cell Game::move(Player const player)
{
    Cell cell;
    strategy = lookupStrategy();

    if (!strategy.empty())
    {
        auto newStatus = lookupMove();

        for (int i{ 0 }; i < 9; ++i)
        {
            if (status[i] == 0 && newStatus[i] == static_cast<char>(player))
            {
                cell.row = i / 3;
                cell.col = i % 3;
                break;
            }
        }

        status = newStatus;

        if (isVictory(player))
        {
            started = false;
        }
    }
    else 
        throw std::runtime_error("No strategy is found");

    return cell;
}
