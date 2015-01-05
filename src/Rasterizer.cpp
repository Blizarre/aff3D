#include "Rasterizer.h"

void Rasterizer::drawLine(int x1, int x2, int y, Uint32 color, bool isWireFrame)
{
	if (isInRangeY(y))
	{
		if (isWireFrame)
		{
			m_surface.DrawPixel(x1, y, color);
			m_surface.DrawPixel(x2, y, color);
		}
		else
		{
			sortAndTrimXValues(x1, x2);
			for (int x = x1; x < x2; x++)
				m_surface.DrawPixel(x, y, color);
		}
	}
}

void Rasterizer::sortAndTrimXValues(int & val1, int & val2)
{
	if (val2 < val1)
	{
		int tmp = val1;
		val1 = val2;
		val2 = tmp;
	}

	// add an extra pixel at the beginnign and at the end of the line to make sure that no gap will be left between polygons
	val1--;
	val2++;

	// TODO: use the fact that val1 < val2
	if (val1 < 0)
		val1 = 0;
	else if (val1 >= m_surface.getWidth())
		val1 = m_surface.getWidth() - 1;

	if (val2 < 0)
		val2 = 0;
	else if (val2 >= m_surface.getWidth())
		val2 = m_surface.getWidth() - 1;
}

/**
* A triangle is draw in several steps:
* - first, the Vertex are projected on the screen space.
* - then, they are ordered by height.
* - the first half of the triangle is drawn (top point to the middle point)
* - and finally the bottom part of the triangle is drawn
* TODO: This code should be refactored massively.
**/
void Rasterizer::drawTriangle(const Triangle & t, bool isWireFrame) {
    // TODO: code reuse between the two halves
    Vertex lightPoint(0.5574f, 0.5574f, 0.5574f);
    float lightCoeff = lightPoint.x * t.points[3].x + lightPoint.y * t.points[3].y + lightPoint.z * t.points[3].z;
    lightCoeff = (lightCoeff > 0 ? lightCoeff : 0);

    Uint32 color = m_surface.getColor(static_cast<Uint8>(t.r * lightCoeff), static_cast<Uint8>(t.g * lightCoeff), static_cast<Uint8>(t.b * lightCoeff));

    Point tP[3];
    for (int v = 0; v<3; v++)
        projectToScreen(t.points[v], tP[v]);

    sortInPlace(tP);

    int x1, x2;
    float a1, a2;
    int yP;


    if (tP[2].y > 0)
    {
        yP = 0;
        if (tP[0].y == tP[2].y && tP[0].y > 0 && tP[0].y < m_surface.getHeight() - 1) {
            drawLine(tP[0].x, tP[2].x, tP[0].y, color, isWireFrame);
        }
        else {
            a1 = (tP[0].x - tP[2].x) / ((float)tP[0].y - tP[2].y);
            a2 = (tP[1].x - tP[2].x) / ((float)tP[1].y - tP[2].y);
            for (int y = tP[2].y; y > tP[1].y; y--) {
                x1 = (int)(-a1 * yP);
                x2 = (int)(-a2 * yP);

                drawLine(x1 + tP[2].x, x2 + tP[2].x, y, color, isWireFrame);
                yP++;
            }
        }
    }
    else // if this point's height is < 0, then it will be the same for all the others
    {
        return;
    }

    if (tP[1].y > 0)
    {
        yP = 0;
        if (tP[0].y == tP[1].y && tP[0].y > 0 && tP[0].y < m_surface.getHeight() - 1) {
            drawLine(tP[0].x, tP[1].x, tP[0].y, color, isWireFrame);
        }
        else {
            a1 = (tP[1].x - tP[0].x) / ((float)tP[1].y - tP[0].y);
            a2 = (tP[2].x - tP[0].x) / ((float)tP[2].y - tP[0].y);


            for (int y = tP[0].y; y <= tP[1].y; y++) {
                x1 = (int)(a1 * yP);
                x2 = (int)(a2 * yP);

                drawLine(x1 + tP[0].x, x2 + tP[0].x, y, color, isWireFrame);
                yP++;
            }
        }
    }
}