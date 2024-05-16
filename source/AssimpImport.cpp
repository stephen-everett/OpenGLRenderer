#include "AssimpImport.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <filesystem>
#include <unordered_map>
#include <algorithm>

const size_t FLOATS_PER_VERTEX = 3;
const size_t VERTICES_PER_FACE = 3;

std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture>& loadedTextures) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString name;
		mat->GetTexture(type, i, &name);
		std::filesystem::path texPath = modelPath.parent_path() / name.C_Str();

		auto existing = loadedTextures.find(texPath);
		if (existing != loadedTextures.end()) {
			textures.push_back(existing->second);
		}
		else {
			sf::Image image;
			image.loadFromFile(texPath.string());
			Texture tex = Texture::loadImage(image, typeName);
			textures.push_back(tex);
			loadedTextures.insert(std::make_pair(texPath, tex));
		}
	}
	return textures;
}

Mesh3D fromAssimpMesh(const aiMesh* mesh, const aiScene* scene, const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture>& loadedTextures) {
	std::vector<Vertex3D> vertices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		auto* tex = mesh->mTextureCoords[0];
		if (tex != nullptr) {
			vertices.emplace_back( mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
				mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z,
				//mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z,
				mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
		}
		else {
			vertices.push_back({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
				0, 0, 1, 0, 0 });
		}
	}

	std::vector<uint32_t> faces;
	faces.reserve(mesh->mNumFaces * VERTICES_PER_FACE);
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		faces.push_back(mesh->mFaces[i].mIndices[0]);
		faces.push_back(mesh->mFaces[i].mIndices[1]);
		faces.push_back(mesh->mFaces[i].mIndices[2]);
	}

	std::vector<Texture> textures = {};
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "baseTexture", modelPath, loadedTextures);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "specMap", modelPath, loadedTextures);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "normalMap", modelPath, loadedTextures);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		normalMaps = loadMaterialTextures(material,
			aiTextureType_NORMALS, "normalMap", modelPath, loadedTextures);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	auto m = Mesh3D(std::move(vertices), std::move(faces), std::move(textures));
	return m;
}


/*
std::shared_ptr<Object3D> assimpLoad(const std::string& path, bool flipTextureCoords) {
	Assimp::Importer importer;

	auto options = aiProcessPreset_TargetRealtime_MaxQuality;
	if (flipTextureCoords) {
		options |= aiProcess_FlipUVs;
	}
	const aiScene* scene = importer.ReadFile(path, options);

	// If the import failed, report it
	if (nullptr == scene) {
		throw std::runtime_error("Error loading assimp file: " + std::string(importer.GetErrorString()));
	}
	else {

	}

	// Troubleshooting for scene
	std::cout << "Successfully loaded scene with " << scene->mNumMeshes << " meshes and "
		<< scene->mNumMaterials << " materials." << std::endl;

	auto* mesh = scene->mMeshes[0];
	std::vector<std::pair<std::string, sf::Image>> textures;

	if (scene->HasMaterials()) {

		auto* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString name;
		material->Get(AI_MATKEY_NAME, name);

		material->GetTexture(aiTextureType_DIFFUSE, 0, &name);

		std::filesystem::path modelPath = path;
		std::filesystem::path texPath = modelPath.parent_path() / name.C_Str();
		sf::Image diffuse;
		diffuse.loadFromFile(texPath.string());
		textures.emplace_back(std::make_pair(std::string("diffuse"), std::move(diffuse)));

		if (!material->GetTexture(aiTextureType_HEIGHT, 0, &name)) {
			std::filesystem::path texPath = modelPath.parent_path() / name.C_Str();
			sf::Image normal;
			normal.loadFromFile(texPath.string());
			textures.emplace_back(std::make_pair(std::string("normalMap"), std::move(normal)));
		}
		if (!material->GetTexture(aiTextureType_SPECULAR, 0, &name)) {
			std::filesystem::path texPath = modelPath.parent_path() / name.C_Str();
			sf::Image specular;
			specular.loadFromFile(texPath.string());
			textures.emplace_back(std::make_pair(std::string("specMap"), std::move(specular)));
		}
	}
	//auto ret = Object3D(std::make_shared<Mesh3D>(fromAssimpMesh(scene->mMeshes[0], scene, textures)));
	std::vector<Mesh3D> meshes;
	std::unordered_map<std::filesystem::path, Texture> loadedTextures;
	auto ret = std::make_shared<Object3D>(processAssimpNode(scene->mRootNode, scene, std::filesystem::path(path), loadedTextures));

	// aiNode -> Object3D. the aiNode's mTransformation -> Object3D.m_baseTransform.
	// The list of meshes in aiNode -> Model3D.
	return std::move(ret);
}
*/

