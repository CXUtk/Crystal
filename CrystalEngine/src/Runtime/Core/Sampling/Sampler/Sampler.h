#pragma once
#include <Crystal.h>
#include <Core/Math/Geometry.h>
#include <random>
#include <algorithm>

namespace crystal
{
	class Sampler
	{
	public:
		//static std::shared_ptr<Sampler> LoadSampler(JsonNode_CPTR pSamplerNode,
		//	const config::ConfigInfo& configInfo);

		Sampler(int samplesPerPixel) : m_samplesPerPixel(samplesPerPixel) {}
        size_t GetSamplesPerPixel() const { return m_samplesPerPixel; }

		virtual ~Sampler() = 0 {};
		virtual float Get1D() = 0;
		virtual Point2f Get2D() = 0;
		virtual std::shared_ptr<Sampler> Clone(int seed) const = 0;

		virtual void Request1DArray(int N);
		virtual void Request2DArray(int N);
		virtual bool StartNextSample();
		virtual void StartPixel(const Point2i& pt, int startSample, int endSample = -1);

		float* Get1DArray(int N);
		Point2f* Get2DArray(int N);

	protected:
        size_t m_samplesPerPixel = 0, m_currentSampleIndex = 0;
        size_t m_startSampleIndex = 0, m_endSampleIndex = 0;
		Point2i m_currentPixel{};

		int m_array1DOffset = 0, m_array2DOffset = 0;

		std::vector<int> m_samples1DArraySizes, m_samples2DArraySizes;
		std::vector<std::vector<float>> m_sampleArray1D;
		std::vector<std::vector<Point2f>> m_sampleArray2D;
	};
}