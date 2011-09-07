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

enum Texts
{
    SAY_AGGRO     = 0,
    SAY_DEATH     = 1,
    SAY_KILL_1    = 2,
    SAY_KILL_2    = 3,
    SAY_LOSING    = 4,
    SAY_PHASE_1   = 5,
    SAY_PHASE_2   = 6,
    SAY_PHASE_3   = 7,
    SAY_SPECIAL_1 = 8,
    SAY_SPECIAL_2 = 9,
    SAY_SPECIAL_3 = 10,
    SAY_SPECIAL_4 = 11,
    SAY_SPECIAL_5 = 12,
    SAY_SPECIAL_6 = 13,
    SAY_WINNING   = 14
};

enum Spells
{
    SPELL_DRAINED           = 89350,
    SPELL_CALL_FLAMES       = 95855,
    SPELL_WRACK             = 89421, // alt: 89435, 92955, 92956
    SPELL_FLAME_BREATH      = 18435, // alt: 92944
    SPELL_TWILIGHT_SLICER   = 92852, // alt: 92954
    SPELL_TWILIGHT_BLAST    = 89280,
    SPELL_TWILIGHT_SPIT     = 89299, // alt: 92953
    SPELL_TWILIGHT_ESSENCE  = 88146,
    SPELL_SHADOW_PULSE      = 78649,
    SPELL_MANA_BARRIER      = 87299,
    SPELL_TWILIGHT_CARAPACE = 87654
};

enum Events
{
    EVENT_WRACK = 1,
    EVENT_FLAME_BREATH,
    EVENT_TWILIGHT_BLAST,
    EVENT_TWILIGHT_ORBS,
    EVENT_TWILIGHT_SLICER
};

class boss_sinestra : public CreatureScript
{
    public:
        boss_sinestra()
            : CreatureScript("boss_sinestra")
        {}

        struct boss_sinestraAI : public BossAI
        {
            uint32 phase;
            Creature *eggA,
                     *eggB;

            boss_sinestraAI(Creature* creature)
                : BossAI(creature, DATA_SINESTRA)
                , phase(1)
                , eggA(NULL)
                , eggB(NULL)
            {}

            void Reset() 
            {
                _Reset();
                
                me->SetHealth(me->GetMaxHealth() * 0.6);
                DoCast(me, SPELL_DRAINED);

                eggA = DoSummon(CREATURE_PULSING_TWILIGHT_EGG, POSITION_PULSING_TWILIGHT_EGG_LEFT);
                eggB = DoSummon(CREATURE_PULSING_TWILIGHT_EGG, POSITION_PULSING_TWILIGHT_EGG_RIGHT);
            }
    
            void EnterCombat(Unit* /*Ent*/)
            {                
                _EnterCombat();
                Talk(SAY_AGGRO);
                DoCast(SPELL_CALL_FLAMES);
                events.ScheduleEvent(EVENT_TWILIGHT_BLAST, 2000);
                events.ScheduleEvent(EVENT_WRACK,          10000);
                events.ScheduleEvent(EVENT_FLAME_BREATH,   20000);
                events.ScheduleEvent(EVENT_TWILIGHT_ORBS,  30000);
            }

            void JustDied(Unit* /*Kill*/)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)                
                    Talk(RAND(SAY_KILL_1, SAY_KILL_2));                
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);
                UpdatePhase();

                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                ExecutePendingEvent();                
                DoMeleeAttackIfReady();
            }
            
            inline void UpdatePhase()
            {
                if (phase == 1 && me->HealthBelowPct(30))
                {
                    phase = 2;
                    DoSummon(CREATURE_CALEN, POSITION_CALEN);
                    Talk(SAY_PHASE_2);
                    me->SetFullHealth();
                    DoCast(SPELL_MANA_BARRIER);
                }
                else if (phase == 2 && eggA->isDead() && eggB->isDead())
                {
                    phase = 3;
                    me->RemoveAllAuras();
                }
            }

            inline void ExecutePendingEvent()
            {
                if (uint32 eventId = events.ExecuteEvent())       
                {
                    switch (eventId)
                    {
                    case EVENT_WRACK:
                        DoCast(SPELL_WRACK);
                        break;

                    case EVENT_FLAME_BREATH:
                        DoCast(SPELL_FLAME_BREATH);
                        events.RescheduleEvent(EVENT_FLAME_BREATH, 20000);
                        break;

                    case EVENT_TWILIGHT_BLAST:
                        if (!me->getVictim()->IsWithinMeleeRange(me))
                            DoCastVictim(SPELL_TWILIGHT_BLAST);
                        events.RescheduleEvent(EVENT_TWILIGHT_BLAST, 6000);
                        break;

                    case EVENT_TWILIGHT_ORBS:
                        Unit *targetA = SelectTarget(SELECT_TARGET_RANDOM),
                             *targetB = targetA;

                        int threatListSize = me->getThreatManager().getThreatList().size();

                        while (targetA == targetB && threatListSize > 1)
                            targetB = SelectTarget(SELECT_TARGET_RANDOM);

                        if (targetA)
                        {
                            Position positionA, positionB;
                            targetA->GetPosition(&positionA);
                            targetB->GetPosition(&positionB);
                            DoSummon(CREATURE_SHADOW_ORB, positionA, 15000, TEMPSUMMON_TIMED_DESPAWN);
                            DoSummon(CREATURE_SHADOW_ORB, positionB, 15000, TEMPSUMMON_TIMED_DESPAWN);
                        }

                        events.RescheduleEvent(EVENT_TWILIGHT_ORBS, 30000);
                        break;
                    }        
                }
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sinestraAI(creature);
        }
};

class creature_pulsing_twilight_egg : public CreatureScript
{
    public:
        creature_pulsing_twilight_egg() : CreatureScript("creature_pulsing_twilight_egg") { }

        struct creature_pulsing_twilight_eggAI : public ScriptedAI
        {
            creature_pulsing_twilight_eggAI(Creature* creature)
                : ScriptedAI(creature)
            {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                me->AddAura(SPELL_TWILIGHT_CARAPACE, me);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new creature_pulsing_twilight_eggAI(creature);
        }
};

class creature_twilight_orb : public CreatureScript
{
    public:
        creature_twilight_orb() : CreatureScript("creature_twilight_orb") { }

        struct creature_twilight_orbAI : public ScriptedAI
        {
            creature_twilight_orbAI(Creature* creature)
                : ScriptedAI(creature)
            {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                events.ScheduleEvent(EVENT_TWILIGHT_SLICER, 10000);
            }
            
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                    case EVENT_TWILIGHT_SLICER:
                        // TODO
                        break;
                    }
                }
            }

        private:
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new creature_twilight_orbAI(creature);
        }
};

class creature_calen : public CreatureScript
{
    public:
        creature_calen() : CreatureScript("creature_calen") { }

        struct creature_calenAI : public ScriptedAI
        {
            creature_calenAI(Creature* creature)
                : ScriptedAI(creature)
            {}

            void IsSummonedBy(Unit* /*summoner*/)
            {
                //me->MonsterYell(SAY_PHASE_2_CALEN, 0, 0);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new creature_calenAI(creature);
        }
};

void AddSC_boss_sinestra()
{
    new boss_sinestra();
    new creature_pulsing_twilight_egg();
    new creature_twilight_orb();
    new creature_calen();
}