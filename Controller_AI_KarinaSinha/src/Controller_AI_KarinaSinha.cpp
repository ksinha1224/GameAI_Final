// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Controller_AI_KarinaSinha.h"

#include "Constants.h"
#include "EntityStats.h"
#include "iPlayer.h"
#include "Vec2.h"

#include <stdlib.h>
#include <time.h>

static const Vec2 farLeft(LEFT_BRIDGE_CENTER_X, 0.f);
static const Vec2 farRight(RIGHT_BRIDGE_CENTER_X, 0.f);

static const Vec2 midLeft(LEFT_BRIDGE_CENTER_X, GAME_GRID_HEIGHT / 4.f);
static const Vec2 midRight(RIGHT_BRIDGE_CENTER_X, GAME_GRID_HEIGHT / 4.f);
static const Vec2 midCenter(GAME_GRID_WIDTH / 2, GAME_GRID_HEIGHT / 4);

static const Vec2 closeLeft(LEFT_BRIDGE_CENTER_X, GAME_GRID_HEIGHT / 2 - 3.f);
static const Vec2 closeRight(RIGHT_BRIDGE_CENTER_X, GAME_GRID_HEIGHT / 2 - 3.f);
static const Vec2 closeCenter(GAME_GRID_WIDTH / 2, GAME_GRID_HEIGHT / 2 - 3);

void Controller_AI_KarinaSinha::tick(float deltaTSec)
{
    assert(m_pPlayer);

    srand(time(NULL)); //generate random seed

    //MODES:
    // 0 = Offensive (attack as soon as possible)
    // 1 = Defensive (wait for player to make a move)

    //DIFFICULTY (each mode has all difficulties):
    // 0 = Easy
    // 1 = Medium
    // 2 = Hard

    if (!settingsInit) //this member boolean prevents the AI from generating new values every tick
    {
        mode = rand() % 2; //pick random number between 0 and 1
        difficulty = rand() % 3; //pick difficulty between 0 and 2
        settingsInit = true;
    }

    if (mode == 0)
    {
        startOffensive(difficulty);
        //std::cout << "Beginning an Offensive Strategy" << std::endl;
    }

    else if (mode == 1)
    {
        startDefensive(difficulty);
        //std::cout << "Beginning a Defensive Strategy" << std::endl;
    }
}

void Controller_AI_KarinaSinha::startOffensive(int d)
{
    int strategy = rand() % 3;
    selectOffense(d, strategy);
}

