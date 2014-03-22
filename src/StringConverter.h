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

#ifndef STRINGCONVERTER_H
#define STRINGCONVERTER_H

#include "PreprocessorCommands.h"

#include <irrString.h>
#include <taglib/taglib.h>
#ifdef HAVE_STRING
#include <string>
#endif //HAVE_STRING

class StringConverter
{
	public:
		StringConverter();
		virtual ~StringConverter();
		//irr::core::stringw toIrrlichtStringW( TagLib::wstring input );
		irr::core::stringw toIrrlichtStringW( std::wstring input );
		TagLib::wstring toTaglibWString( irr::core::stringw input );
		TagLib::wstring toTaglibWString( std::wstring input );
		std::wstring toStdWString( irr::core::stringw input );
		std::wstring toStdWString( TagLib::wstring input );
		std::wstring toStdWString( std::string input );
		std::wstring toStdWString( char* input );
		std::wstring toStdWString( wchar_t* input );
		std::wstring toStdWString( const char* input );
		std::wstring toStdWString( const wchar_t* input );
		std::wstring toStdWString( int input );
		std::wstring toStdWString( long input );
		std::wstring toStdWString( long long input );
		std::wstring toStdWString( unsigned input );
		std::wstring toStdWString( unsigned long input );
		std::wstring toStdWString( unsigned long long input );
		std::wstring toStdWString( float input );
		std::wstring toStdWString( double input );
		std::wstring toStdWString( long double input );
		const wchar_t* toWCharArray( std::wstring input );
		const wchar_t* toWCharArray( irr::core::stringw input );
		const wchar_t* toWCharArray( char* input );
		const wchar_t* toWCharArray( const char* input );
	protected:
	private:
};

#endif // STRINGCONVERTER_H
