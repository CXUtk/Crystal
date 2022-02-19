
class TestObject;

class TestObject
{
public:
	int			Age;
	float		Weight;
	std::string	Name;
	bool		IsMale;

	constexpr static auto properties = std::make_tuple(
		property(&TestObject::Weight, "Weight"),
		property(&TestObject::Age, "Age")
	);
};