void Controller_AI_KarinaSinha::startDefensive(int d)
{
    int enemyNum = m_pPlayer->getNumOpponentMobs();
    bool isNorth = m_pPlayer->isNorth();

    bool mobPosNorth = false;

    /*
    if (d == 0)
    {
        std::cout << "I'm in EASY mode!" << std::endl;
    }

    else if (d == 1)
    {
        std::cout << "I'm in MEDIUM mode!" << std::endl;
    }

    else
    {
        std::cout << "I'm in HARD mode!" << std::endl;
    }

    std::cout << "Waiting for an enemy..." << std::endl;

    */

    if (enemyNum > 0)
    {
        if (isNorth)
        {
            mobPosNorth = m_pPlayer->getOpponentMob(0).m_Position.y < BRIDGE_CENTER_Y - 2.f;
        }

        else
        {
            mobPosNorth = m_pPlayer->getOpponentMob(0).m_Position.y > BRIDGE_CENTER_Y - 2.f;
        }

        if (mobPosNorth) //wait for player's first mob to get close to AI's side
        {
            std::cout << "Enemy detected!" << std::endl;

            for (int i = 0; i < enemyNum; i++)
            {
                float nX = m_pPlayer->getOpponentMob(i).m_Position.x;
                float nY = RIVER_TOP_Y - 4.f;
                Vec2 newMobPos(nX, nY);
                Vec2 mobPos_Game = newMobPos.Player2Game(isNorth);

                if (m_pPlayer->getOpponentMob(i).m_Stats.getMobType() == 0 && m_pPlayer->getElixir() > 5) //0 = Swordsmen
                {
                    if (d == 0)
                    {
                        //AI responds with an Archer, who is weak to the Swordsman
                        m_pPlayer->placeMob(iEntityStats::Archer, mobPos_Game);
                    }

                    else if (d == 1)
                    {
                        //AI responds with a Swordsman, who is neutral to the Swordsman
                        m_pPlayer->placeMob(iEntityStats::Swordsman, mobPos_Game);
                    }

                    else
                    {
                        //AI responds with a Giant, who is strong against the Swordsman
                        m_pPlayer->placeMob(iEntityStats::Giant, mobPos_Game);
                        std::cout << "Giants are better than Swordsmen!" << std::endl;
                    }
                }

                else if (m_pPlayer->getOpponentMob(i).m_Stats.getMobType() == 1 && m_pPlayer->getElixir() > 3) //1 = Archer
                {
                    if (d == 0)
                    {
                        //AI responds with a Giant, who is weak to the Archer
                        m_pPlayer->placeMob(iEntityStats::Giant, mobPos_Game);
                    }

                    else if (d == 1)
                    {
                        //AI responds with an Archer, who is neutral to the Archer
                        m_pPlayer->placeMob(iEntityStats::Archer, mobPos_Game);
                    }

                    else
                    {
                        //AI responds with a Swordsman, who is strong against the Archer
                        m_pPlayer->placeMob(iEntityStats::Swordsman, mobPos_Game);
                        std::cout << "Swordsmen are better than Archers!" << std::endl;
                    }

                }

                else if (m_pPlayer->getOpponentMob(i).m_Stats.getMobType() == 2 && m_pPlayer->getElixir() > 2) //2 = Giant
                {
                    if (d == 0)
                    {
                        //AI responds with a Swordsman, who is weak to the Giant
                        m_pPlayer->placeMob(iEntityStats::Swordsman, mobPos_Game);
                    }

                    else if (d == 1)
                    {
                        //AI responds with a Giant, who is neutral to the Giant
                        m_pPlayer->placeMob(iEntityStats::Giant, mobPos_Game);
                    }

                    else
                    {
                        //AI responds with an Archer, who is strong against the Giant
                        m_pPlayer->placeMob(iEntityStats::Archer, mobPos_Game);
                        std::cout << "Archers are better than Giants!" << std::endl;
                    }
                }
            }
        }
    }
}

