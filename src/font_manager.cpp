//This file originally downloaded from http://www.michaelzeilfelder.de/irrlicht.htm#TrueType. The copyright statement and license below do not apply to that original version, only to the modified version contained here.
/**
 * Copyright © 2013 James Dearing.
 * This file is part of Cybrinth.
 * 
 * Cybrinth is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * 
 * Cybrinth is distributed in the hope that it will be fun, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License along with Cybrinth. If not, see <http://www.gnu.org/licenses/>.
*/

#include "font_manager.h"
#include "gui_freetype_font.h"
#include <sstream>

FontManager::FontManager() {
}

FontManager::~FontManager() {
	FontMap::iterator itFont = mFontMap.begin();

	for( ; itFont != mFontMap.end(); ++itFont ) {
		itFont->second->drop();
	}

	FaceMap::iterator itFace = mFaceMap.begin();

	for( ; itFace != mFaceMap.end(); ++itFace ) {
		itFace->second->drop();
	}
}

irr::gui::IGUIFont* FontManager::GetTtFont( irr::video::IVideoDriver* driver, const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_ ) {
	if( !filename_ || !strlen( filename_ ) )
		return NULL;

	// Make a unique font name for the given settings.
	// We need a new font for each setting, but only a new face when loading a different fontfile
	std::string fontString( MakeFontIdentifier( filename_, size_, antiAlias_, transparency_ ) );
	FontMap::iterator itFont = mFontMap.find( fontString );

	if( itFont != mFontMap.end() )
		return itFont->second;

	// check if the face is already loaded
	std::string faceName( filename_ );
	CGUITTFace * face = NULL;
	FaceMap::iterator itFace = mFaceMap.find( faceName );

	if( itFace != mFaceMap.end() ) {
		face = itFace->second;
	}

	// no face loaded
	if( !face ) {
		// make a new face
		face = new CGUITTFace;

		if( !face->load( filename_ ) ) {
			face->drop();
			return NULL;
		}

		mFaceMap[faceName] = face;
	}

	// access to the video driver in my application.
	CGUIFreetypeFont * font = new CGUIFreetypeFont( driver );

	font->attach( face, size_ );
	font->AntiAlias = antiAlias_;
	font->Transparency = transparency_;
	mFontMap[fontString] = font;

	return font;
}

// make a unique font name for different settings.
std::string FontManager::MakeFontIdentifier( const char* filename_, unsigned int size_, bool antiAlias_, bool transparency_ ) {
	std::ostringstream stream;
	stream << filename_ << size_;

	if( antiAlias_ )
		stream << 'a';

	if( transparency_ )
		stream << 't';

	//fprintf(stderr, "font: %s", stream.str().c_str());

	return stream.str();
}

