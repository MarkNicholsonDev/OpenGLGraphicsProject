#pragma once
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Mesh.h"
#include <vector>
#include <string>

class SceneNode {
public:
	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; };
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	//Left here purely for compatability for tutorials which use this - Use LoadTextures instead
	void SetTexture(GLuint tex) { texture = tex; }
	GLuint GetTexture() const { return texture; }
	void SetActiveTexture() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
	}

	void AddChild(SceneNode* s);
	void RemoveChild(SceneNode* s, int limbInd);

	virtual void Update(float dt);
	virtual void Draw(const OGLRenderer& r);

	//void LoadTextures() { for (GLuint t : textures) { SOIL_load_OGL_texture(TEXTUREDIR t, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS); } };
	void AddToTextures(vector<GLuint> in) { for (GLuint t : in) { textures.emplace_back(t); } }

	void AddShader(Shader* s) { shader = s; };
	Shader* GetShader() { return shader; };

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }

protected:
	SceneNode* parent;
	Mesh* mesh;
	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;

	float distanceFromCamera;
	float boundingRadius;

	GLuint texture; // Exists only for compatability for tutorials - Not to be used in project
	vector<GLuint> textures;
	Shader* shader;
	Vector4 colour;
	std::vector<SceneNode*> children;
};