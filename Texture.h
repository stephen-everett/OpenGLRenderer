#pragma once
#include <string>
#include <filesystem>
#include <SFML/Graphics.hpp>

/**
 * @brief Represents a texture that has been loaded into VRAM, and is expected to be bound
 * to a sampler2D with a given sampler name in the fragment shader.
 */
struct Texture {
	// The ID of the texture, to be bound with glBindTexture when drawing a mesh.
	uint32_t textureId;
	// The name of the sampler2D uniform in the fragment shader that this texture will bind to.
	std::string samplerName;

	/**
	 * @brief Loads an SFML Image into VRAM and returns a Texture object identifying it.
	 */
	static Texture loadImage(const sf::Image& texture, const std::string& samplerName) {
		uint32_t texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.getSize().x, texture.getSize().y, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, texture.getPixelsPtr());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture{ texId, samplerName };
	}
};