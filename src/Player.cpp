/**
 * Copyright © 2012-2014 James Dearing.
 * This file is part of Cybrinth.
 *
 * Cybrinth is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * Cybrinth is distributed in the hope that it will be fun, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along with Cybrinth. If not, see <http://www.gnu.org/licenses/>.
*/
#include "Player.h"
#include <irrlicht/irrlicht.h>
#ifdef HAVE_IOSTREAM
#include <iostream>
#endif //HAVE_IOSTREAM
#include "colors.h"
#include <boost/filesystem.hpp>
#include "GameManager.h"

// cppcheck-suppress uninitMemberVar
Player::Player() {
	try {
		x = 0;
		y = 0;
		xInterp = 0;
		yInterp = 0;
		moving = false;
		setColor( RED );
		texture = nullptr;
		isHuman = true;
		timeTakenThisMaze = 0;
		stepsTakenThisMaze = 0;
		keysCollectedThisMaze = 0;
		scoreTotal = 0;
		setPlayerNumber( UINT_FAST8_MAX );
		reset();
		setGM( nullptr );
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::Player(): " << e.what() << std::endl;
	}
}

Player::~Player() {
	try {
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::~Player(): " << e.what() << std::endl;
	}
}

//Draws a filled circle. Somebody please implement a faster algorithm.
void Player::createTexture( irr::IrrlichtDevice* device, uint_fast16_t size ) {
	try {
		auto driver = device->getVideoDriver();
		irr::video::IImage *tempImage = driver->createImage( irr::video::ECF_A8R8G8B8, irr::core::dimension2d< irr::u32 >( size, size ) ); //Colorspace should be irr::video::A1R5G5B5 but that causes a bug on my current laptop.
		tempImage->fill( irr::video::SColor( 0, 0, 0, 0 ) ); //Fills the image with invisibility!
		tempImage->setPixel( size - 1, size - 1, irr::video::SColor( 0, 0, 0, 0 ) ); //Workaround for a bug in Irrlicht's software renderer
		
		irr::core::position2d< decltype( size ) > origin( size / 2, size / 2 );
		
		{
			int_fast16_t radius = size / 2;
			float rSquared = pow( static_cast< float >( radius ), 2.0f );
			for( auto x = -radius; x <= 0; ++x ) {
				auto height = static_cast< decltype( radius ) >( sqrt( rSquared - static_cast< float >( pow( static_cast< float >( x ), 2.0f ) ) ) );
				for( auto y = -height; y <= 0; ++y ) {
					tempImage->setPixel( x + origin.X, y + origin.Y, colorOne );
					tempImage->setPixel( x + origin.X, -y + origin.Y, colorOne );
					tempImage->setPixel( -x + origin.X, y + origin.Y, colorOne );
					tempImage->setPixel( -x + origin.X, -y + origin.Y, colorOne );
				}
			}
		}
		
		{
			size /= 2;
			int_fast16_t radius = size / 2;
			float rSquared = pow( static_cast< float >( radius ), 2.0f );
			for( auto x = -radius; x <= 0; ++x ) {
				auto height = static_cast< decltype( radius ) >( sqrt( rSquared - static_cast< float >( pow( static_cast< float >( x ), 2.0f ) ) ) );
				for( auto y = -height; y <= 0; ++y ) {
					tempImage->setPixel( x + origin.X, y + origin.Y, colorTwo );
					tempImage->setPixel( x + origin.X, -y + origin.Y, colorTwo );
					tempImage->setPixel( -x + origin.X, y + origin.Y, colorTwo );
					tempImage->setPixel( -x + origin.X, -y + origin.Y, colorTwo );
				}
			}
		}
		
		
		driver->removeTexture( texture );
		texture = driver->addTexture( L"playerCircle", tempImage );
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::createTexture(): " << e.what() << std::endl;
	}
}

void Player::draw( irr::IrrlichtDevice* device, uint_fast16_t width, uint_fast16_t height ) {
	try {
		uint_fast16_t size;

		if( width < height ) {
			size = width;
		} else {
			size = height;
		}

		if( texture->getSize().Width != size ) {
			loadTexture( device, size );
			if( texture == nullptr || texture == NULL ) {
				createTexture( device, size );
			}
		}

		Object::draw( device, width, height );
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::draw(): " << e.what() << std::endl;
	}
}

uint_fast8_t Player::getItem() {
	return heldItem;
}

Collectable::type_t Player::getItemType() {
	return heldItemType;
}

intmax_t Player::getScoreLastMaze() {
	return scoreLastMaze;
}
intmax_t Player::getScoreTotal() {
	return scoreTotal;
}

void Player::giveItem( uint_fast8_t item, Collectable::type_t type ) {
	heldItem = item;
	heldItemType = type;
	if( gm != nullptr ) {
		gm->getCollectable( heldItem )->setX( x );
		gm->getCollectable( heldItem )->setY( y );
		gm->getCollectable( heldItem )->owned = true;
	}
}

bool Player::hasItem() {
	return heldItem != UINT_FAST8_MAX;
}

bool Player::hasItem( uint_fast8_t item ) {
	return heldItem == item;
}

