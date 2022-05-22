#pragma once
#include <Core/Math/Math.h>
#include <glm/gtx/transform.hpp>
#include <random>

namespace crystal
{
    using RNG = std::mt19937;
    constexpr bool SHOW_DEBUG = false;

    inline float UniformRandomFloat(RNG& mt)
    {
        return std::min(OneMinusEpsilon, mt() * 2.3283064365386963e-10f);
    }

    // Solve for Ax = b
    template <int N>
    inline bool matSolve(glm::mat<N, N, glm::f32, glm::packed_highp> A,
        glm::vec<N, float, glm::packed_highp> b, glm::vec<N, float, glm::packed_highp>& x)
    {
        for (int i = 0; i < N; i++)
        {
            float absMax = A[i][i];
            int maxR = i;
            for (int j = i + 1; j < N; j++)
            {
                float abs = std::abs(A[i][j]);
                if (abs > absMax)
                {
                    absMax = abs;
                    maxR = j;
                }
            }
            if (absMax < EPS) return false;
            for (int j = i; j < N; j++)
            {
                std::swap(A[j][i], A[j][maxR]);
            }
            std::swap(b[i], b[maxR]);
            for (int j = i + 1; j < N; j++)
            {
                float K = A[i][j] / A[i][i];
                for (int k = i + 1; k < N; k++)
                {
                    A[k][j] -= A[k][i] * K;
                }
                b[j] -= b[i] * K;
            }
        }
        for (int i = N - 1; i >= 0; i--)
        {
            for (int j = i + 1; j < N; j++) b[i] -= A[j][i] * b[j];
            b[i] /= A[i][i];
        }
        x = b;
        return true;
    }

    inline glm::mat3 adjoint(const glm::mat3& m, float invDet)
    {
        glm::mat3 Inverse;
        Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invDet;
        Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * invDet;
        Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invDet;
        Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * invDet;
        Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * invDet;
        Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * invDet;
        Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * invDet;
        Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * invDet;
        Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;
        return Inverse;
    }

    template<glm::length_t L, typename T, enum glm::qualifier Q>
    inline T sqr(const glm::vec<L, T, Q>& v)
    {
        return glm::dot(v, v);
    }

    inline glm::vec3 GetUnitVectorUsingCos(float cosTheta, float phi)
    {
        float r = std::sqrt(std::max(0.f, 1 - cosTheta * cosTheta));
        return glm::vec3(r * std::cos(phi), cosTheta, -r * std::sin(phi));
    }

    inline glm::vec3 GetUnitVector(float theta, float phi)
    {
        float r = sin(theta);
        return glm::vec3(r * std::cos(phi), std::cos(theta), -r * std::sin(phi));
    }

    inline Vector2f GetThetaPhi(const Vector3f& dir)
    {
        float phi = std::atan2(-dir.z, dir.x);
        phi = (phi < 0) ? (phi + glm::two_pi<float>()) : phi;
        float theta = std::acos(dir.y);
        return Vector2f(theta, phi);
    }

    inline bool refract(Vector3f wo, float etaA, float etaB, Vector3f* wt)
    {
        float eta = etaA / etaB;
        wo *= -eta;
        auto sin2ThetaT = wo.x * wo.x + wo.z * wo.z;
        if (sin2ThetaT > 1.f) return false;
        wo.y = -std::sqrt(1.f - sin2ThetaT);
        *wt = wo;
        return true;
    }

    inline bool refract(const Vector3f& wo, const Vector3f& wh, float eta, glm::vec3* wt)
    {
        auto cosThetaO = glm::clamp(glm::dot(wo, wh), -1.f, 1.f);
        auto sin2ThetaO = std::max(0.f, 1.f - cosThetaO * cosThetaO);
        auto sin2ThetaI = eta * eta * sin2ThetaO;
        if (sin2ThetaI > 1.f) return false;

        auto cosThetaI = std::sqrt(1.f - sin2ThetaI);
        *wt = eta * -wo + (eta * cosThetaO - cosThetaI) * wh;
        return true;
    }

    inline Vector3f NextCosineUnitHemiSphere(const Vector2f& sample, float* pdf)
    {
        auto r = std::sqrt(sample.x);
        auto phi = sample.y * glm::two_pi<float>();

        auto x = r * std::cos(phi);
        auto z = r * std::sin(phi);
        auto y = std::sqrt(1.0f - r * r);
        *pdf = y / glm::pi<float>();
        return Vector3f(x, y, z);
    }

