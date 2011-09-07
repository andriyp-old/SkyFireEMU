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

#include "ScriptPCH.h"
#include "the_bastion_of_twilight.h"

class instance_the_bastion_of_twilight: public InstanceMapScript
{
public:
    instance_the_bastion_of_twilight()
        : InstanceMapScript("instance_the_bastion_of_twilight", 671) 
    {}

    InstanceScript* GetInstanceScript(InstanceMap *map) const
    {
        return new instance_the_bastion_of_twilight_InstanceMapScript(map);
    }

    struct instance_the_bastion_of_twilight_InstanceMapScript: public InstanceScript
    {
        instance_the_bastion_of_twilight_InstanceMapScript(InstanceMap *map) : InstanceScript(map)
        {
            SetBossNumber(MAX_ENCOUNTER);
        }
    };
};

void AddSC_instance_the_bastion_of_twilight()
{
    new instance_the_bastion_of_twilight();
}