void Player::loadTexture( irr::IrrlichtDevice* device ) {
	try {
		if( !( texture == nullptr || texture == NULL ) ) {
			device->getVideoDriver()->removeTexture( texture );
			texture = nullptr;
		}
		loadTexture( device, 1 );
		if( texture == nullptr || texture == NULL ) {
			createTexture( device, 1 );
		}
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::loadTexture(): " << e.what() << std::endl;
	}
}

void Player::loadTexture( irr::IrrlichtDevice* device, uint_fast16_t size ) {
	irr::core::stringw fileName = L"player";
	if( !( texture == nullptr || texture == NULL ) ) {
		device->getVideoDriver()->removeTexture( texture );
		texture = nullptr;
	}
	Object::loadTexture( device, size, fileName );
	
	if( texture == nullptr || texture == NULL ) {
		if( playerNumber < 10 ) {
			fileName += L"00";
		} else if( playerNumber < 100 ) {
			fileName += L"0";
		}
		fileName += playerNumber;
		Object::loadTexture( device, size, fileName );
	}
	
	if( texture == nullptr || texture == NULL ) {
		createTexture( device, size );
	} else {
		irr::video::IVideoDriver* driver = device->getVideoDriver();
		irr::video::IImage* image = resizer.textureToImage( driver, texture );
		irr::core::stringw textureName = texture->getName().getInternalName(); //Needed when converting the image back to a texture
		driver->removeTexture( texture );
		texture = nullptr;
		
		//Find the darkest and lightest colors
		irr::video::SColor darkestColor = WHITE;
		auto darkestLuminance = darkestColor.getLuminance();
		irr::video::SColor lightestColor = BLACK;
		auto lightestLuminance = lightestColor.getLuminance();
		for( decltype( image->getDimension().Width ) x = 0; x < image->getDimension().Width; ++x ) {
			for( decltype( image->getDimension().Height ) y = 0; y < image->getDimension().Height; ++y ) {
				auto pixel = image->getPixel( x, y );
				if( pixel.getAlpha() > 0 ) {
					if( pixel.getLuminance() < darkestLuminance ) {
						darkestColor = pixel;
						darkestLuminance = darkestColor.getLuminance();
					}
					
					if( pixel.getLuminance() > lightestLuminance ) { //This is a separate if, not an else if, because there's a tiny chance the lightest and darkest colors might be the same
						lightestColor = pixel;
						lightestLuminance = lightestColor.getLuminance();
					}
				}
			}
		}
		
		//Now, set pixels to their desired colors (interpolate between colorOne and colorTwo instead of the lightest and darkest colors in the original file)
		for( decltype( image->getDimension().Width ) x = 0; x < image->getDimension().Width; ++x ) {
			for( decltype( image->getDimension().Height ) y = 0; y < image->getDimension().Height; ++y ) {
				auto pixel = image->getPixel( x, y );
				if( pixel.getAlpha() > 0 ) {
					auto luminance = pixel.getLuminance();
					if( luminance == lightestLuminance ) {
						auto newColor = colorTwo;
						newColor.setAlpha( pixel.getAlpha() );
						image->setPixel( x, y, newColor );
					} else if( luminance < lightestLuminance && luminance > darkestLuminance ) {
						auto interpolation = ( lightestLuminance - luminance ) / 255.0f;
						auto newColor = colorOne.getInterpolated( colorTwo, interpolation );
						image->setPixel( x, y , newColor );
					} else { // if( luminance == darkestLuminance ) {
						auto newColor = colorOne;
						newColor.setAlpha( pixel.getAlpha() );
						image->setPixel( x, y, newColor );
					}
				}
			}
		}
		
		textureName += L"-recolored";
		texture = resizer.imageToTexture( driver, image, textureName );
	}
}

void Player::moveX( int_fast8_t val ) {
	try {
		Object::moveX( val );
		stepsTakenThisMaze += 1;
		if( hasItem() && gm != nullptr ) {
			gm->getCollectable( heldItem )->moveX( val );
		}
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::moveX(): " << e.what() << std::endl;
	}
}

void Player::moveY( int_fast8_t val ) {
	try {
		Object::moveY( val );
		stepsTakenThisMaze += 1;
		if( hasItem() && gm != nullptr ) {
			gm->getCollectable( heldItem )->moveY( val );
		}
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::moveY(): " << e.what() << std::endl;
	}
}

void Player::removeItem() {
	if( hasItem() ) {
		gm->eraseCollectable( heldItem );
		heldItem = UINT_FAST8_MAX;
	}
}

void Player::reset() {
	try {
		stepsTakenLastMaze = stepsTakenThisMaze;
		stepsTakenThisMaze = 0;
		timeTakenLastMaze = timeTakenThisMaze;
		timeTakenThisMaze = 0;
		keysCollectedLastMaze = keysCollectedThisMaze;
		keysCollectedThisMaze = 0;
		heldItem = UINT_FAST8_MAX;
		scoreLastMaze = 0;
	} catch( std::exception &e ) {
		std::wcerr << L"Error in Player::reset(): " << e.what() << std::endl;
	}
}

void Player::setColorBasedOnNum() {
	Object::setColorBasedOnNum( playerNumber );
}

void Player::setGM( GameManager* newGM ) {
	gm = newGM;
}

void Player::setPlayerNumber( uint_fast8_t newNumber ) {
	playerNumber = newNumber;
}

void Player::setScore( intmax_t newScore ) {
	scoreTotal += newScore;
	scoreLastMaze = newScore;
}
