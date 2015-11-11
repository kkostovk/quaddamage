/***************************************************************************
 *   Copyright (C) 2009-2015 by Veselin Georgiev, Slavomir Kaslev et al    *
 *   admin@raytracing-bg.net                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/**
 * @File shading.h
 * @Brief Contains declarations of shader classes
 */
#ifndef __SHADING_H__
#define __SHADING_H__

#include "geometry.h"
#include "color.h"

class Shader {
public:
	virtual Color shade(const Ray& ray, const IntersectionInfo& info) = 0;
	virtual ~Shader() {}
};

class Texture {
public:
	virtual Color sample(const IntersectionInfo& info) = 0;
	virtual ~Texture() {}
};

class CheckerTexture: public Texture {
public:
	Color color1, color2;
	double scaling;
	CheckerTexture() { color1.makeZero(); color2.makeZero(); scaling = 1; }
	virtual Color sample(const IntersectionInfo& info);
};

class Bitmap;
class BitmapTexture: public Texture {
	Bitmap* bitmap;
	double scaling;
public:
	BitmapTexture(const char* filename, double scaling = 1.0);
	~BitmapTexture();
	Color sample(const IntersectionInfo& info);
};

class Lambert: public Shader {
public:
	Color color;
	Texture* texture;
	Lambert() { color.makeZero(); texture = NULL; }
	Color shade(const Ray& ray, const IntersectionInfo& info);	
};

class Phong: public Shader {
public:
	Color color;
	Texture* texture;
	double specularMultiplier;
	double specularExponent;
	Color shade(const Ray& ray, const IntersectionInfo& info);	
};

class Refl: public Shader {
public:
	double multiplier;
	Refl(double mult = 0.99): multiplier(mult) {}
	Color shade(const Ray& ray, const IntersectionInfo& info);	
	
};

class Refr: public Shader {
public:
	double ior_ratio;
	double multiplier;
	Refr(double ior, double mult = 0.99): ior_ratio(ior), multiplier(mult) {}
	Color shade(const Ray& ray, const IntersectionInfo& info);	
};

#endif // __SHADING_H__
