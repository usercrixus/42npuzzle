#pragma once
#include <vector>
#include <memory>

template <typename T>
class AAStar
{
protected:
	std::vector<T> visitedStates;
public:
	virtual ~AAStar();

	virtual bool isGoal() const = 0;
	virtual size_t hash() const = 0;
	virtual bool equals(const T &other) const = 0;
	virtual int estimate(const T& state) const = 0;
};

template <typename T>
inline AAStar<T>::~AAStar()
{
}
