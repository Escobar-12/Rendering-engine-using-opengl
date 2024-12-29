#pragma once
#include "Mesh.h"
#include <vector>
#include <string>
#include <iostream>
#include <assimp/config.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct All
{
	std::vector<float> MeshVertices;
	std::vector<unsigned int> MeshIndeces;
	std::vector<Texture> MeshTextures;
};

class Model
{
private:
	
	std::string directory;

	void ProcessNode(aiNode* Node, const aiScene* Scene);
	Mesh ProcessMesh(aiMesh* OMesh, const aiScene* Scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	void loadModel(std::string const& path);
	unsigned int TextureFromFile(const char* path, const std::string& directory);
	Material loadMaterial(aiMaterial* mat);
public:
	
	std::vector<Texture> Loaded_Textures;
	std::vector<All> MeshesData;
	std::vector<Mesh> Meshes;
	Model(std::string Path);
	void Draw(unsigned int Program);
};

// -----------------
