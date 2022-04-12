#pragma once
#include "Crystal.h"
#include "Core/Utils/GameTimer.h"
#include "Platform/RHI/Interfaces.h"
#include "Resource/Config/EngineInitArguments.h"
#include <functional>

namespace crystal
{
    class DLLEXPROT Engine final
    {
    public:
        ~Engine();

        /**
         * @brief Get instance of current Engine singleton
         * @return pointer to Engine
        */
        static Engine* GetInstance()
        {
            static Engine engine;
            return &engine;
        }

        /**
         * @brief Get instance of current running window
         * @return
        */
        IGameWindow* GetWindow() const;

        /**
         * @brief GEt instance of engine input controller
         * @return
        */
        InputController* GetInputController() const;

        /**
         * @brief
         * @return
        */
        IGraphicsDevice* GetGraphicsDevice() const;

        /**
         * @brief
         * @return
        */
        IGraphicsContext* GetGraphicsContext() const;

        /**
         * @brief
         * @return
        */
        SpriteBatch* GetSpriteBatch() const;

        /**
         * @brief
         * @return
        */
        GeometryRenderer* GetGeometryRenderer() const;

        /**
         * @brief
         * @return
        */
        GraphicsAPIType	GetGraphicsAPIType() const;


        AssetManager* GetAssetManager() const;


        UIStateMachine* GetUIStateMachine() const;

        double GetCurrentTime() const;

        /**
         * @brief Take control on the application and start the main game loop
         * @param application
        */
        void Start(std::unique_ptr<Application>&& application);

    private:
        std::unique_ptr<Application>			m_application = nullptr;
        std::unique_ptr<IPlatformProvider>		m_platformProvider = nullptr;
        std::unique_ptr<InputController>		m_inputController = nullptr;

        std::unique_ptr<SpriteBatch>			m_spriteBatch = nullptr;
        std::unique_ptr<GeometryRenderer>	    m_pGeometryRenderer = nullptr;
        std::unique_ptr<UIStateMachine>         m_pUIStateMachine = nullptr;

        std::shared_ptr<AssetManager>			m_pAssetManager = nullptr;

        double		m_fpsCap = 0.0;			// Maximum FPS
        GameTimer	m_gameTimer{};		    // Game timer

        EngineInitArguments	m_initArgs{};

        Engine();

        void m_Initialize();
        void ReadConfig();
    };
}