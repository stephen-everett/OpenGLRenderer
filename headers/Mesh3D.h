#pragma once
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "ShaderProgram.h"
#include "Texture.h"

struct Vertex3D {
	float_t x;
	float_t y;
	float_t z;

	float_t nx;
	float_t ny;
	float_t nz;

	float_t u;
	float_t v;

	// add tx ty tz etc inside here and constructor

	Vertex3D(float_t px, float_t py, float_t pz, float_t normX, float_t normY, float_t normZ,
		float_t texU, float_t texV) :
		x(px), y(py), z(pz), nx(normX), ny(normY), nz(normZ), u(texU), v(texV) {}
};

/**
 * @brief Represents a mesh whose vertices have positions, normal vectors, and texture coordinates;
 * as well as a list of Textures to bind when rendering the mesh.
 */
class Mesh3D {
private:
	uint32_t m_vao;
	std::vector<Texture> m_textures;
	size_t m_vertexCount;
	size_t m_faceCount;

public:
	Mesh3D() = delete;

	
	/**
	 * @brief Construcst a Mesh3D using existing vectors of vertices and faces.
	*/
	Mesh3D(std::vector<Vertex3D>&& vertices, std::vector<uint32_t>&& faces, 
		Texture texture);

	Mesh3D(std::vector<Vertex3D>&& vertices, std::vector<uint32_t>&& faces,
		std::vector<Texture>&& textures);

	void addTexture(Texture texture);

	/**
	 * @brief Constructs a 1x1 square centered at the origin in world space.
	*/
	static Mesh3D square(const std::vector<Texture>& textures);
	/**
	 * @brief Constructs a 1x1x1 cube centered at the origin in world space.
	*/
	static Mesh3D cube(Texture texture);
	/**
	 * @brief Constructs the upper-left half of the 1x1 square centered at the origin.
	*/
	static Mesh3D triangle(Texture texture);

	/**
	 * @brief Renders the mesh to the given context.
	 */
	void render(sf::RenderWindow& window, ShaderProgram& program) const;
	
};
