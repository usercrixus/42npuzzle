// solveWithAStar.cpp
#include "solveWithAStar.hpp"
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>

struct ANode
{
    NPuzzle board;
    int g;         // cost so far
    int f;         // g + h
    int parentIdx; // index in allNodes
    NPuzzle::Move moveFromParent;
};

struct PQItem
{
    int idx;
    int f;
    bool operator<(PQItem const &o) const
    {
        // we want smallest f at top, so invert
        return f > o.f;
    }
};

std::vector<NPuzzle::Move> solveWithAStar(const NPuzzle &start)
{
    if (!start.isSolvable())
        return {};
    if (start.isGoal())
        return {};

    std::vector<ANode> allNodes;
    allNodes.reserve(20000);

    // g-scores by flattened string
    std::unordered_map<std::string, int> gScore;
    gScore.reserve(20000);

    std::priority_queue<PQItem> open;
    auto startKey = start.flatten();
    int h0 = start.estimate(start); // estimate(goal) but same as estimate(start→goal)
    allNodes.push_back({start, 0, h0, -1, NPuzzle::LEFT});
    open.push({0, h0});
    gScore[startKey] = 0;

    int goalIdx = -1;
    while (!open.empty())
    {
        auto [curIdx, curF] = open.top();
        open.pop();
        auto &node = allNodes[curIdx];

        // If this node is actually stale (we found a better g in the meantime), skip
        if (node.g + node.board.estimate(node.board) != curF)
            continue;

        if (node.board.isGoal())
        {
            goalIdx = curIdx;
            break;
        }

        // Expand
        auto zeroPos = node.board.getZero();
        for (auto mv : node.board.getMove(zeroPos))
        {
            NPuzzle next = node.board.applyMove(mv);
            std::string key = next.flatten();
            int tentativeG = node.g + 1;

            auto it = gScore.find(key);
            if (it == gScore.end() || tentativeG < it->second)
            {
                int h = next.estimate(next);
                int f = tentativeG + h;
                int nextIdx = allNodes.size();
                allNodes.push_back({next, tentativeG, f, curIdx, mv});
                gScore[key] = tentativeG;
                open.push({nextIdx, f});
            }
        }
    }

    // Reconstruct
    std::vector<NPuzzle::Move> path;
    if (goalIdx != -1)
    {
        for (int at = goalIdx; at > 0; at = allNodes[at].parentIdx)
        {
            path.push_back(allNodes[at].moveFromParent);
        }
        std::reverse(path.begin(), path.end());
    }
    return path;
}
