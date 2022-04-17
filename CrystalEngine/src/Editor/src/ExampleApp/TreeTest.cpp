#include "TreeTest.h"
#include <Core/Utils/Logger.h>
#include <Core/Math/Geometry.h>
#include "Function/Input/InputController.h"
#include <Core/Utils/GameTimer.h>

#include <Function/Render/RenderExports.h>

#include <Core/Utils/Misc.h>

#include "Resource/Asset/AssetManager.h"
#include <random>

namespace crystal
{
    void ClearTree(TreeNode* node)
    {
        for (auto child : node->Children)
        {
            ClearTree(child);
        }

        delete node;
    }

    static SliceInfo SliceNormal = { 1, 1, 1, 1, Slice_Nine };
    static constexpr int NodeSize = 16;
    static constexpr int NodeMinPadding = 8;
    static constexpr int InitPadding = 32;

    void TreeTest::DFSDraw(TreeNode* node, int level, SpriteBatch* spriteBatch, GeometryRenderer* gRender)
    {
        Vector2i left = Vector2i(InitPadding + node->Left, 700 - level * 32);
        Vector2i right = Vector2i(InitPadding + node->Right, 700 - level * 32 + NodeSize);

        spriteBatch->DrawSlicedTexture(m_texture2D, SliceNormal,
            Bound2i(left, right), Color4f(1.f));
        for (auto child : node->Children)
        {
            DFSDraw(child, level + 1, spriteBatch, gRender);
        }

        Vector2i pcenter = Vector2i(InitPadding + node->Center(), 700 - level * 32);
        for (auto child : node->Children)
        {
            Vector2i center = Vector2i(InitPadding + child->Center(), 700 - level * 32 - 32 + NodeSize);

            gRender->DrawLine(pcenter, center, Color4f(1.f, 1.f, 0.f, 1.f));
        }
    }

    void TreeTest::PushDown(TreeNode* node, float Mod, int level)
    {
        float mod = Mod + node->Mod;
        for (auto child : node->Children)
        {
            PushDown(child, mod, level + 1);
        }
        node->Left += mod;
        node->Right += mod;
        node->Mod = 0;
    }

    void TreeTest::PushDownRightOnly(TreeNode* node, float Mod, int level)
    {
        float mod = Mod + node->Mod;
        if (!node->Children.empty())
        {
            PushDown(node->Children.back(), mod, level + 1);
        }
        node->Left += mod;
        node->Right += mod;
        node->Mod = 0;
    }

    static TreeNode* Root = nullptr;
    static TreeNode* LastForLevel[100]{};

	TreeTest::TreeTest()
	{}

    TreeTest::~TreeTest()
	{}

	void TreeTest::Initialize()
	{
		crystal::GlobalLogger::Log(crystal::SeverityLevel::Debug, "CrystalTracer initialize");

		auto window = m_engine->GetWindow();
		auto windowSize = window->GetWindowSize();
		auto graphicsDevice = m_engine->GetGraphicsDevice();
        auto assetManager = m_engine->GetAssetManager();

        m_texture2D = assetManager->LoadAsset<ITexture2D>("ui:Panel");

        m_PSO = graphicsDevice->CreatePipelineStateObject();
        m_PSO->SetRasterState(graphicsDevice->CreateRasterStateFromTemplate(RasterStates::CullNone));
        m_PSO->SetBlendState(graphicsDevice->CreateBlendStateFromTemplate(BlendStates::AlphaBlend));
        m_PSO->SetDepthStencilState(graphicsDevice->CreateDepthStencilStateFromTemplate(DepthStencilStates::NoDepthTest));

        InitTree();
    }

	void TreeTest::Update(const crystal::GameTimer& gameTimer)
	{}




	void TreeTest::Draw(const crystal::GameTimer& gameTimer)
	{
		auto windowSize = m_engine->GetWindow()->GetWindowSize();
		auto graphicsContext = m_engine->GetGraphicsContext();
		auto graphicsDevice = m_engine->GetGraphicsDevice();
		auto spriteBatch = m_engine->GetSpriteBatch();
        auto gRender = m_engine->GetGeometryRenderer();
		graphicsContext->Clear(
			crystal::ClearOptions::CRYSTAL_CLEAR_TARGET
			| crystal::ClearOptions::CRYSTAL_CLEAR_DEPTH
			| crystal::ClearOptions::CRYSTAL_CLEAR_STENCIL,
			crystal::Color4f(0.f, 0.f, 0.f, 0.f), 1.0f, 0.f);

        spriteBatch->Begin(SpriteSortMode::Deferred, m_PSO);
        gRender->Begin();
        DFSDraw(Root, 0, spriteBatch, gRender);
        gRender->End();
		spriteBatch->End();
	}

	void TreeTest::Exit()
	{}

	bool TreeTest::Paused()
	{
		return false;
	}


    void TreeTest::Dfs1(TreeNode* node, int level)
    {
        float minLeft = std::numeric_limits<float>::infinity();
        float maxRight = -std::numeric_limits<float>::infinity();
        for (auto child : node->Children)
        {
            Dfs1(child, level + 1);

            minLeft = std::min(minLeft, child->Left);
            maxRight = std::max(maxRight, child->Right);
        }

        auto lastNode = LastForLevel[level];
        float lastRight = lastNode ? (lastNode->Right) : 0;
        float nextPos = lastRight + NodeMinPadding;
        if (node->Children.empty())
        {
            node->Left = nextPos;
            node->Right = node->Left + NodeSize;
            LastForLevel[level] = node;
            return;
        }

        float estimatedLeft = (minLeft + maxRight) * 0.5f - NodeSize / 2;
        float pos = std::max(estimatedLeft, nextPos);
        node->Mod = pos - estimatedLeft;
        node->Left = estimatedLeft;
        node->Right = node->Left + NodeSize;
        PushDown(node, 0, level);
        LastForLevel[level] = node;
    }

    TreeNode* GenerateFromText(const char* text)
    {
        TreeNode* node = new TreeNode();
        std::vector<TreeNode*> stack;
        stack.push_back(node);

        int i = 0;
        while (text[i])
        {
            if (text[i] == '[')
            {
                stack.push_back(new TreeNode());
            }
            else if (text[i] == ']')
            {
                auto snode = stack.back();
                stack.pop_back();
                stack.back()->Children.push_back(snode);
            }
            i++;
        }

        return node;
    }

    void TreeTest::InitTree()
    {

       /* Root = new TreeNode();
        std::vector<TreeNode*> nodes;
        nodes.push_back(Root);
        std::mt19937 mt;
        for (int i = 0; i < 64; i++)
        {
            int id = mt() % nodes.size();
            auto node = new TreeNode();
            nodes[id]->Children.push_back(node);
            nodes.push_back(node);
        }*/

        Root = GenerateFromText("X[X[]X[X[X[]X[X[]X[X[]]]X[]X[X[X[]X[X[X[]X[X[]X[]X[]]X[]]X[]]]X[X[]X[]]X[]]]X[X[X[]]]X[]X[]]]");

        Dfs1(Root, 0);
        printf("F\n");
    }

}
