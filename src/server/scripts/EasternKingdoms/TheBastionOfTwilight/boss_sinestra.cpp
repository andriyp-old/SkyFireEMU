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

enum SinestraTexts
{
    SAY_SINESTRA_AGGRO     = 0,
    SAY_SINESTRA_DEATH     = 1,
    SAY_SINESTRA_KILL_1    = 2,
    SAY_SINESTRA_KILL_2    = 3,
    SAY_SINESTRA_LOSING    = 4,
    SAY_SINESTRA_PHASE_1   = 5,
    SAY_SINESTRA_PHASE_2   = 6,
    SAY_SINESTRA_PHASE_3   = 7,
    SAY_SINESTRA_SPECIAL_1 = 8,
    SAY_SINESTRA_SPECIAL_2 = 9,
    SAY_SINESTRA_SPECIAL_3 = 10,
    SAY_SINESTRA_SPECIAL_4 = 11,
    SAY_SINESTRA_SPECIAL_5 = 12,
    SAY_SINESTRA_SPECIAL_6 = 13,
    SAY_SINESTRA_WINNING   = 14
};

enum CalenTexts
{
    SAY_CALEN_INTRO    = 0,
    SAY_CALEN_LOSING   = 1,
    SAY_CALEN_PHASE_2  = 3,
    SAY_CALEN_PHASE_3  = 4,
    SAY_CALEN_RECHARGE = 5,
    SAY_CALEN_WINNING  = 6,
    SAY_CALEN_DEATH    = 7
};

enum Spells
{
    // Sinestra :
    SPELL_DRAINED             = 89350,
    SPELL_CALL_FLAMES         = 95855,
    SPELL_WRACK               = 89421, // alt: 89435, 92955, 92956
    SPELL_FLAME_BREATH        = 18435, // alt: 92944    
    SPELL_TWILIGHT_BLAST      = 89280,
    SPELL_TWILIGHT_SPIT       = 89299, // H25: 92953
    SPELL_TWILIGHT_ESSENCE    = 88146,
    SPELL_SHADOW_PULSE        = 78649,
    SPELL_MANA_BARRIER        = 87299,
    SPELL_TWILIGHT_CARAPACE   = 87654,
    SPELL_TWILIGHT_EXTINCTION = 87945,
    // Shadow Orb :
    SPELL_TWILIGHT_SLICER     = 92851,
    // Calen :
    SPELL_FIERY_BARRIER       = 87231
};

enum Events
{
    EVENT_WRACK = 1,
    EVENT_FLAME_BREATH,
    EVENT_TWILIGHT_BLAST,
    EVENT_TWILIGHT_ORBS,
    EVENT_TWILIGHT_WHELPS,
    EVENT_TWILIGHT_SLICER,
    EVENT_TWILIGHT_EXTINCTION
};

class creature_shadow_orb : public CreatureScript
{
    public:
        creature_shadow_orb() : CreatureScript("creature_shadow_orb") { }

        typedef struct creature_shadow_orbAI : public ScriptedAI
        {
            Creature *mateOrb;

            creature_shadow_orbAI(Creature* creature)
                : ScriptedAI(creature)
                , mateOrb(NULL)
            {}

            inline void SetMate (Creature* mate)
            {
                mateOrb = mate;
            }

            void IsSummonedBy(Unit*)
            {
                events.ScheduleEvent(EVENT_TWILIGHT_SLICER, 5000);
            }
            
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                case EVENT_TWILIGHT_SLICER:
                    if (mateOrb)                        
                        DoCast(mateOrb, SPELL_TWILIGHT_SLICER, true);
                    break;
                }                
            }

        private:
            EventMap events;
        } AI;

        CreatureAI* GetAI(Creature* creature) const
        {
            return new creature_shadow_orbAI(creature);
        }
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
                Talk(SAY_SINESTRA_AGGRO);
                DoCast(SPELL_CALL_FLAMES);
                events.ScheduleEvent(EVENT_TWILIGHT_BLAST,  2000);
                events.ScheduleEvent(EVENT_WRACK,           10000);
                events.ScheduleEvent(EVENT_FLAME_BREATH,    20000);
                events.ScheduleEvent(EVENT_TWILIGHT_ORBS,   30000);
                events.ScheduleEvent(EVENT_TWILIGHT_WHELPS, 45000);
            }

            void JustDied(Unit* /*Kill*/)
            {
                _JustDied();
                Talk(SAY_SINESTRA_DEATH);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)                
                    Talk(RAND(SAY_SINESTRA_KILL_1, SAY_SINESTRA_KILL_2));                
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
                    Talk(SAY_SINESTRA_PHASE_2);
                    me->SetFullHealth();
                    DoCast(SPELL_MANA_BARRIER);                    
                    events.ScheduleEvent(EVENT_TWILIGHT_EXTINCTION, 2000);
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
                        events.RescheduleEvent(EVENT_TWILIGHT_BLAST, 5000);
                        break;

                    case EVENT_TWILIGHT_EXTINCTION:                        
                        DoSummon(CREATURE_CALEN, POSITION_CALEN);
                        DoCast(SPELL_TWILIGHT_EXTINCTION);
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
                            Creature *orbA = DoSummon(CREATURE_SHADOW_ORB, positionA, 15000, TEMPSUMMON_TIMED_DESPAWN),
                                     *orbB = DoSummon(CREATURE_SHADOW_ORB, positionB, 15000, TEMPSUMMON_TIMED_DESPAWN);
                            ((creature_shadow_orb::AI*)orbB->GetAI())->SetMate(orbA);
                        }

                        events.RescheduleEvent(EVENT_TWILIGHT_ORBS, 30000);
                        break;

                    case EVENT_TWILIGHT_WHELPS:
                        DoSummon(CREATURE_TWILIGHT_WHELP, RAND(POSITION_TWILIGHT_WHELP_1_A,    POSITION_TWILIGHT_WHELP_1_B));
                        DoSummon(CREATURE_TWILIGHT_WHELP, RAND(POSITION_TWILIGHT_WHELP_2_A,    POSITION_TWILIGHT_WHELP_2_B));
                        DoSummon(CREATURE_TWILIGHT_WHELP, RAND(POSITION_TWILIGHT_WHELP_3_A,    POSITION_TWILIGHT_WHELP_3_B));
                        DoSummon(CREATURE_TWILIGHT_WHELP, RAND(POSITION_TWILIGHT_WHELP_4_A,    POSITION_TWILIGHT_WHELP_4_B));
                        DoSummon(CREATURE_TWILIGHT_WHELP, RAND(POSITION_TWILIGHT_WHELP_5_LEFT, POSITION_TWILIGHT_WHELP_5_RIGHT));
                        events.RescheduleEvent(EVENT_TWILIGHT_WHELPS, 45000);
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

class creature_calen : public CreatureScript
{
    public:
        creature_calen() : CreatureScript("creature_calen") { }

        struct creature_calenAI : public ScriptedAI
        {
            creature_calenAI(Creature* creature)
                : ScriptedAI(creature)
            {}

            void EnterCombat(Unit*)
            {                
                Talk(SAY_CALEN_INTRO);
                DoCast(SPELL_FIERY_BARRIER);
            }

            void IsSummonedBy(Unit* /*summoner*/)
            {
                //DoCast/
                //me->MonsterYell(SAY_SINESTRA_PHASE_2_CALEN, 0, 0);
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
    new creature_shadow_orb();
    new creature_calen();
}