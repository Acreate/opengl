#ifndef FFIMAGE_H_H_HEAD__FILE__
#define FFIMAGE_H_H_HEAD__FILE__
#pragma once
#include "base.h"
#include <cstring>
#include <string>

class FfImage {
private:
	int m_width;
	int m_height;
	int m_picType;
	FfRGBA *m_data;
public:
	int getWidth( ) const { return m_width; }
	int getHeight( ) const { return m_height; }
	int getPicType( ) const { return m_picType; }
	FfRGBA * getData( ) const { return m_data; }

	FfRGBA getColor( int x, int y ) const {
		if( x < 0 || x > m_width - 1 || y < 0 || y > m_height - 1 ) {
			return FfRGBA( 0, 0, 0, 0 );
		}
		return m_data[ y * m_width + x ];
	}
	FfImage( int _width = 0, int _height = 0, int _picType = 0, FfRGBA *_data = NULL ) {
		m_width = _width;
		m_height = _height;
		m_picType = _picType;

		int _sumSize = m_width * m_height;
		if( _data && _sumSize ) {
			m_data = new FfRGBA[ _sumSize ];
			memcpy( m_data, _data, sizeof( FfRGBA ) * _sumSize );
		} else {
			m_data = NULL;
		}
	}
	~FfImage( ) {
		if( m_data ) {
			delete[]m_data;
		}
	}
public:
	static FfImage * readFromFile( const std::string& fileName );
};

#endif // FFIMAGE_H_H_HEAD__FILE__