void Controller_AI_KarinaSinha::selectOffense(int d, int s)
{
    bool isNorth = m_pPlayer->isNorth();

    if (d == 0)
    {
        //std::cout << "I'm in EASY mode!" << std::endl;

        if (s == 0)
        {
            //spawn 2 archers far away
            if (m_pPlayer->getElixir() > 6) //these placements only take 4 elixir but since the AI is on easy, it should take its time
            {
                std::cout << "I'll take my time..." << std::endl;
                Vec2 archerL_Game = farLeft.Player2Game(isNorth);
                Vec2 archerR_Game = farRight.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Archer, archerL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerR_Game);
            }
        }

        else
        {
            //spawn 1 archer and 1 swordsman far away
            if (m_pPlayer->getElixir() > 7)//these placements only take 5 elixir but since the AI is on easy, it should take its time
            {
                std::cout << "What to do...?" << std::endl;
                Vec2 archer_Game = farLeft.Player2Game(isNorth);
                Vec2 swordsman_Game = farRight.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Archer, archer_Game);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsman_Game);
            }
        }
    }

    else if (d == 1)
    {
        //std::cout << "I'm in MEDIUM mode!" << std::endl;

        if (s == 0)
        {
            //spawn 2 archers midfield
            if (m_pPlayer->getElixir() > 4) //the AI is now on medium so these units will spawn as soon as possible
            {
                std::cout << "I should hurry..." << std::endl;
                Vec2 archerL_Game = midLeft.Player2Game(isNorth);
                Vec2 archerR_Game = midRight.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Archer, archerL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerR_Game);
            }
        }
        
        else if (s == 1)
        {
            //spawn 2 swordsmen close to the river and 1 archer midfield (space out spawn)

            Vec2 swordsmanL_Game = closeLeft.Player2Game(isNorth);
            Vec2 swordsmanR_Game = closeRight.Player2Game(isNorth);
            Vec2 archer_Game = midCenter.Player2Game(isNorth);
            int varyTime = rand() % 5;

            if (m_pPlayer->getElixir() > 8) //the AI is now on medium so these units will spawn at varying times with different teams to mix it up
            {
                std::cout << "I'm getting better at this!" << std::endl;
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanL_Game);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanR_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archer_Game);
            }

            else if (m_pPlayer->getElixir() > 5 && varyTime == 0)
            {
                std::cout << "Maybe these units!" << std::endl;
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanR_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archer_Game);
            }

            else if (m_pPlayer->getElixir() > 5 && varyTime == 2)
            {
                std::cout << "What about these?" << std::endl;
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archer_Game);
            }

            else if (m_pPlayer->getElixir() > 5 && varyTime == 4)
            {
                std::cout << "This is a good combo!" << std::endl;
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, swordsmanR_Game);
            }
        }

        else
        {
            //spawn 2 archers midfield and 1 swordsman close to the river
            if (m_pPlayer->getElixir() > 7) //the AI is now on medium so these units will spawn as soon as possible
            {
                std::cout << "I should hurry..." << std::endl;
                Vec2 archerL_Game = midLeft.Player2Game(isNorth);
                Vec2 archerR_Game = midRight.Player2Game(isNorth);
                Vec2 swordsman_Game = closeCenter.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Archer, archerL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerR_Game);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsman_Game);
            }
        }
    }

    else
    {
        //std::cout << "I'm in HARD mode!" << std::endl;

        if (s == 0)
        {
            //spawn 1 giant and 2 archers all close to the river (space out spawn)
            Vec2 giant_Game = closeCenter.Player2Game(isNorth);
            Vec2 archerL_Game = closeLeft.Player2Game(isNorth);
            Vec2 archerR_Game = closeRight.Player2Game(isNorth);
            int varyTime = rand() % 2;

            if (m_pPlayer->getElixir() > 9) //the AI is now on hard so these units will spawn at varying times with different teams to mix it up
            {
                std::cout << "These will be hard to beat!" << std::endl;
                m_pPlayer->placeMob(iEntityStats::Giant, giant_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerR_Game);
            }

            else if (m_pPlayer->getElixir() > 7 && varyTime == 0)
            {
                std::cout << "I should use some Elixir..." << std::endl;
                m_pPlayer->placeMob(iEntityStats::Giant, giant_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerL_Game);
            }
        }

        else if (s == 1)
        {
            //spawn 1 giant and 1 swordsman both close to the river
            if (m_pPlayer->getElixir() > 8) //the AI is now on hard so these units will spawn as soon as possible
            {
                std::cout << "These units are tough!" << std::endl;
                Vec2 giant_Game = closeLeft.Player2Game(isNorth);
                Vec2 swordsman_Game = closeRight.Player2Game(isNorth);

                m_pPlayer->placeMob(iEntityStats::Giant, giant_Game);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsman_Game);
            }
        }

        else
        {
            //spawn 3 archers (2 midfield, 1 close to the river) and 1 swordsman close to the river (space out spawn)
            Vec2 archerMidL_Game = midLeft.Player2Game(isNorth);
            Vec2 archerMidR_Game = midRight.Player2Game(isNorth);
            Vec2 archerCenterL_Game = closeLeft.Player2Game(isNorth);
            Vec2 swordsmanCenterR_Game = closeRight.Player2Game(isNorth);
            int varyTime = rand() % 3;

            if (m_pPlayer->getElixir() > 9) //the AI is now on hard so these units will spawn at varying times with different teams to mix it up
            {
                std::cout << "No nonsense time!" << std::endl;
                m_pPlayer->placeMob(iEntityStats::Archer, archerMidL_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerMidR_Game);
                m_pPlayer->placeMob(iEntityStats::Archer, archerCenterL_Game);
                m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanCenterR_Game);
            }

            else if (m_pPlayer->getElixir() > 5)
            {
                if (varyTime == 1)
                {
                    std::cout << "My elixir's piling up..." << std::endl;
                    m_pPlayer->placeMob(iEntityStats::Archer, archerMidR_Game);
                    m_pPlayer->placeMob(iEntityStats::Archer, archerCenterL_Game);
                }

                else if (varyTime == 2)
                {
                    std::cout << "Variation is key!" << std::endl;
                    m_pPlayer->placeMob(iEntityStats::Archer, archerCenterL_Game);
                    m_pPlayer->placeMob(iEntityStats::Swordsman, swordsmanCenterR_Game);
                }
            }
        }
    }
}