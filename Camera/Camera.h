
#pragma once

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


class Camera
{
public:
	static Camera& getInstance()
	{
		static Camera instance;
		return instance;
	}

	~Camera();

	void			Init();
	void			InitPerspectiveProjection();
	void			InitOrthographicProjection();

	void			Update(float dt);

	glm::mat4x4		getViewMatrix();
	glm::mat4x4		getProjectionMatrix();

private:
	Camera();

	Camera(const Camera&);
	void operator=(const Camera&);

	// camera properties
	glm::vec3		position;
	glm::vec3		up;
	glm::vec3		rightVec;

	float			near;
	float			far;
	float			left;
	float			right;
	float			top;
	float			bottom;
	
	float			fov;

	bool			bPerspective;

	glm::mat4x4		viewMatrix;
	glm::mat4x4		projMatrix;
};