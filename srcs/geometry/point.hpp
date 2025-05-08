template <typename T>
class Point
{
private:
	T x;
	T y;
public:
	Point(T x, T y);
	~Point();

	T getX();
	T getY();
};

template <typename T>
inline Point<T>::Point(T x, T y):
x(x),
y(y)
{
}

template <typename T>
inline Point<T>::~Point()
{
}

template <typename T>
inline T Point<T>::getX()
{
    return (x);
}

template <typename T>
inline T Point<T>::getY()
{
    return (y);
}
