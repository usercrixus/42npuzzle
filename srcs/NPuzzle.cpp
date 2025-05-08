#include "NPuzzle.hpp"
#include <iostream>
#include <climits>

NPuzzle::NPuzzle() : size(0) {}
NPuzzle::~NPuzzle() {}

bool NPuzzle::parse(std::string path)
{
    FileToVector<int> parser;
    if (!parser.parse(path))
        return false;
    auto const &tokens = parser.getTokens();
    if (tokens.size() < 2)
        return (std::cout << "The file " << path << " is incorrectly formatted\n", false);
    size = tokens[0];
    if (tokens.size() - 1 != size * size)
        return (std::cout << "The file " << path << " is incorrectly formatted\n", false);
    puzzle.assign(size, std::vector<int>());
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            puzzle[i].push_back(tokens[1 + i * size + j]);
    return true;
}

void NPuzzle::print() const
{
    for (auto const &row : puzzle)
    {
        for (int v : row)
            std::cout << v << " ";
        std::cout << "\n";
    }
}

std::string NPuzzle::flatten() const
{
    std::ostringstream oss;
    for (const auto& row : puzzle)
        for (int v : row)
            oss << v << ",";
    return oss.str();
}

bool NPuzzle::isSolvable() const
{
    std::vector<int> flat;
    int blankRow = -1;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int v = puzzle[i][j];
            if (v == 0) blankRow = i;
            else flat.push_back(v);
        }
    }
    int inv = 0;
    for (size_t a = 0; a < flat.size(); ++a)
        for (size_t b = a+1; b < flat.size(); ++b)
            if (flat[a] > flat[b]) ++inv;
    if (size % 2 == 1) return (inv % 2) == 0;
    int rowFromBottom = size - blankRow;
    return ((inv + rowFromBottom) % 2) == 1;
}


Point<int> NPuzzle::getZero() const
{
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            if (puzzle[i][j] == 0)
                return Point<int>(j, i);
    return Point<int>(-1, -1);
}

std::vector<NPuzzle::Move> NPuzzle::getMove(Point<int> zero) const
{
    std::vector<Move> mv;
    int x = zero.getX(), y = zero.getY();
    if (x > 0)
        mv.push_back(LEFT);
    if (x < size - 1)
        mv.push_back(RIGHT);
    if (y > 0)
        mv.push_back(TOP);
    if (y < size - 1)
        mv.push_back(BOTTOM);
    return mv;
}

NPuzzle NPuzzle::applyMove(Move m) const
{
    NPuzzle nxt = *this;
    auto [x, y] = std::pair<int, int>(getZero().getX(), getZero().getY());
    if (m == LEFT)
        std::swap(nxt.puzzle[y][x], nxt.puzzle[y][x - 1]);
    if (m == RIGHT)
        std::swap(nxt.puzzle[y][x], nxt.puzzle[y][x + 1]);
    if (m == TOP)
        std::swap(nxt.puzzle[y][x], nxt.puzzle[y - 1][x]);
    if (m == BOTTOM)
        std::swap(nxt.puzzle[y][x], nxt.puzzle[y + 1][x]);
    return nxt;
}

bool NPuzzle::isGoal() const
{
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
        {
            int expect = i * size + j + 1;
            if (i == size - 1 && j == size - 1)
            {
                if (puzzle[i][j] != 0)
                    return false;
            }
            else if (puzzle[i][j] != expect)
                return false;
        }
    return true;
}

std::pair<NPuzzle::Move, int> NPuzzle::getBestMove() const
{
    auto zero = getZero();
    auto moves = getMove(zero);
    std::pair<Move, int> best{LEFT, INT_MAX};
    for (auto m : moves)
    {
        NPuzzle tmp = applyMove(m);
        int h = estimate(tmp);
        if (h < best.second)
            best = {m, h};
    }
    return best;
}

int NPuzzle::estimate(const NPuzzle &other) const
{
    int sum = 0;
    for (int row = 0; row < size; ++row)
        for (int col = 0; col < size; ++col)
        {
            int value = other.puzzle[row][col];
            if (value > 0)
            {
                int targetRow = (value - 1) / size;
                int targetCol = (value - 1) % size;
                sum += abs(row - targetRow) + abs(col - targetCol);
            }
        }
    return sum;
}

const std::vector<std::vector<int>> &NPuzzle::getPuzzle() const
{
    return puzzle;
}
