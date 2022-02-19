#include <CrystalEngine/src/Engine.h>
#include <CrystalEngine/src/Tests/OrbitControllerTest.h>
#include <CrystalEngine/src/Tests/TriangleTest.h>
#include <CrystalEngine/src/Tests/QuadTest.h>
#include <CrystalEngine/src/Tests/RenderTargetTest.h>
#include <CrystalEngine/src/Core/Utils/Logger.h>
#include <SJson/SJson.hpp>
using namespace crystal;



#define PROPERTY(type, member) property(&type::member, #member)

template<typename Class, typename T>
struct PropertyImpl
{
	constexpr PropertyImpl(T Class::* aMember, const char* aName) : member{ aMember }, name{ aName } {}

	using Type = T;

	T Class::* member;
	const char* name;
};

template<typename Class, typename T>
constexpr auto property(T Class::* member, const char* name)
{
	return PropertyImpl<Class, T>{member, name};
}


struct TestObject
{
	int			Age;
	float		Weight;
	std::string	Name;
	bool		IsMale;

	//constexpr static auto properties = std::make_tuple(
	//	property(&TestObject::Weight, "Weight"),
	//	property(&Age, "Age")
	//);
};

//template<typename T>
//constexpr SJson::JsonNode toJson_s(T& v)
//{
//	return SJson::JsonNode(v);
//}
//
//template <typename T, T... S, typename F>
//constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
//{
//	(static_cast<void>(f(std::integral_constant<T, S>{})), ...);
//}
//
//template<typename Class>
//constexpr SJson::JsonNode toJson(const Class& v)
//{
//	SJson::JsonNode json;
//	// We first get the number of properties
//	constexpr auto nbProperties = std::tuple_size<decltype(Class::properties)>::value;
//
//	// We iterate on the index sequence of size `nbProperties`
//	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i) {
//		// get the property
//		constexpr auto property = std::get<i>(Class::properties);
//
//		// set the value to the member
//		json[property.name] = v.*(property.member);
//	});
//	return json;
//}

int main()
{
	//TestObject obj = {};
	//SJson::JsonNode json = toJson<TestObject>(obj);
	//SJson::JsonFormatOption options;
	//options.Inline = false;
	//options.KeysWithQuotes = true;
	//printf("%s\n", json.ToString(options));
	property(&TestObject::Weight, "Weight");
	Engine* engine = Engine::GetInstance();
	try
	{
		engine->Start(std::make_unique<QuadTest>());
	}
	catch (std::exception ex)
	{
		crystal::GlobalLogger::Log(SeverityLevel::Error, ex.what());
	}
	return 0;
}
