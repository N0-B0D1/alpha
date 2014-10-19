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

#include <iostream>

#include "AlphaController.h"

namespace alpha
{
    AlphaController::AlphaController() { }
    AlphaController::~AlphaController() { }

    void AlphaController::Execute()
    {
        if (this->Initialize())
        {
            for (;;)
            {
                if (!this->Update())
                {
                    break;
                }
            }
        }
        //else
        //{
        //    // log error message
        //}

        if (!this->Shutdown())
        {
            // log error message
            return;
        }
    }

    bool AlphaController::Initialize()
    {
        // create graphcs
        // create input device manager
        // create game logic

        // setup timer/clock
        m_start = std::chrono::high_resolution_clock::now();

        return true;
    }

    bool AlphaController::Update()
    {
        static int loops = 1000;
        static const double sk_maxUpdateTime = 1.0f / 60.0f;
        static double s_accumulator = 0.0f;

        // update event manager
        // process any events for me

        // calculate loop time total and elapsed
        std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - m_start;
        double currentTime = (double)(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 100000.0f);
        double elapsedTime = currentTime - m_timeLastFrame;
        /*if (elapsedTime > 0.25) {
            elapsedTime = 0.25;
        }*/
        m_timeLastFrame = currentTime;
        s_accumulator += elapsedTime;
        std::cout << "time: " << currentTime << " [" << elapsedTime << "]" << std::endl;

        // update systems
        double delta = 0.0f;
        while (s_accumulator >= sk_maxUpdateTime)
        {
            m_logic.Update(currentTime, sk_maxUpdateTime);
            m_graphics.Update(currentTime, sk_maxUpdateTime);

            delta += sk_maxUpdateTime;
            s_accumulator -= sk_maxUpdateTime;
        }



        // update the time last frame to now
        // We want to essentially ignore the time taken to update
        // and let the renderer "produce" time that can be "consumed" on the next update
        now = std::chrono::high_resolution_clock::now();
        elapsed = now - m_start;
        m_timeLastFrame = (double)(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 100000.0f);

        // render after updates complete
        m_graphics.Render();

        loops -= 1;
        return loops > 0;
    }

    bool AlphaController::Shutdown()
    {
        // destroy systems in reverse order
        // shutdown needs to be smart about deleting things that might not exist
        // in case something failed during initialization, and we are only half built

        return true;
    }

    void InitiateAlpha()
    {
        AlphaController controller;
        controller.Execute();
    }
}