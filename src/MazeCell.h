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
 * The MazeCell class represents one cell in the maze.
 */

#ifndef MAZECELL_H
#define MAZECELL_H

#ifdef WINDOWS
    #include <irrlicht.h>
#else
    #include <irrlicht/irrlicht.h>
#endif
#include "Integers.h"
#include "PreprocessorCommands.h"

class MazeCell {
	public:
		enum border_t : uint_fast8_t { ACIDPROOF, LOCK, WALL, NONE };
		MazeCell();
		virtual ~MazeCell();
		border_t getTop() const;
		void setTop( border_t val );
		border_t getLeft() const;
		void setLeft( border_t val );
		border_t getBottom() const;
		void setOriginalBottom( border_t val );
		border_t getRight() const;
		void setOriginalRight( border_t val );
		void setOriginalTop( border_t val );
		void setOriginalLeft( border_t val );
		border_t getOriginalTop() const; //For those solving algorithms that absolutely cannot be used on mazes that are no longer simply connected.
		border_t getOriginalLeft() const;
		void removeLocks();
		bool visited; //Used by the maze generation algorithm to ensure there are no loops, by canGetTo() to indicate which cells it's already look at, and during play for the players' own use.
		uint_fast16_t distanceFromStart;
		uint_fast16_t id;
		bool isDeadEnd() const;
		irr::video::SColor getVisitorColor() const;
		void setVisitorColor( irr::video::SColor color );
		bool hasLock();
		bool hasLeftLock() const;
		bool hasTopLock() const;
		bool topVisible;
		bool leftVisible;
		bool bottomVisible;
		bool rightVisible;
	protected:
	private:
		border_t top;
		border_t left;
		border_t bottom; //Useless except on maze borders and when checking if it's a dead end. Do not rely on this to be accurate.
		border_t right; //Ditto.
		border_t originalTop;
		border_t originalLeft;
		irr::video::SColor visitorColor;
};

#endif // MAZECELL_H
