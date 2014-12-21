#ifndef __TRANSFO_H__256000111

#define __TRANSFO_H__256000111

#include <vector>
#include <array>
#include <algorithm>

#include "vertex.h"

class Transformation {

public:

	Transformation() {
		// Initialization with unity matrix (all 0, except the diagonal filled with 1)
		m_matrix.fill(0.0f);

		m_matrix[0] = 1.0f;
		m_matrix[5] = 1.0f;
		m_matrix[10] = 1.0f;
		m_matrix[15] = 1.0f;
	}

	void translate(const float delta[3]);

	void rotationX(const float rot);
	void rotationY(const float rot);
	void rotationZ(const float rot);

	Vertex applyTo(const Vertex& v) const;
	void applyTo(const Vertex & vIn, Vertex & vOut) const;

	/***
	 * Apply the transformation to all elements of the array (size N). 
	***/
	template <size_t N>
	void applyTo(const std::array<Vertex, N>& originalArray, std::array<Vertex, N>& newArray) const
	{
		auto itOrig = originalArray.cbegin();
		auto itNew = newArray.begin();

		for (; itOrig != originalArray.cend(); ++itOrig, ++itNew)
		{
			// Push_back is not really the best way to do this.
			// We'll see if it makes a difference in the benchmark
			applyTo(*itOrig, *itNew);
		}
	};

	
protected:

	std::array<float, 16> m_matrix;

};

#endif
