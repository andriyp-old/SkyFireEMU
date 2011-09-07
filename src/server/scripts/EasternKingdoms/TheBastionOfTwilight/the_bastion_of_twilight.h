/*
* Copyright (C) 2010-2011 Project SkyFire <http://www.projectskyfire.org/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEF_THE_BASTION_OF_TWILIGHT_H_
#define DEF_THE_BASTION_OF_TWILIGHT_H_

enum Creatures
{
    CREATURE_SINESTRA             = 45213,
    CREATURE_PULSING_TWILIGHT_EGG = 46842, 
    CREATURE_SHADOW_ORB           = 40100,
    CREATURE_CALEN                = 46277
};

enum Data
{
    DATA_SINESTRA,
    MAX_ENCOUNTER
};

const Position SinestraMinionPositions [] =
{
    {-984.649963f, -777.938354f, 438.592987f, 0.876615f},  // 0 : Calen
    {-996.034119f, -674.310852f, 440.059662f, 4.635530f},  // 1 : Pulsing Twilight Egg - Left
    {-903.791565f, -769.350952f, 440.954987f, 3.305641f},  // 2 : Pulsing Twilight Egg - Right
};

#define POSITION_CALEN                      SinestraMinionPositions[0]
#define POSITION_PULSING_TWILIGHT_EGG_LEFT  SinestraMinionPositions[1]
#define POSITION_PULSING_TWILIGHT_EGG_RIGHT SinestraMinionPositions[2]

#endif