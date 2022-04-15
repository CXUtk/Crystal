#pragma once
#include <Application.h>
#include "Engine.h"
#include <Core/Utils/Math.h>
#include "Platform/RHI/Interfaces.h"

namespace crystal
{
    struct TreeNode
    {
        float Left, Right;
        float Mod;

        std::vector<TreeNode*> Children{};

        TreeNode()
        {
            Left = Right = 0;
            Mod = 0;
        }

        float Center() const { return (Left + Right) / 2.f; }
    };

	class TreeTest : public Application
	{
	public:
        TreeTest();
		virtual ~TreeTest() override;

		virtual void Initialize() override;

		virtual void Update(const crystal::GameTimer& gameTimer) override;

		virtual void Draw(const crystal::GameTimer& gameTimer) override;

		virtual void Exit() override;

		virtual bool Paused() override;

	private:
		bool m_renderPause = false;

		std::shared_ptr<IShaderProgram> m_pShader = nullptr;
		std::shared_ptr<IPipelineStateObject> m_PSO = nullptr;
		std::shared_ptr<ITexture2D> m_texture2D = nullptr;
		std::shared_ptr<ITexture2D> m_texture2D1 = nullptr;


        void InitTree();
        void DFSDraw(TreeNode* node, int level, SpriteBatch* spriteBatch, GeometryRenderer* gRender);
        void PushDown(TreeNode* node, float Mod, int level);
        void PushDownRightOnly(TreeNode* node, float Mod, int level);
        void Dfs1(TreeNode* node, int level);
	};
}
