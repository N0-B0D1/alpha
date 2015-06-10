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

#include "Logic/LogicSystem.h"
#include "Entities/EntityFactory.h"
#include "Entities/Entity.h"
#include "Toolbox/Logger.h"
#include "Assets/AssetSystem.h"
#include "Events/EventData_EntityCreated.h"
#include "Events/EventData_HIDKeyAction.h"
#include "FSA/StateMachine.h"
#include "HID/HIDContextManager.h"
#include "HID/HIDConstants.h"
#include "Audio/AudioSystem.h"
#include "Audio/Sound.h"

namespace alpha
{
    LogicSystem::LogicSystem()
        : AlphaSystem(60)
        , m_pEntityFactory(nullptr)
        , m_pAssets(nullptr)
        , m_pAudio(nullptr)
        , m_pHIDContextManager(nullptr)
    { }
    LogicSystem::~LogicSystem() { }

    bool LogicSystem::VInitialize()
    {
        m_pEntityFactory = new EntityFactory();
        if (!m_pEntityFactory)
        {
            LOG_ERR("LogicSystem: Failed to create EntityManager");
            return false;
        }

        // create input subscriber
        m_subHIDKeyAction = std::make_shared<EventDataSubscriber<EventData_HIDKeyAction>>();

        // setup context manager
        m_pHIDContextManager = new HIDContextManager();

        return true;
    }

    bool LogicSystem::VShutdown()
    {
        m_entities.clear();

        if (m_pEntityFactory)
        {
            delete m_pEntityFactory;
        }
        return true;
    }

    bool LogicSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        ReadHIDKeyActionSubscription();
        return true;
    }

    void LogicSystem::SetAssetSystem(AssetSystem * const pAssets)
    {
        m_pAssets = pAssets;
    }

    void LogicSystem::SetAudioSystem(AudioSystem * const pAudio)
    {
        m_pAudio = pAudio;
    }

    std::shared_ptr<Entity> LogicSystem::GetEntity(const unsigned long entityId)
    {
        auto it = m_entities.find(entityId);
        if (it != m_entities.end())
        {
            return it->second;
        }
        return std::shared_ptr<Entity>();
    }

    std::shared_ptr<Entity> LogicSystem::CreateEntity(const char * resource)
    {
        std::shared_ptr<Entity> new_entity = nullptr;

        // get the entity script resource specified, and make an entity with it
        if (m_pAssets != nullptr) 
        {
            auto asset = m_pAssets->GetAsset(resource);
            if (asset != nullptr)
            {
                new_entity = m_pEntityFactory->CreateEntity(asset);
                m_entities[new_entity->GetId()] = new_entity;
            }
        }

        // create an Entity Created event, and publish it to all subscribers
        std::shared_ptr<EventData_EntityCreated> event = std::make_shared<EventData_EntityCreated>(new_entity);
        m_pubEntityCreated.Publish(event);

        // might be nullptr
        return new_entity;
    }

    void LogicSystem::DestroyEntity(const unsigned long entityId)
    {
        m_entities.erase(m_entities.find(entityId));
    }

    std::weak_ptr<Sound> LogicSystem::CreateSound(const char * resource)
    {
        std::weak_ptr<Sound> new_sound = std::weak_ptr<Sound>();

        if (m_pAssets != nullptr)
        {
            auto asset = m_pAssets->GetAsset(resource);
            if (asset != nullptr)
            {
                if (m_pAudio != nullptr)
                {
                    new_sound = m_pAudio->CreateSound(asset);
                }
            }
        }

        return new_sound;
    }

    void LogicSystem::SubscribeToEntityCreated(std::shared_ptr<AEventDataSubscriber> pSubscriber)
    {
        m_pubEntityCreated.Subscribe(pSubscriber);
    }

    void LogicSystem::SubscribeToSetActiveCamera(std::shared_ptr<AEventDataSubscriber> pSubscriber)
    {
        m_pubSetActiveCamera.Subscribe(pSubscriber);
    }

    std::shared_ptr<AEventDataSubscriber> LogicSystem::GetHIDKeyActionSubscriber() const
    {
        return m_subHIDKeyAction;
    }

    void LogicSystem::ReadHIDKeyActionSubscription()
    {
        // read any published EventData_HIDKeyAction events that may have occured since the last update.
        while (std::shared_ptr<const EventData_HIDKeyAction> data = m_subHIDKeyAction->GetNextEvent())
        {
            bool pressed = data->GetPressed();
            HIDAction action = data->GetAction();
            HID device = data->GetDevice();

            switch (device)
            {
            case HID_KEYBOARD:
                if (pressed)
                {
                    m_pHIDContextManager->KeyboardButtonDown(&action);
                }
                else
                {
                    m_pHIDContextManager->KeyboardButtonUp(&action);
                }

                break;
            case HID_MOUSE:
                switch (action.raw)
                {
                case MA_X_AXIS:
                case MA_Y_AXIS:
                    m_pHIDContextManager->MouseMoved(&action, data->GetRelative(), data->GetAbsolute());
                    break;
                default:
                    if (pressed)
                    {
                        m_pHIDContextManager->MouseButtonDown(&action);
                    }
                    else
                    {
                        m_pHIDContextManager->MouseButtonUp(&action);
                    }
                    break;
                }

                break;
            }
        }
    }
}
