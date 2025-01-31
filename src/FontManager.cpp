//This file originally downloaded from http://www.michaelzeilfelder.de/irrlicht.htm#TrueType. The copyright statement and license below do not apply to that original version, only to the modified version contained here.
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
 * The FontManager class is responsible for loading fonts.
 */

#include "FontManager.h"
#include "GUIFreetypeFont.h"
#include "StringConverter.h"

#include <irrlicht/irrlicht.h>
#ifdef HAVE_IOSTREAM
	#include <iostream>
#endif //HAVE_IOSTREAM
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

FontManager::FontManager() {
	try {
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in FontManager::FontManager(): " << e.what() << std::endl;
	}
}

FontManager::~FontManager() {
	try {
		FontMap::iterator itFont = mFontMap.begin();

		for( ; itFont not_eq mFontMap.end(); ++itFont ) {
			itFont->second->drop();
		}

		FaceMap::iterator itFace = mFaceMap.begin();

		for( ; itFace not_eq mFaceMap.end(); ++itFace ) {
			itFace->second->drop();
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in FontManager::~FontManager(): " << e.what() << std::endl;
	}
}

bool FontManager::canLoadFont( irr::core::stringw filename_ ) {
	try {
		CGUITTFace * face = new CGUITTFace;
		if( face->load( filename_ ) ) {
			face->drop();
			return true;
		} else {
			face->drop();
			return false;
		}
	} catch( std::exception &e ) {
		std::wcerr << L"Error in FontManager::canLoadFont(): " << e.what() << std::endl;
		return false;
	}
}

bool FontManager::canLoadFont( boost::filesystem::path filename_ ) {
	try {
		if ( not is_directory( filename_ ) ) {
			StringConverter sc;
			return canLoadFont( sc.toIrrlichtStringW( filename_.wstring() ) );
		}
	} catch( std::exception &e ) {
		std::wcerr << L"Error in FontManager::canLoadFont(): " << e.what() << std::endl;
	}
	return false;
}

irr::gui::IGUIFont* FontManager::GetTtFont( irr::video::IVideoDriver* driver, irr::core::stringw filename_, irr::u32 size_, bool antiAlias_, bool transparency_ ) {
	try {
		if( filename_.size() == 0 ) {//filename_.empty() ) { Irrlicht 1.8+ has .empty() but Raspbian only has 1.7 in its repositories
			return nullptr;
		}

		// Make a unique font name for the given settings.
		// We need a new font for each setting, but only a new face when loading a different fontfile
		irr::core::stringw fontString( MakeFontIdentifier( filename_, size_, antiAlias_, transparency_ ) );
		FontMap::iterator itFont = mFontMap.find( fontString );

		if( itFont not_eq mFontMap.end() )
			return itFont->second;

		// check if the face is already loaded
		irr::core::stringw faceName( filename_ );
		CGUITTFace * face = nullptr;
		FaceMap::iterator itFace = mFaceMap.find( faceName );

		if( itFace not_eq mFaceMap.end() ) {
			face = itFace->second;
		}

		// no face loaded
		if( not face ) {
			// make a new face
			face = new CGUITTFace;

			if( not face->load( filename_ ) ) {
				face->drop();
				return nullptr;
			}

			mFaceMap[ faceName ] = face;
		}

		// access to the video driver in my application.
		CGUIFreetypeFont * font = new CGUIFreetypeFont( driver );

		font->attach( face, size_ );
		font->AntiAlias = antiAlias_;
		font->Transparency = transparency_;
		mFontMap[ fontString ] = font;

		return font;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in FontManager::GetTtFont(): " << e.what() << std::endl;
		return nullptr;
	}
}

// make a unique font name for different settings.
irr::core::stringw FontManager::MakeFontIdentifier( irr::core::stringw filename_, irr::u32 size_, bool antiAlias_, bool transparency_ ) {
	try {
		/*std::ostringstream stream;
		stream << filename_ << size_;

		if( antiAlias_ )
			stream << 'a';

		if( transparency_ )
			stream << 't';

		//fprintf(stderr, "font: %s", stream.str().c_str());

		return stream.str();*/
		irr::core::stringw result = filename_;
		result.append( size_ );
		if( antiAlias_ ) {
			result.append( 'a' );
		}
		if( transparency_ ) {
			result.append( 't' );
		}
		return result;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in FontManager::MakeFontIdentifier(): " << e.what() << std::endl;
		irr::core::stringw s;
		return s;
	}
}
