#ifndef BASE_H_H_HEAD__FILE__
#define BASE_H_H_HEAD__FILE__
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
typedef unsigned int uint;
typedef unsigned char byte;

struct FfRGBA {
	byte m_r;
	byte m_g;
	byte m_b;
	byte m_a;

	FfRGBA( byte _r = 255,
			byte _g = 255,
			byte _b = 255,
			byte _a = 255 ) {
		m_r = _r;
		m_g = _g;
		m_b = _b;
		m_a = _a;
	}
};

template< typename T >
struct tVec3 {
	T m_x;
	T m_y;
	T m_z;
	tVec3( T _x = 0, T _y = 0, T _z = 0 ) {
		m_x = _x;
		m_y = _y;
		m_z = _z;
	}
};
template< typename T >
struct tVec2 {
	T m_x;
	T m_y;

	tVec2( T _x = 0, T _y = 0 ) {
		m_x = _x;
		m_y = _y;
	}
};

#endif // BASE_H_H_HEAD__FILE__
