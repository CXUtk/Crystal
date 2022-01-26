#include "CrystalTracer.h"

namespace tracer
{
	CrystalTracer::CrystalTracer()
	{
		printf("Construct\n");
	}
	CrystalTracer::~CrystalTracer()
	{
		printf("Destruct\n");
	}
	void CrystalTracer::Initialize()
	{
		printf("Init\n");
	}
	void CrystalTracer::Update(double deltaTime)
	{
	}
	void CrystalTracer::Draw(double deltaTime)
	{}
	void CrystalTracer::Exit()
	{
		printf("Exit\n");
	}
}