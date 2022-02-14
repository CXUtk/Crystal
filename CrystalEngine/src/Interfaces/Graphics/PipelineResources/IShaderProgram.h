#include "../GraphicsCommon.h"

namespace crystal
{
	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram() = 0 {};

		virtual void Apply() = 0;
		virtual void SetUniform1f(const std::string& name, float value) = 0;
		virtual void SetUniformMat4f(const std::string& name, const glm::mat4& value) = 0;
	};
}