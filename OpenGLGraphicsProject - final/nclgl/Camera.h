#pragma once
#include "Matrix4.h"
#include "Vector3.h"

class Camera {
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
		roll = 0.0f;
	}
	
	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = 0.0f;
		this->position = position;
	}

	~Camera(void) {};

	void UpdateCamera(float dt = 1.0f);
	void CameraDemoRun(float dt = 1.0f);
	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	float GetYaw() const { return yaw; }
	void SetYaw(float y) { yaw = y; }

	float GetRoll() const { return roll; }
	void SetRoll(float r) { roll = r; }

	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }

protected:
	float yaw;
	float pitch;
	float roll;
	Vector3 position;
};