    inline glm::vec3 NextUnitHemiSphere(glm::vec2 sample, float* pdf)
    {
        auto y = sample.x;
        auto phi = sample.y * glm::two_pi<float>();

        float r = std::sqrt(1 - y * y);
        auto x = r * std::cos(phi);
        auto z = r * std::sin(phi);

        *pdf = 1.0f / glm::two_pi<float>();
        return glm::vec3(x, y, z);
    }


    inline glm::vec3 NextUnitSphere(glm::vec2 sample, float* pdf)
    {
        auto y = 2.f * sample.x - 1.f;
        auto phi = sample.y * glm::two_pi<float>();

        float r = std::sqrt(1 - y * y);
        auto x = r * std::cos(phi);
        auto z = r * std::sin(phi);

        *pdf = .5f / glm::two_pi<float>();
        return glm::vec3(x, y, z);
    }

    inline Vector3f UniformSampleCone(const Vector3f& sample, Float cosThetaMax, float* pdf)
    {
        Float cosTheta = 1.0f + sample.x * (1 - cosThetaMax);
        Float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
        Float phi = sample.y * glm::two_pi<float>();
        *pdf = 1.f / (glm::two_pi<float>() * (1 - cosThetaMax));
        return Vector3f(std::cos(phi) * sinTheta, cosTheta, -std::sin(phi) * sinTheta);
    }

    inline Matrix3f BuildTNB(const Vector3f& N)
    {
        Vector3f n = glm::normalize(N);
        Vector3f v = std::fabs(glm::dot(n, Vector3f(0, 0, 1))) < 1e-3 ? Vector3f(1, 0, 0) : Vector3f(0, 0, 1);
        Vector3f T = glm::normalize(glm::cross(v, n));
        Vector3f B = glm::normalize(glm::cross(n, T));
        return Matrix3f(T, n, B);
    }

    template<typename T>
    inline void quickPrint(const T& value) {}

    template<>
    inline void quickPrint(const glm::vec3& value)
    {
        printf("%lf, %lf, %lf", value.x, value.y, value.z);
    }

    inline void FixVector(Vector3f& vector)
    {
        vector = glm::clamp(vector, Vector3f(-1.0), Vector3f(1.0));
    }

    inline float PowerHeuristic(int nf, float fPdf, int ng, float gPdf)
    {
        float f = nf * fPdf, g = ng * gPdf;
        return (f * f) / (f * f + g * g);
    }


    inline uint32_t ReverseBits32(uint32_t n)
    {
        n = (n << 16) | (n >> 16);
        n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
        n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
        n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
        n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
        return n;
    }

    inline uint64_t ReverseBits64(uint64_t n)
    {
        uint64_t n0 = ReverseBits32((uint32_t)n);
        uint64_t n1 = ReverseBits32((uint32_t)(n >> 32));
        return (n0 << 32) | n1;
    }

    template<glm::length_t L, typename T, enum glm::qualifier Q>
    inline void reportNaN(const glm::vec<L, T, Q>& v, const char* title, int lineNum, const char* fileName)
    {
        if (glm::isnan(v) != glm::vec<L, bool, Q>(false))
        {
            printf("NaN detected at file %s (file %s, line %d)\n", title, fileName, lineNum);
        }
    }

    inline void reportNaN(float v, const char* title, int lineNum, const char* fileName)
    {
        if (std::isnan(v))
        {
            printf("NaN detected at file %s (file %s, line %d)\n", title, fileName, lineNum);
        }
    }

    template<glm::length_t L, typename T, enum glm::qualifier Q>
    inline void reportINF(const glm::vec<L, T, Q>& v, const char* title, int lineNum, const char* fileName)
    {
        if (glm::isinf(v) != glm::vec<L, bool, Q>(false))
        {
            printf("INF detected at %s (file %s, line %d)\n", title, fileName, lineNum);
        }
    }

    inline void reportINF(float v, const char* title, int lineNum, const char* fileName)
    {
        if (std::isinf(v))
        {
            printf("INF detected at %s (file %s, line %d)\n", title, fileName, lineNum);
        }
    }

}

// Micro definitions
#define NAN_DETECT_V(vec, name) if constexpr(crystal::SHOW_DEBUG) {crystal::reportNaN(vec, name, __LINE__, __FILE__);}
#define INF_DETECT_V(vec, name) if constexpr(crystal::SHOW_DEBUG) {crystal::reportINF(vec, name, __LINE__, __FILE__);}

