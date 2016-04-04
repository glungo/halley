#pragma once

namespace Halley
{
	class ResourceLoader;

	class Texture : public Resource
	{
	public:
		static std::unique_ptr<Texture> loadResource(ResourceLoader& loader);
		unsigned int getNativeId() const { return privateImpl; }

	protected:
		unsigned int privateImpl;
		Vector2i size;
	};
}
