#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>
#include "Object3D.h"
#include <iostream>

void Object3D::rebuildModelMatrix() {
	auto m = glm::translate(glm::mat4(1), m_position);
	m = glm::translate(m, m_center * m_scale);
	m = glm::rotate(m, m_orientation[2], glm::vec3(0, 0, 1));
	m = glm::rotate(m, m_orientation[0], glm::vec3(1, 0, 0));
	m = glm::rotate(m, m_orientation[1], glm::vec3(0, 1, 0));
	m = glm::scale(m, m_scale);
	m = glm::translate(m, -m_center);
	m = m * m_baseTransform;
	m_modelMatrix = m;
}

Object3D::Object3D(std::vector<Mesh3D>&& meshes)
	: Object3D(std::move(meshes), glm::mat4(1)) {
	rebuildModelMatrix();
}

Object3D::Object3D(std::vector<Mesh3D>&& meshes, const glm::mat4& baseTransform)
	: m_meshes(meshes), m_position(), m_orientation(), m_scale(1.0),
	m_center(), m_baseTransform(baseTransform)
{
	rebuildModelMatrix();
}

const glm::vec3& Object3D::getPosition() const {
	return m_position;
}

const glm::vec3& Object3D::getOrientation() const {
	return m_orientation;
}

const glm::vec3& Object3D::getScale() const {
	return m_scale;
}

/**
 * @brief Gets the center of the object's rotation.
 */
const glm::vec3& Object3D::getCenter() const {
	return m_center;
}

const std::string& Object3D::getName() const {
	return m_name;
}

size_t Object3D::numberOfChildren() const {
	return m_children.size();
}

const Object3D& Object3D::getChild(size_t index) const {
	return m_children[index];
}

Object3D& Object3D::getChild(size_t index) {
	return m_children[index];
}

void Object3D::setPosition(const glm::vec3& position) {
	m_position = position;
	rebuildModelMatrix();
}

void Object3D::setOrientation(const glm::vec3& orientation) {
	m_orientation = orientation;
	rebuildModelMatrix();
}

void Object3D::setScale(const glm::vec3& scale) {
	m_scale = scale;
	rebuildModelMatrix();
}

/**
 * @brief Sets the center point of the object's rotation, which is otherwise a rotation around 
   the origin in local space..
 */
void Object3D::setCenter(const glm::vec3& center)
{
	m_center = center;
}

void Object3D::setName(const std::string& name) {
	m_name = name;
}

void Object3D::move(const glm::vec3& offset) {
	m_position = m_position + offset;
	rebuildModelMatrix();
}

void Object3D::rotate(const glm::vec3& rotation) {
	m_orientation = m_orientation + rotation;
	rebuildModelMatrix();
}

void Object3D::grow(const glm::vec3& growth) {
	m_scale = m_scale * growth;
	rebuildModelMatrix();
}

void Object3D::addChild(Object3D&& child)
{
	m_children.emplace_back(child);
}

void Object3D::render(sf::RenderWindow& window, ShaderProgram& shaderProgram) const {
	renderRecursive(window, shaderProgram, glm::mat4(1));
}

/**
 * @brief Renders the object and its children, recursively.
 * @param parentMatrix the model matrix of this object's parent in the model hierarchy.
 */
void Object3D::renderRecursive(sf::RenderWindow& window, ShaderProgram& shaderProgram, const glm::mat4& parentMatrix) const {
	// This object's true model matrix is the combination of its parent's matrix and the object's matrix.
	glm::mat4 trueModel = parentMatrix * m_modelMatrix;
	shaderProgram.setUniform("model", trueModel);
	// Render each mesh in the object.
	for (auto& mesh : m_meshes) {
		mesh.render(window, shaderProgram);
	}
	// Render the children of the object.
	for (auto& child : m_children) {
		child.renderRecursive(window, shaderProgram, trueModel);
	}
}


void Object3D::setSpec(bool specValue) {
	spec = specValue;
}

void Object3D::setNorm(bool normValue) {
	norm = normValue;
}
void Object3D::setTBN(bool TBN) {
	tbn = TBN;
}

bool Object3D::hasSpec() {
	return spec;
}

bool Object3D::hasNorm() {
	return norm;
}

bool Object3D::hasTBN() {
	return tbn;
}