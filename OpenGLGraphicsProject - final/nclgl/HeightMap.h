#pragma once
#include <string>
#include "Mesh.h"

class HeightMap : public Mesh {
public:
	HeightMap(const std::string& name, Vector2 t);
	~HeightMap(void) {}
	Vector3 GetHeightmapSize() const { return heightmapSize; }

	float GetMinHeight() const { return minHeight; }
	float GetMaxHeight() const { return maxHeight; }
	void SetMinHeight(float y) { minHeight = y; }
	void SetMaxHeight(float y) { maxHeight = y; }
	void SetTextureScale(Vector2 t) { textureScale = t; };
	void SetBlendmapScale(Vector2 b) { blendmapScale = b; };
	void SetScaleVector(Vector2 s) { scaleVector = s; };
	Vector2 GetScaleVector() { return scaleVector; };


protected:
	Vector3 heightmapSize;
	Vector2 blendmapScale;
	Vector2 textureScale;
	Vector2 scaleVector; // To be passed into the fragment shader to scale between the heightmap
	float minHeight;// and the textures for the mesh
	float maxHeight;
};
