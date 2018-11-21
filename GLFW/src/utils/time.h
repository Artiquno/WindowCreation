#pragma once
#ifndef TIME_H
#define TIME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <type_traits>

// Static helper class for time information
// Should probably make this a float or double only but meh
template <class T>
class _Time
{
	static_assert(std::is_arithmetic<T>::value, "Time can only be a number type");
private:
	static T _time;
	static T _deltaTime;

public:
	static T getTime() { return _time; }
	static T deltaTime() { return _deltaTime; }
	static T frameRate() { return static_cast<T>(1.0) / _deltaTime; }
	static void update()
	{
		T time = static_cast<T>(glfwGetTime());
		_deltaTime = time - _time;
		_time = time;
	}
};

template <class T>
T _Time<T>::_time = static_cast<T>(0.0);
template <class T>
T _Time<T>::_deltaTime = static_cast<T>(0.0);

typedef _Time<float> Time;

#endif // !TIME_H
