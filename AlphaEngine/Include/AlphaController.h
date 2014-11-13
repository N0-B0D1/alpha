#ifndef ALPHA_CONTROLLER_H
#define ALPHA_CONTROLLER_H

/**
Copyright 2014 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <chrono>
#include "Toolbox/Logger.h"

namespace alpha
{
    class EventManager;
    class LogicSystem;
    class GraphicsSystem;
    class AssetSystem;

    /**
     * The AlphaController is the main engine controller which handles the lifecycle of the engine.
     */
	class AlphaController
	{
	public:
        AlphaController();
        virtual ~AlphaController();

        void SetLogic(LogicSystem *pLogic);
        void Execute();
        
    private:
        // non-copyable
        AlphaController(const AlphaController&);
        AlphaController & operator=(const AlphaController&);

        bool Initialize();
        bool Update();
        bool Shutdown();

        /** main loop timer */
        //AlphaClock m_clock;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
        double m_timeLastFrame = 0.0f;
        double m_timeAccumulator = 0.0f;
        double sk_maxUpdateTime = 1.0f / 60.0f;

        /** Event management system */
        EventManager *m_pEvents;

        /** game logic system */
        LogicSystem *m_pLogic;

        /** Graphics render system */
        GraphicsSystem *m_pGraphics;

        /** Asset Management System */
        AssetSystem * m_pAssets;
	};

    template <class GameLogic> 
    int InitiateAlpha()
    {
        GameLogic *logic = new GameLogic();
        AlphaController controller;
        controller.SetLogic(logic);
        controller.Execute();
        return 0;
    }
}

#endif // ALPHA_CONTROLLER_H
