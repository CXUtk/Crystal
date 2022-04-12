#pragma once
#include "Crystal.h"
#include "Core/Utils/GameTimer.h"
#include "Platform/RHI/Interfaces.h"
#include "Resource/Config/ConfigManager.h"
#include <functional>



namespace crystal
{
    class Engine final
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

        // Platform
        IGameWindow* GetWindow() const;
        IGraphicsDevice* GetGraphicsDevice() const;
        IGraphicsContext* GetGraphicsContext() const;
        GraphicsAPIType	GetGraphicsAPIType() const;

        // Funtcion
        InputController* GetInputController() const;
        SpriteBatch* GetSpriteBatch() const;
        GeometryRenderer* GetGeometryRenderer() const;
        UIStateMachine* GetUIStateMachine() const;

        // Resource
        AssetManager* GetAssetManager() const;
        ConfigManager* GetConfigManager() const;

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
        std::unique_ptr<ConfigManager>          m_pConfigManager = nullptr;

        double		m_fpsCap = 0.0;			// Maximum FPS
        GameTimer	m_gameTimer{};		    // Game timer

        Engine();

        void m_Initialize();
        void ReadConfig();
        void LoadAssets();
    };
}