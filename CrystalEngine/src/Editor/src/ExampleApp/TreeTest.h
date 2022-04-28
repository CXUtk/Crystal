#pragma once
#include <Application.h>
#include "Engine.h"
#include <Core/Math/Math.h>
#include "Platform/RHI/Interfaces.h"
#include "Tree/Tree.h"

namespace crystal
{
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
        TreeNode* m_root = nullptr;

		std::shared_ptr<IShaderProgram> m_pShader = nullptr;
		std::shared_ptr<IPipelineStateObject> m_PSO = nullptr;
		std::shared_ptr<ITexture2D> m_texture2D = nullptr;
		std::shared_ptr<ITexture2D> m_texture2D1 = nullptr;


        void InitTree();
        void DFSDraw(TreeNode* node, int level, SpriteBatch* spriteBatch, GeometryRenderer* gRender);

        float DfsMinimum(TreeNode* node);

        void Dfs1(TreeNode* node, int level);
        void PositionMethod1(TreeNode* node);

        void Dfs2(TreeNode* node);
        void PositionMethod2(TreeNode* node);
	};
}