std::shared_ptr<Object3D> assimpLoad(const std::string& path, bool flipTextureCoords) {
	Assimp::Importer importer;

	// Set import options for Assimp
	auto options = aiProcessPreset_TargetRealtime_MaxQuality;
	if (flipTextureCoords) {
		options |= aiProcess_FlipUVs;
	}
	const aiScene* scene = importer.ReadFile(path, options);

	// If the import failed, report it
	if (nullptr == scene) {
		throw std::runtime_error("Error loading assimp file: " + std::string(importer.GetErrorString()));
	}

	// checks the amount of meshes and materials in the scene
	std::cout << "Successfully loaded scene with " << scene->mNumMeshes << " meshes and "
		<< scene->mNumMaterials << " materials." << std::endl;

	// Load textures
	std::vector<std::pair<std::string, sf::Image>> textures;

	// Load textures from materials
	if (scene->HasMaterials()) {
		// Loop through each material
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
			auto* material = scene->mMaterials[i];
			aiString name;
			material->Get(AI_MATKEY_NAME, name);
			std::cout << "Material Name: " << name.C_Str() << std::endl;

			// Process diffuse texture
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString texPath;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
					std::filesystem::path modelPath = path;
					std::filesystem::path textureFullPath = modelPath.parent_path() / texPath.C_Str();
					std::cout << "Diffuse Texture Path: " << textureFullPath.string() << std::endl;

					sf::Image diffuse;
					if (diffuse.loadFromFile(textureFullPath.string())) {
						textures.emplace_back(std::make_pair("diffuse", std::move(diffuse)));
					}
					else {
						std::cerr << "Failed to load diffuse texture: " << textureFullPath.string() << std::endl;
					}
				}
			}

			// Process normal map
			if (material->GetTextureCount(aiTextureType_HEIGHT) > 0) {
				aiString texPath;
				if (material->GetTexture(aiTextureType_HEIGHT, 0, &texPath) == AI_SUCCESS) {
					std::filesystem::path modelPath = path;
					std::filesystem::path textureFullPath = modelPath.parent_path() / texPath.C_Str();
					std::cout << "Normal Map Path: " << textureFullPath.string() << std::endl;

					sf::Image normal;
					if (normal.loadFromFile(textureFullPath.string())) {
						textures.emplace_back(std::make_pair("normalMap", std::move(normal)));
					}
					else {
						std::cerr << "Failed to load normal map: " << textureFullPath.string() << std::endl;
					}
				}
			}

			// Process specular map
			if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
				aiString texPath;
				if (material->GetTexture(aiTextureType_SPECULAR, 0, &texPath) == AI_SUCCESS) {
					std::filesystem::path modelPath = path;
					std::filesystem::path textureFullPath = modelPath.parent_path() / texPath.C_Str();
					std::cout << "Specular Map Path: " << textureFullPath.string() << std::endl;

					sf::Image specular;
					if (specular.loadFromFile(textureFullPath.string())) {
						textures.emplace_back(std::make_pair("specMap", std::move(specular)));
					}
					else {
						std::cerr << "Failed to load specular map: " << textureFullPath.string() << std::endl;
					}
				}
			}

			// Process ambient occlusion map
			if (material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION) > 0) {
				aiString texPath;
				if (material->GetTexture(aiTextureType_AMBIENT_OCCLUSION, 0, &texPath) == AI_SUCCESS) {
					std::filesystem::path modelPath = path;
					std::filesystem::path textureFullPath = modelPath.parent_path() / texPath.C_Str();
					std::cout << "AO Map Path: " << textureFullPath.string() << std::endl;

					sf::Image ao;
					if (ao.loadFromFile(textureFullPath.string())) {
						textures.emplace_back(std::make_pair("aoMap", std::move(ao)));
					}
					else {
						std::cerr << "Failed to load AO map: " << textureFullPath.string() << std::endl;
					}
				}
			}
		}
	}

	// Process the first mesh in the scene
	auto* mesh = scene->mMeshes[0];

	// Load the mesh
	std::unordered_map<std::filesystem::path, Texture> loadedTextures;
	
	// Process the node and then return an Object3D pointer
	auto ret = std::make_shared<Object3D>(processAssimpNode(scene->mRootNode, scene, std::filesystem::path(path), loadedTextures));

	return ret;
}


Object3D processAssimpNode(aiNode* node, const aiScene* scene,
	const std::filesystem::path& modelPath,
	std::unordered_map<std::filesystem::path, Texture>& loadedTextures) {

	// Load the aiNode's meshes.
	std::vector<Mesh3D> meshes;
	for (auto i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(fromAssimpMesh(mesh, scene, modelPath, loadedTextures));
	}

	std::vector<Texture> textures;
	for (auto& p : loadedTextures) {
		textures.push_back(p.second);
	}
	glm::mat4 baseTransform;
	for (auto i = 0; i < 4; i++) {
		for (auto j = 0; j < 4; j++) {
			baseTransform[i][j] = node->mTransformation[j][i];
		}
	}
	auto parent = Object3D(std::move(meshes), baseTransform);

	for (auto i = 0; i < node->mNumChildren; i++) {
		Object3D child = processAssimpNode(node->mChildren[i], scene, modelPath, loadedTextures);
		parent.addChild(std::move(child));
	}

	return parent;
}
