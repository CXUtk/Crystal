#pragma once
namespace crystal
{
	class IRasterState
	{
	public:
		virtual ~IRasterState() = 0 {};

        virtual void SetScissorState(bool enable) = 0;
        virtual bool GetScissorState() const = 0;

        virtual void SetScissorBound(const Bound2i& scissorBound) = 0;
        virtual Bound2i GetScissorBound() const = 0;
	};
}