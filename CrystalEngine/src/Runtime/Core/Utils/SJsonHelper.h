#pragma once
#include <SJson/SJson.hpp>
#include <Core/Math/Geometry.h>

namespace SJson
{
    template<>
    inline crystal::Vector3f de_serialize(const JsonNode& node)
    {
        assert(node.GetType() == ValueType::Array);
        array_type vec = node.Get<array_type>();
        assert(vec.size() == 3);

        crystal::Vector3f result{};
        result[0] = vec[0].Get<crystal::Float>();
        result[1] = vec[1].Get<crystal::Float>();
        result[2] = vec[2].Get<crystal::Float>();
        return result;
    }

    inline SJson::JsonNode serialize(const crystal::Vector3f& v)
    {
        return SJson::array({ v.x, v.y, v.z });
    }

    template<>
    inline crystal::Quaternion de_serialize(const JsonNode& node)
    {
        assert(node.GetType() == ValueType::Array);
        array_type vec = node.Get<array_type>();
        assert(vec.size() == 4);

        crystal::Quaternion result{};
        result[0] = vec[0].Get<crystal::Float>();
        result[1] = vec[1].Get<crystal::Float>();
        result[2] = vec[2].Get<crystal::Float>();
        result[3] = vec[3].Get<crystal::Float>();
        return result;
    }

    inline SJson::JsonNode serialize(const crystal::Quaternion& q)
    {
        return SJson::array({ q.x, q.y, q.z, q.w });
    }
}