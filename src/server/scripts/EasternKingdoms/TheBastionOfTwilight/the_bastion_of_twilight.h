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
    CREATURE_TWILIGHT_WHELP       = 48050,
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
    {-984.649963f,  -777.938354f, 438.592987f, 0.876615f}, //  0 : Calen
    {-996.034119f,  -674.310852f, 440.059662f, 4.635530f}, //  1 : Pulsing Twilight Egg - Left
    {-903.791565f,  -769.350952f, 440.954987f, 3.305641f}, //  2 : Pulsing Twilight Egg - Right
    {-1010.095154f, -769.764771f, 438.593140f, 5.838569f}, //  3 : Twilight Whelp 1 - A
    {-1010.195435f, -783.726807f, 438.593140f, 0.097302f}, //  4 : Twilight Whelp 1 - B
    {-991.100342f,  -802.459595f, 438.593073f, 1.212565f}, //  5 : Twilight Whelp 2 - A
    {-977.335999f,  -802.529236f, 438.593079f, 1.730928f}, //  6 : Twilight Whelp 2 - B
    {-958.439026f,  -783.782776f, 438.593079f, 2.673406f}, //  7 : Twilight Whelp 3 - A
    {-958.211792f,  -769.876282f, 438.593079f, 3.207477f}, //  8 : Twilight Whelp 3 - B
    {-977.209656f,  -751.108948f, 438.592957f, 4.373792f}, //  9 : Twilight Whelp 4 - A
    {-990.974243f,  -751.070374f, 438.592957f, 4.907859f}, // 10 : Twilight Whelp 4 - B
    {-999.449768f,  -708.023438f, 439.796204f, 4.863904f}, // 11 : Twilight Whelp 5 - Left  (Egg)
    {-919.777161f,  -773.681519f, 439.909698f, 3.316663f}, // 12 : Twilight Whelp 5 - Right (Egg)
};

#define POSITION_CALEN                      SinestraMinionPositions[0]
#define POSITION_PULSING_TWILIGHT_EGG_LEFT  SinestraMinionPositions[1]
#define POSITION_PULSING_TWILIGHT_EGG_RIGHT SinestraMinionPositions[2]
#define POSITION_TWILIGHT_WHELP_1_A         SinestraMinionPositions[3]
#define POSITION_TWILIGHT_WHELP_1_B         SinestraMinionPositions[4]
#define POSITION_TWILIGHT_WHELP_2_A         SinestraMinionPositions[5]
#define POSITION_TWILIGHT_WHELP_2_B         SinestraMinionPositions[6]
#define POSITION_TWILIGHT_WHELP_3_A         SinestraMinionPositions[7]
#define POSITION_TWILIGHT_WHELP_3_B         SinestraMinionPositions[8]
#define POSITION_TWILIGHT_WHELP_4_A         SinestraMinionPositions[9]
#define POSITION_TWILIGHT_WHELP_4_B         SinestraMinionPositions[10]
#define POSITION_TWILIGHT_WHELP_5_LEFT      SinestraMinionPositions[11]
#define POSITION_TWILIGHT_WHELP_5_RIGHT     SinestraMinionPositions[12]

#endif