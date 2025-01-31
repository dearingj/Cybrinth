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
 * The GUIFreetypeFont class is used to represent fonts.
 */

#include "CustomException.h"
#include "GUIFreetypeFont.h"

#if COMPILE_WITH_FREETYPE

#ifdef HAVE_CASSERT
	#include <cassert>
#endif //HAVE_CASSERT
#include "Integers.h"
#ifdef HAVE_IOSTREAM
	#include <iostream>
#endif //HAVE_IOSTREAM

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif


// --------------------------------------------------------
CGUITTGlyph::CGUITTGlyph()
	: IReferenceCounted()
	, cached( false )
	, size( 0 )
	, top( 0 )
	, left( 0 )
	, texw( 0 )
	, texh( 0 )
	, imgw( 0 )
	, imgh( 0 )
	, tex( nullptr )
	, top16( 0 )
	, left16( 0 )
	, texw16( 0 )
	, texh16( 0 )
	, imgw16( 0 )
	, imgh16( 0 )
	, tex16( nullptr )
	, image( nullptr ) {
	try {
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTGlyph::CGUITTGlyph(): " << e.what() << std::endl;
	}
}

CGUITTGlyph::~CGUITTGlyph() {
	try {
		delete[ ] image;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTGlyph::~CGUITTGlyph(): " << e.what() << std::endl;
	}
}

