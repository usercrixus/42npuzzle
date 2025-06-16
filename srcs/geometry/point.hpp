#pragma once

template <typename T>
class Point
{
  private:
	T x;
	T y;

  public:
	Point(T x, T y);
	Point();
	~Point();

	T getX() const;
	T getY() const;
};

template <typename T>
inline Point<T>::Point(T x, T y) : x(x), y(y)
{
}

template <typename T>
inline Point<T>::Point() : x(0), y(0)
{
}


template <typename T>
inline Point<T>::~Point()
{
}

template <typename T>
inline T Point<T>::getX() const
{
	return x;
}

template <typename T>
inline T Point<T>::getY() const
{
	return y;
}
