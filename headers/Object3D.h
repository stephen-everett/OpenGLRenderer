#pragma once
#include <memory>
#include <vector>
#include "Mesh3D.h"
#include "ShaderProgram.h"
/**
 * @brief Represents an object placed in a 3D scene. The object is a node in an hierarchy of
 * objects representing a single 3D model. Each object in the hierarchy has its own position,
 * orientation, and scale, by which it uniformly transforms a list of meshes in the object.
*/
class Object3D {
private:
	// The object's list of meshes and children.
	std::vector<Mesh3D> m_meshes;
	std::vector<Object3D> m_children;

	// The object's position, orientation, and scale in world space.
	glm::vec3 m_position;
	glm::vec3 m_orientation;
	glm::vec3 m_scale;
	glm::vec3 m_center;

	// The object's cached local->world transformation matrix.
	glm::mat4 m_modelMatrix;
	glm::mat4 m_baseTransform;

	// Some objects from Assimp imports have a "name" field, useful for debugging.
	std::string m_name;

	// keep track of what maps are available
	bool spec;
	bool norm;
	bool tbn;

	// Recomputes the local->world transformation matrix.
	void rebuildModelMatrix();

public:
	// No default constructor; you must have a mesh to initialize an object.
	Object3D() = delete;

	Object3D(std::vector<Mesh3D>&& meshes);
	Object3D(std::vector<Mesh3D>&& meshes, const glm::mat4& baseTransform);

	// Simple accessors.
	const glm::vec3& getPosition() const;
	const glm::vec3& getOrientation() const;
	const glm::vec3& getScale() const;
	const glm::vec3& getCenter() const;
	const std::string& getName() const;

	bool hasSpec();
	bool hasNorm();
	bool hasTBN();

	// Child management.
	size_t numberOfChildren() const;
	const Object3D& getChild(size_t index) const;
	Object3D& getChild(size_t index);

	// Simple mutators.
	void setPosition(const glm::vec3& position);
	void setOrientation(const glm::vec3& orientation);
	void setScale(const glm::vec3& scale);
	void setCenter(const glm::vec3& center);
	void setName(const std::string& name);

	void setSpec(bool);
	void setNorm(bool);
	void setTBN(bool);

	// Transformations.
	void move(const glm::vec3& offset);
	void rotate(const glm::vec3& rotation);
	void grow(const glm::vec3& growth);
	void addChild(Object3D&& child);

	// Rendering.
	void render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const;
	void renderRecursive(sf::RenderWindow& window, ShaderProgram& shaderProgram, const glm::mat4& parentMatrix) const;

};