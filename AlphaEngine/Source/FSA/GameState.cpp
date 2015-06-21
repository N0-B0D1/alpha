/**
Copyright 2014-2015 Jason R. Wendlandt

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

#include <string>

#include "FSA/GameState.h"
#include "Logic/LogicSystem.h"
#include "Entities/Entity.h"
#include "Audio/Sound.h"
#include "HID/HIDContextManager.h"
#include "HID/HIDContext.h"
#include "Logic/LogicSystemEvents.h"

namespace alpha
{
    AGameState::~AGameState() { }

    void AGameState::VTransition(std::shared_ptr<AState> nextState)
    {
        std::shared_ptr<AGameState> state = std::dynamic_pointer_cast<AGameState>(nextState);
        state->SetLogic(m_pLogic);
    }

    std::shared_ptr<Entity> AGameState::GetEntity(const unsigned long entityId)
    {
        return m_pLogic->GetEntity(entityId);
    }
    std::shared_ptr<Entity> AGameState::CreateEntity(const char * resource)
    {
        return m_pLogic->CreateEntity(resource);
    }
    void AGameState::DestroyEntity(const unsigned long entityId)
    {
        m_pLogic->DestroyEntity(entityId);
    }

    std::weak_ptr<Sound> AGameState::CreateSound(const char * resource)
    {
        return m_pLogic->CreateSound(resource);
    }

    void AGameState::SetLogic(LogicSystem * const pLogic)
    {
        m_pLogic = pLogic;
    }

    void AGameState::SetActiveInputContext(HIDContext * context)
    {
        m_pLogic->m_pHIDContextManager->SetActiveContext(context);
    }

    void AGameState::BindAction(std::string action, std::function<void()> delegate)
    {
        m_pLogic->m_pHIDContextManager->BindAction(action, delegate);
    }

    void AGameState::BindState(std::string state, std::function<void(bool)> delegate)
    {
        m_pLogic->m_pHIDContextManager->BindState(state, delegate);
    }

    void AGameState::BindRange(std::string range, std::function<void(long, float)> delegate)
    {
        m_pLogic->m_pHIDContextManager->BindRange(range, delegate);
    }

    void AGameState::SetActiveCamera(std::shared_ptr<CameraComponent> pCameraComponent)
    {
        m_pLogic->PublishEvent(new Event_SetActiveCamera(pCameraComponent));
    }
}
