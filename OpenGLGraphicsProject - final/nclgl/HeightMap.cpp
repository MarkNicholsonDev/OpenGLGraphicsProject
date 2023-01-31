#include "HeightMap.h"
#include <iostream>

HeightMap::HeightMap(const std::string& name, Vector2 ts) {
	int iWidth, iHeight, iChans;
	SetTextureScale(ts);
	unsigned char* data = SOIL_load_image(name.c_str(), &iWidth, &iHeight, &iChans, 1);
	if (!data) {
		std::cout << "Heightmap can't load file!\n";
		return;
	}
	SetBlendmapScale(Vector2(1.0 / ((float)iWidth), 1.0 / ((float)iHeight)));

	numVertices = iWidth * iHeight;
	numIndices = (iWidth - 1) * (iHeight - 1) * 6;
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	Vector3 vertexScale = Vector3(6.0f, 3.0f, 6.0f);

	Vector2 scaleVector = Vector2((float)iWidth / vertexScale.x, (float)iHeight / vertexScale.z);
	SetScaleVector(scaleVector);

	for (int z = 0; z < iHeight; z++) {
		for (int x = 0; x < iWidth; x++) {
			int offset = (z * iWidth) + x;
			if (data[offset] > GetMaxHeight()) { SetMaxHeight(data[offset]); }

			vertices[offset] = Vector3(x, data[offset], z) * vertexScale;
			textureCoords[offset] = Vector2(x, z) * textureScale;
		}
	}

	SOIL_free_image_data(data);
	int i = 0;
	for (int z = 0; z < iHeight - 1; z++) {
		for (int x = 0; x < iWidth - 1; x++) {
			int a = (z * iWidth) + x;
			int b = (z * iWidth) + x + 1;
			int c = ((z + 1) * iWidth) + x + 1;
			int d = ((z + 1) * iWidth) + x;
			indices[i++] = a;
			indices[i++] = c;
			indices[i++] = b;

			indices[i++] = c;
			indices[i++] = a;
			indices[i++] = d;
		}
	}

	SetMinHeight(0.0f);
	GenerateNormals();
	GenerateTangents();
	BufferData();

	heightmapSize.x = vertexScale.x * (iWidth - 1);
	heightmapSize.y = vertexScale.y * 255.0f;
	heightmapSize.z = vertexScale.z * (iHeight - 1);
}