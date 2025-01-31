/**
 * @file
 * @author James Dearing <dearingj@lifetime.oregonstate.edu>
 * 
 * @section LICENSE
 * Copyright © 2012-2017.
 * This file is part of Cybrinth.
 *
 * Cybrinth is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * Cybrinth is distributed 'as is' in the hope that it will be fun, but WITHOUT ANY WARRANTY; without even the implied warranty of TITLE, MERCHANTABILITY, COMPLETE DESTRUCTION OF EVIL MONSTERS, or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with Cybrinth. If not, see <http://www.gnu.org/licenses/>.
 * 
 * @section DESCRIPTION
 * The PlayerStart class represents a given player's beginning point in the maze. Like a Goal but it's at the start rather than the end.
 */
 
#include "PlayerStart.h"
#include "colors.h"
#ifdef HAVE_IOSTREAM
#include <iostream>
#endif //HAVE_IOSTREAM
#include "XPMImageLoader.h"

PlayerStart::PlayerStart() {
	try {
		setColors( GRAY, LIGHTRED );
		reset();
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in PlayerStart::PlayerStart(): " << e.what() << std::endl;
	}
}

PlayerStart::~PlayerStart() {
	try {
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in PlayerStart::~PlayerStart(): " << e.what() << std::endl;
	}
}

void PlayerStart::createTexture( irr::IrrlichtDevice* device, uint_fast16_t size ) {
	try {
		irr::video::IVideoDriver* driver = device->getVideoDriver();
		XPMImageLoader loader;
		
		irr::video::IImage* tempImage = driver->createImage( irr::video::ECF_A8R8G8B8, irr::core::dimension2d< irr::u32 >( size, size ) );
		loader.loadOtherImage( driver, tempImage, XPMImageLoader::START );
		
		irr::core::stringw textureName = "start-xpm";
		
		adjustImageColors( tempImage );
		
		texture = resizer.imageToTexture( driver, tempImage, textureName );

		if( texture == nullptr ) {
			irr::video::IImage* temp = driver->createImage( irr::video::ECF_A1R5G5B5, irr::core::dimension2d< irr::u32 >( size, size ) );
			temp->fill( WHITE );
			texture = resizer.imageToTexture( driver, temp, "generic start" );
		}
		
		if( texture not_eq nullptr and texture->getSize() not_eq irr::core::dimension2d< irr::u32 >( size, size ) ) {
			auto newTexture = resizer.resize( texture, size, size, driver );
			driver->removeTexture( texture );
			texture = newTexture;
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in PlayerStart::createTexture(): " << e.what() << std::endl;
	}
}

void PlayerStart::draw( irr::IrrlichtDevice* device, uint_fast16_t width, uint_fast16_t height ) {
	try {
		uint_fast16_t size;

		if( width < height ) {
			size = width;
		} else {
			size = height;
		}

		if( texture == nullptr or ( texture not_eq nullptr and texture->getSize().Width not_eq size ) ) {
			Object::loadTexture( device, size, L"start" ); //NOTE:The "start" string should be the same as in the loadTexture() function above
			if( texture == nullptr or texture == NULL ) {
				createTexture( device, size );
			}
		}

		Object::draw( device, width, height );
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in PlayerStart::draw(): " << e.what() << std::endl;
	}
}

void PlayerStart::loadTexture( irr::IrrlichtDevice* device ) {
	try {
		Object::loadTexture( device, 1, L"start" ); //NOTE:The "start" string should be the same as in the draw() function above
		if( texture == nullptr or texture == NULL ) {
			createTexture( device, 1 );
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in PlayerStart::loadTexture(): " << e.what() << std::endl;
	}
}

void PlayerStart::reset() {
	try {
		texture = nullptr;
		x = 0;
		y = 0;
		distanceFromExit = 0;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in PlayerStart::reset(): " << e.what() << std::endl;
	}
}