void CGUITTGlyph::cache( irr::u32 idx_, const CGUIFreetypeFont * freetypeFont ) {
	try {
		assert( freetypeFont );

		auto face = freetypeFont->TrueTypeFace->face;

		if( FT_Set_Pixel_Sizes( face, 0, size ) ) {
			throw( CustomException( std::wstring( L"Cannot set pixel size to " + std::to_wstring( size ) ) ) );
		}

		if( size > freetypeFont->LargestGlyph.Height ) {
			freetypeFont->LargestGlyph.Height = size;
		}

		if( not FT_Load_Glyph( face, idx_, FT_LOAD_DEFAULT ) ) { //FT_LOAD_NO_HINTING bitor FT_LOAD_NO_BITMAP ) ) {
			FT_GlyphSlot glyph = face->glyph;
			FT_Bitmap  bits;

			if( glyph->format == ft_glyph_format_outline ) {
				if( not FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL ) ) {
					bits = glyph->bitmap;
					irr::u8 *pt = bits.buffer;
					delete[ ] image;
					image = new irr::u8[ bits.width * bits.rows ];
					memcpy( image, pt, bits.width * bits.rows );
					top = glyph->bitmap_top;
					left = glyph->bitmap_left;
					imgw = 1;
					imgh = 1;
					texw = bits.width;
					texh = bits.rows;
					
					while( imgw <= texw ) {
						imgw <<= 1;
					}
					
					while( imgh <= texh ) {
						imgh <<= 1;
					}

					if( imgw > imgh ) {
						imgh = imgw;
					} else {
						imgw = imgh;
					}

					irr::s32 offx = left;
					irr::s32 offy = size - top;

					if( offx + texw > freetypeFont->LargestGlyph.Width ) {
						freetypeFont->LargestGlyph.Width = offx + texw;
					}

					if( offy + texh > freetypeFont->LargestGlyph.Height ) {
						freetypeFont->LargestGlyph.Height = offy + texh;
					}

					irr::u32 *texd = new irr::u32[ imgw*imgh ];
					memset( texd, 0, imgw * imgh * sizeof( irr::u32 ) );
					irr::u32 *texp = texd;
					bool cflag = ( freetypeFont->Driver->getDriverType() == irr::video::EDT_DIRECT3D8 );

					for( decltype( bits.rows ) i = 0; i < bits.rows; ++i ) {
						irr::u32 *rowp = texp;

						for( decltype( bits.width ) j = 0; j < bits.width; ++j ) {
							if( *pt ) {
								if( cflag ) {
									*rowp = *pt;
									*rowp *= 0x01010101;
								} else {
									*rowp = *pt << 24;
									*rowp or_eq 0xffffff;
								}
							} else {
								*rowp = 0;
							}

							++pt;
							++rowp;
						}

						texp += imgw;
					}

					irr::c8 name[ 128 ];
					sprintf( name, "ttf%u_%u_%p", idx_, size, ( void * ) freetypeFont );
					irr::video::IImage *img = freetypeFont->Driver->createImageFromData( irr::video::ECF_A8R8G8B8, irr::core::dimension2d< irr::u32 >( imgw, imgh ), texd );
					setGlyphTextureFlags( freetypeFont->Driver );
					tex = freetypeFont->Driver->addTexture( name, img );
					img->drop();
					restoreTextureFlags( freetypeFont->Driver );
					delete[ ] texd;
					cached = true;
				}
			}
		}

		if( not FT_Load_Glyph( face, idx_, FT_LOAD_DEFAULT bitor FT_LOAD_RENDER ) ) { //FT_LOAD_NO_HINTING bitor FT_LOAD_RENDER bitor FT_LOAD_MONOCHROME ) ) {
			FT_GlyphSlot glyph = face->glyph;
			FT_Bitmap bits = glyph->bitmap;
			irr::u8 *pt = bits.buffer;
			top16 = glyph->bitmap_top;
			left16 = glyph->bitmap_left;
			imgw16 = 1;
			imgh16 = 1;
			texw16 = bits.width;
			texh16 = bits.rows;
			
			while( imgw16 < texw16 ) {
				imgw16 <<= 1;
			}
			
			while( imgh16 < texh16 ) {
				imgh16 <<= 1;
			}

			if( imgw16 > imgh16 ) {
				imgh16 = imgw16;
			} else {
				imgw16 = imgh16;
			}

			irr::s32 offx = left;
			irr::s32 offy = size - top;

			if( offx + texw > freetypeFont->LargestGlyph.Width ) {
				freetypeFont->LargestGlyph.Width = offx + texw;
			}

			if( offy + texh > freetypeFont->LargestGlyph.Height ) {
				freetypeFont->LargestGlyph.Height = offy + texh;
			}


			irr::u16 *texd16 = new irr::u16[ imgw16*imgh16 ];
			memset( texd16, 0, imgw16 * imgh16 * sizeof( irr::u16 ) );
			irr::u16 *texp16 = texd16;

			for( decltype( bits.rows ) y = 0; y < bits.rows; ++y ) {
				irr::u16 *rowp = texp16;

				for( decltype( bits.width ) x = 0; x < bits.width; ++x ) {
					if( pt[ y * bits.pitch + ( x / 8 ) ] bitand ( 0x80 >> ( x % 8 ) ) ) {
						*rowp = 0xffff;
					}

					++rowp;
				}

				texp16 += imgw16;
			}

			irr::c8 name[ 128 ];
			sprintf( name, "ttf%u_%u_%p_16", idx_, size, ( void * ) freetypeFont );
			irr::video::IImage *img = freetypeFont->Driver->createImageFromData( irr::video::ECF_A1R5G5B5, irr::core::dimension2d< irr::u32 >( imgw16, imgh16 ), texd16 );
			setGlyphTextureFlags( freetypeFont->Driver );
			tex16 = freetypeFont->Driver->addTexture( name, img );
			img->drop();
			restoreTextureFlags( freetypeFont->Driver );
	//		freetypeFont->Driver->makeColorKeyTexture(tex16,video::SColor(0,0,0,0));
			delete[ ] texd16;
		}
	} catch( std::exception &e ) {
		std::wcerr << L"Error in CGUITTGlyph::cache(): " << e.what() << std::endl;
	}
}

bool CGUITTGlyph::mTexFlag16 = false;
bool CGUITTGlyph::mTexFlag32 = true;
bool CGUITTGlyph::mTexFlagMip = false;

void CGUITTGlyph::setGlyphTextureFlags( irr::video::IVideoDriver* driver_ ) {
	try {
		mTexFlag16 = driver_->getTextureCreationFlag( irr::video::ETCF_ALWAYS_16_BIT );
		mTexFlag16 = driver_->getTextureCreationFlag( irr::video::ETCF_ALWAYS_32_BIT );
		mTexFlagMip = driver_->getTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS );
		driver_->setTextureCreationFlag( irr::video::ETCF_ALWAYS_16_BIT, false );
		driver_->setTextureCreationFlag( irr::video::ETCF_ALWAYS_16_BIT, true );
		driver_->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, false );
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTGlyph::setGlyphTextureFlags(): " << e.what() << std::endl;
	}
}

