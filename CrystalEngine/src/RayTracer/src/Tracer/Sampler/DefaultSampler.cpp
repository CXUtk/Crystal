#include "DefaultSampler.h"
using namespace crystal;

namespace tracer
{
	DefaultSampler::DefaultSampler(int samplesPerPixel, int seed) : Sampler(samplesPerPixel), _rng(seed)
	{}

	DefaultSampler::~DefaultSampler()
	{}

	float DefaultSampler::Get1D()
	{
		return UniformRandomFloat(_rng);
	}

	Point2f DefaultSampler::Get2D()
	{
		return Point2f(UniformRandomFloat(_rng), UniformRandomFloat(_rng));
	}

	std::shared_ptr<Sampler> DefaultSampler::Clone(int seed) const
	{
		auto other = std::make_shared<DefaultSampler>(*this);
		other->_rng.seed(seed);
		return other;
	}
}