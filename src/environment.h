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
 * @File environment.h
 * @Brief declarations of the Environment classes
 */
#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "color.h"
#include "vector.h"

enum CubeOrder {
	NEGX,
	NEGY,
	NEGZ,
	POSX,
	POSY,
	POSZ,
};

class Environment {
public:
	virtual ~Environment() {}
	/// gets a color from the environment at the specified direction
	virtual Color getEnvironment(const Vector& dir) = 0;
};

class Bitmap;
class CubemapEnvironment: public Environment {
	Bitmap* maps[6];
	
	Color getSide(const Bitmap& bmp, double x, double y);
	bool loadMaps(const char* folder);
public:
 	/// loads a cubemap from 6 separate images, from the specified folder.
 	/// The images have to be named "posx.bmp", "negx.bmp", "posy.bmp", ...
 	/// (or they may be .exr images, not .bmp).
 	/// The folder specification shouldn't include a trailing slash; 
 	/// e.g. "/images/cubemaps/cathedral" is OK.
	CubemapEnvironment(const char* folder);
	
	~CubemapEnvironment();
	Color getEnvironment(const Vector& dir);
    
};


#endif // __ENVIRONMENT_H__