void CGUITTGlyph::restoreTextureFlags( irr::video::IVideoDriver* driver_ ) {
	try {
		driver_->setTextureCreationFlag( irr::video::ETCF_ALWAYS_16_BIT, mTexFlag16 );
		driver_->setTextureCreationFlag( irr::video::ETCF_ALWAYS_16_BIT, mTexFlag16 );
		driver_->setTextureCreationFlag( irr::video::ETCF_CREATE_MIP_MAPS, mTexFlagMip );
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTGlyph::restoreTextureFlags(): " << e.what() << std::endl;
	}
}

// --------------------------------------------------------
FT_Library	CGUITTFace::library  = 0;
int CGUITTFace::countClassObjects = 0;

CGUITTFace::CGUITTFace()
	: face( 0 ) {
	try {
		++countClassObjects;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTFace::CGUITTFace(): " << e.what() << std::endl;
	}
}

CGUITTFace::~CGUITTFace() {
	try {
		if( face )
			FT_Done_Face( face );

		--countClassObjects;
		assert( countClassObjects >= 0 );

		if( not countClassObjects and library ) {
			FT_Done_FreeType( library );
			library = 0;
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTFace::~CGUITTFace(): " << e.what() << std::endl;
	}
}

//! loads a font file
bool CGUITTFace::load( const irr::io::path& filename ) {
	try {
		if( not library ) {
			if( FT_Init_FreeType( &library ) ) {
				return false;
			}
		}

		irr::core::stringc ansiFilename( filename ); // path can be anything but freetype can only work with ANSI filenames

		if( FT_New_Face( library, ansiFilename.c_str(), 0, &face ) ) {
			return false;
		}

		return true;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUITTFace::load(): " << e.what() << std::endl;
		return false;
	}
}

// --------------------------------------------------------
//! constructor
CGUIFreetypeFont::CGUIFreetypeFont( irr::video::IVideoDriver* driver )
	: Driver( driver )
	, TrueTypeFace( 0 ) {
	try {
		#ifdef DEBUGFLAG
			setDebugName( "CGUIFreetypeFont" );
		#endif

		if( Driver )
			Driver->grab();

		AntiAlias = false;
		Transparency = false;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::CGUIFreetypeFont(): " << e.what() << std::endl;
	}
}



//! destructor
CGUIFreetypeFont::~CGUIFreetypeFont() {
	try {
		if( TrueTypeFace )
			TrueTypeFace->drop();

		if( Driver )
			Driver->drop();

		clearGlyphs();
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::~CGUIFreetypeFont(): " << e.what() << std::endl;
	}
}

bool CGUIFreetypeFont::attach( CGUITTFace *Face, irr::u32 size ) {
	try {
		if( not Driver or not Face )
			return false;

		Face->grab();

		if( TrueTypeFace )
			TrueTypeFace->drop();

		TrueTypeFace = Face;

		if( not TrueTypeFace )
			return false;

		clearGlyphs();
		Glyphs.reallocate( TrueTypeFace->face->num_glyphs );
		Glyphs.set_used( TrueTypeFace->face->num_glyphs );

		for( decltype( TrueTypeFace->face->num_glyphs ) i = 0; i < TrueTypeFace->face->num_glyphs; ++i ) {
			CGUITTGlyph * glyph = new CGUITTGlyph();

			glyph->size = size;
	//		glyph->cache((wchar_t)i + 1);

			Glyphs[ i ] = glyph;
		}

		// This is a workaround to get a probably ok height for getDimension. So we check a few extreme characters which usually make trouble.
		// Workaround appears unneeded: getDimension calls getGlyphByChar on every character of its input string anyway.
		/*getGlyphByChar( L'A' );
		getGlyphByChar( L'g' );
		getGlyphByChar( L'.' );
		getGlyphByChar( L'(' );*/

		//A more thorough workaround than above, this checks the height of every character the font has.
		/*FT_UInt index;
		FT_ULong code;
		FT_Get_First_Char( TrueTypeFace->face, &index );
		while( index not_eq 0 )
		{
			getGlyphByIndex( index );

			// fetch next character from font face
			code = FT_Get_Next_Char( TrueTypeFace->face, code, &index );
		}*/

		return	true;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::attach(): " << e.what() << std::endl;
		return false;
	}
}

void CGUIFreetypeFont::clearGlyphs() {
	try {
		for( decltype( Glyphs.size() ) i = 0; i < Glyphs.size(); ++i ) {
			if( Glyphs[ i ] ) {
				Glyphs[ i ]->drop();
			}

			Glyphs[ i ] = 0;
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::clearGlyphs(): " << e.what() << std::endl;
	}
}

irr::u32 CGUIFreetypeFont::getGlyphByChar( wchar_t c ) const {
	try {
		irr::u32 idx = FT_Get_Char_Index( TrueTypeFace->face, c );

		if( idx and not Glyphs[ idx - 1 ]->cached )
			Glyphs[ idx - 1 ]->cache( idx, this );

		return	idx;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::getGlyphByChar(): " << e.what() << std::endl;
		return 0;
	}
}

irr::u32 CGUIFreetypeFont::getGlyphByIndex( irr::u32 idx ) const {
	try {
		//u32 idx = FT_Get_Char_Index( TrueTypeFace->face, c );

		if( idx < Glyphs.size() and not Glyphs[ idx - 1 ]->cached )
			Glyphs[ idx - 1 ]->cache( idx, this );

		return	idx;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::getGlyphByChar(): " << e.what() << std::endl;
		return 0;
	}
}

//! returns the dimension of a text
irr::core::dimension2d< irr::u32 > CGUIFreetypeFont::getDimension( irr::core::stringw text ) const {
	return getDimension( text.c_str() );
}

//! returns the dimension of a text
irr::core::dimension2d< irr::u32 > CGUIFreetypeFont::getDimension( std::wstring text ) const {
	return getDimension( text.c_str() );
}

//! returns the dimension of a text
irr::core::dimension2d< irr::u32 > CGUIFreetypeFont::getDimension( const wchar_t* text ) const {
	try {
		irr::core::dimension2d< irr::u32 > dim( 0, Glyphs[ 0 ]->size );

		for( const wchar_t* p = text; *p; ++p ) {
			dim.Width += getWidthFromCharacter( *p );
		}

		// The correct solution might be working with TrueTypeFace->height but I can't figure out how to use units_per_EM
		// even if I know which FT_Render_Mode I used. I'm sure there is some way to figure that out, but I have to give up for now.
		// This works well enough as it is.
		if( TrueTypeFace and LargestGlyph.Height > dim.Height )
			dim.Height = LargestGlyph.Height;

		return dim;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::getDimension(): " << e.what() << std::endl;
		return irr::core::dimension2d< irr::u32 >();
	}
}


inline irr::u32 CGUIFreetypeFont::getWidthFromCharacter( wchar_t c ) const {
	try {
		irr::u16 n = getGlyphByChar( c );

		if( n > 0 ) {
			irr::u32 w = Glyphs[ n - 1 ]->texw;
			irr::u32 left = Glyphs[ n - 1 ]->left;

			if( w + left > 0 )
				return w + left;
		}

		if( c >= 0x2000 ) {
			return	Glyphs[ 0 ]->size;
		} else {
			return	Glyphs[ 0 ]->size / 2;
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::getWidthFromCharacter(): " << e.what() << std::endl;
		return 0;
	}
}


//! draws an text and clips it to the specified rectangle if wanted
void CGUIFreetypeFont::draw( const irr::core::stringw& textstring, const irr::core::rect< irr::s32 >& position, irr::video::SColor color, bool hcenter, bool vcenter, const irr::core::rect< irr::s32 >* clip ) {
	try {
		if( not Driver )
			return;

		irr::core::dimension2d< irr::s32 > textDimension;
		irr::core::position2d< irr::s32 > offset = position.UpperLeftCorner;

		const wchar_t * text = textstring.c_str();

		if( hcenter or vcenter ) {
			textDimension = getDimension( text );

			if( hcenter )
				offset.X = (( position.getWidth() - textDimension.Width ) >> 1 ) + offset.X;

			if( vcenter )
				offset.Y = (( position.getHeight() - textDimension.Height ) >> 1 ) + offset.Y;
		}

		while( *text ) {
			irr::u32 n = getGlyphByChar( *text );

			if( n > 0 ) {
				if( AntiAlias ) {
	//				irr::s32 imgw = Glyphs[ n-1 ]->imgw;
	//				irr::s32 imgh = Glyphs[ n-1 ]->imgh;
					irr::s32 texw = Glyphs[ n-1 ]->texw;
					irr::s32 texh = Glyphs[ n-1 ]->texh;
					irr::s32 offx = Glyphs[ n-1 ]->left;
					irr::s32 offy = Glyphs[ n-1 ]->size - Glyphs[ n-1 ]->top;

					if( Driver->getDriverType() not_eq irr::video::EDT_SOFTWARE ) {
						if( not Transparency )
							color.color or_eq 0xff000000;

						Driver->draw2DImage( Glyphs[ n-1 ]->tex, irr::core::position2d< irr::s32 >( offset.X + offx, offset.Y + offy ), irr::core::rect< irr::s32 >( 0, 0, texw, texh ), clip, color, true );
					} else {
						irr::s32 a = color.getAlpha();
						irr::s32 r = color.getRed();
						irr::s32 g = color.getGreen();
						irr::s32 b = color.getBlue();
						irr::u8 *pt = Glyphs[ n-1 ]->image;

						if( not Transparency )	a = 255;

						for( decltype( texh ) y = 0; y < texh; ++y ) {
							for( decltype( texw ) x = 0; x < texw; ++x ) {
								if( not clip or clip->isPointInside( irr::core::position2d< irr::s32 >( offset.X + x + offx, offset.Y + y + offy ) ) ) {
									if( *pt ) {
										Driver->draw2DRectangle( irr::video::SColor(( a * *pt ) / 255, r, g, b ), irr::core::rect< irr::s32 >( offset.X + x + offx, offset.Y + y + offy, offset.X + x + offx + 1, offset.Y + y + offy + 1 ) );
									}

									++pt;
								}
							}
						}
					}
				} else {
	//				irr::s32 imgw = Glyphs[ n-1 ]->imgw16;
	//				irr::s32 imgh = Glyphs[ n-1 ]->imgh16;
					irr::s32 texw = Glyphs[ n-1 ]->texw16;
					irr::s32 texh = Glyphs[ n-1 ]->texh16;
					irr::s32 offx = Glyphs[ n-1 ]->left16;
					irr::s32 offy = Glyphs[ n-1 ]->size - Glyphs[ n-1 ]->top16;

					if( not Transparency ) {
						color.color or_eq 0xff000000;
					}

					Driver->draw2DImage( Glyphs[ n-1 ]->tex16,
										 irr::core::position2d< irr::s32 >( offset.X + offx, offset.Y + offy ),
										 irr::core::rect< irr::s32 >( 0, 0, texw, texh ),
										 clip, color, true );
				}

				offset.X += getWidthFromCharacter( *text );
			} else {
				offset.X += getWidthFromCharacter( *text );
			}

			++text;
		}
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::draw(): " << e.what() << std::endl;
	}
}

//! Calculates the index of the character in the text which is on a specific position.
irr::s32  CGUIFreetypeFont::getCharacterFromPos( const wchar_t* text, irr::s32 pixel_x ) const {
	try {
		irr::s32 x = 0;
		irr::s32 idx = 0;

		while( text[ idx ] ) {
			x += getWidthFromCharacter( text[ idx ] );

			if( x >= pixel_x )
				return idx;

			++idx;
		}

		return -1;
	} catch ( std::exception &e ) {
		std::wcerr << L"Error in CGUIFreetypeFont::getCharacterFromPos(): " << e.what() << std::endl;
		return -1;
	}
}

#endif // #if COMPILE_WITH_FREETYPE
