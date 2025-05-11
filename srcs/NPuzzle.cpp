#include "NPuzzle.hpp"

NPuzzle::NPuzzle(int heuristicMode) : size(0), heuristicMode(heuristicMode) {}
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

bool NPuzzle::parse()
{
    size = 4;
    std::vector<int> buffer(size * size);
    std::iota(buffer.begin(), buffer.end(), 0);
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(buffer.begin(), buffer.end(), rng);
    puzzle.assign(size, std::vector<int>(size));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            puzzle[i][j] = buffer[i * size + j];
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
    std::cout << "\n";
}

std::string NPuzzle::flatten() const
{
    std::ostringstream oss;
    for (const auto &row : puzzle)
        for (int v : row)
            oss << v << ",";
    return oss.str();
}

bool NPuzzle::isSolvable() const
{
    std::vector<int> flat;
    int blankRow = -1;
    for (int row = 0; row < size; ++row)
    {
        for (int column = 0; column < size; ++column)
        {
            int v = puzzle[row][column];
            if (v == 0)
                blankRow = row;
            else
                flat.push_back(v);
        }
    }
    int inv = 0;
    for (size_t row = 0; row < flat.size(); ++row)
        for (size_t column = row + 1; column < flat.size(); ++column)
            if (flat[row] > flat[column])
                ++inv;
    if (size % 2 == 1)
        return (inv % 2) == 0;
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

NPuzzle::HeuristicFunction NPuzzle::getHeuristicFunction() const
{
    if (heuristicMode == 1)
        return &NPuzzle::estimateMisplacedTiles;
    if (heuristicMode == 2)
        return &NPuzzle::estimateLinearConflict;
    return &NPuzzle::estimateManhattan;
}


int NPuzzle::estimateManhattan(const NPuzzle &other) const
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

int NPuzzle::estimateMisplacedTiles(const NPuzzle &other) const
{
    int count = 0;
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            int value = other.puzzle[row][col];
            int goalValue = (row == size - 1 && col == size - 1) ? 0 : row * size + col + 1;
            if (value != 0 && value != goalValue)
                ++count;
        }
    }
    return count;
}

int NPuzzle::estimateLinearConflict(const NPuzzle &other) const
{
    int conflict = 0;
    for (int row = 0; row < size; ++row)
    {
        for (int i = 0; i < size; ++i)
        {
            int tile1 = other.puzzle[row][i];
            if (tile1 == 0) continue;
            int goalRow1 = (tile1 - 1) / size;
            if (goalRow1 != row) continue;

            for (int j = i + 1; j < size; ++j)
            {
                int tile2 = other.puzzle[row][j];
                if (tile2 == 0) continue;
                int goalRow2 = (tile2 - 1) / size;
                if (goalRow2 != row) continue;

                int goalCol1 = (tile1 - 1) % size;
                int goalCol2 = (tile2 - 1) % size;
                if (goalCol1 > goalCol2)
                    conflict += 2;
            }
        }
    }
    return estimateManhattan(other) + conflict; // Manhattan + linear conflict
}

const std::vector<std::vector<int>> &NPuzzle::getPuzzle() const
{
    return puzzle;
}
