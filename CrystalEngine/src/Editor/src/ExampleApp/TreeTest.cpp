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

    static float DrawMinimumX = 0.f;

    void TreeTest::DFSDraw(TreeNode* node, int level, SpriteBatch* spriteBatch, GeometryRenderer* gRender)
    {
        Vector2i left = Vector2i(InitPadding + node->Left - DrawMinimumX, 700 - level * 32);
        Vector2i right = Vector2i(InitPadding + node->Right - DrawMinimumX, 700 - level * 32 + NodeSize);

        spriteBatch->DrawSlicedTexture(m_texture2D, SliceNormal,
            Bound2i(left, right), Color4f(1.f));
        for (auto child : node->Children)
        {
            DFSDraw(child, level + 1, spriteBatch, gRender);
        }

        Vector2i pcenter = Vector2i(InitPadding + node->Center() - DrawMinimumX, 700 - level * 32);
        for (auto child : node->Children)
        {
            Vector2i center = Vector2i(InitPadding + child->Center() - DrawMinimumX, 700 - level * 32 - 32 + NodeSize);
            gRender->DrawLine(pcenter, center, Color4f(1.f, 1.f, 0.f, 1.f));
        }
    }

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
        DFSDraw(m_root, 0, spriteBatch, gRender);
        gRender->End();
		spriteBatch->End();
	}

	void TreeTest::Exit()
	{}

	bool TreeTest::Paused()
	{
		return false;
	}


    void TreeTest::Dfs1(TreeNode* node)
    {
        float leftMostEdge = 0, rightMostEdge = 0;
        bool firstChild = true;
        node->LeftContour = nullptr;
        node->RightContour = nullptr;
        for (auto child : node->Children)
        {
            Dfs1(child);

            float maxLeft = 0.f;
            if (!node->RightContour)
            {
                node->LeftContour = child;
            }
            else
            {
                TreeNode* leftSubtreeRight = node->RightContour;
                TreeNode* rightSubtreeLeft = child;
                // Search for minimum left for additional node Q
                while (leftSubtreeRight && rightSubtreeLeft)
                {
                    float leftNodeLeftToCenterLeft = child->Left - rightSubtreeLeft->Left;
                    maxLeft = std::max(maxLeft, leftSubtreeRight->Right + leftNodeLeftToCenterLeft);

                    if (!leftSubtreeRight->RightContour || !rightSubtreeLeft->LeftContour)
                    {
                        break;
                    }
                    leftSubtreeRight = leftSubtreeRight->RightContour;
                    rightSubtreeLeft = rightSubtreeLeft->LeftContour;
                }

                // Merge contours
                if (leftSubtreeRight->RightContour)
                {
                    // If left subtree has extra layers, concat with right contour
                    rightSubtreeLeft->RightContour = leftSubtreeRight->RightContour;
                }
                if (rightSubtreeLeft->LeftContour)
                {
                    leftSubtreeRight->LeftContour = rightSubtreeLeft->LeftContour;
                }
            }

            node->RightContour = child;

            if (firstChild)
            {
                child->MoveToLeftBeginAt(maxLeft);
                leftMostEdge = child->Left;
                firstChild = false;
            }
            else
            {
                child->MoveToLeftBeginAt(maxLeft + NodeMinPadding);
            }

            rightMostEdge = child->Right;
        }

        float center = (leftMostEdge + rightMostEdge) / 2.f;
        node->Left = center - NodeSize / 2;
        node->Right = node->Left + NodeSize;
    }

    float TreeTest::DfsMinimum(TreeNode* node)
    {
        float minn = node->Left;
        for (auto child : node->Children)
        {
            minn = std::min(minn, DfsMinimum(child));
        }
        return minn;
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

        m_root = GenerateFromText("X[X[]X[X[X[]X[X[]X[X[]]]X[]X[X[X[]X[X[X[]X[X[]X[]X[]]X[]]X[]]]X[X[]X[]]X[]]]X[X[X[]]]X[]X[]]]");
            //GenerateFromText("X[X[]X[X[X[]X[X[]X[X[]]]X[]X[X[X[]X[X[X[]X[X[]X[]X[]]X[]]X[]]]X[X[]X[]]X[]]]X[X[X[]]]X[]X[]]]");

        Dfs1(m_root);
        DrawMinimumX = DfsMinimum(m_root);
        printf("F\n");
    }

}
