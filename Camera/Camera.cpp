
#include "Camera.h"

//////////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	position = glm::vec3(0.0f, 2.0f, 10.0f);
	up		 = glm::vec3(0.0f, 1.0f, 0.0f);
	rightVec = glm::vec3(1.0f, 0.0f, 0.0f);

	bPerspective = true;	
	near = 0.1f;				
	far = 100.0f;
	fov = 45.0f;	// in degrees

	Init();
}

//////////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
void Camera::Init()
{
	// Create View Matrix
	viewMatrix = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), up);

	// By default, initialize Perspective Projection matrix...
	InitPerspectiveProjection();
}

//////////////////////////////////////////////////////////////////////////////////////////
void Camera::InitPerspectiveProjection()
{
	// Create Perspective Projection
	projMatrix = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 1000.0f);

	/*top = near * tan((glm::pi<float>()/180) * (fov/2));
	bottom = -top;
	right = top * 1.3333f;			// top * aspect
	left = -right;


	// Create Column major matrix!
	float mat[16];

	mat[0] = (2*near)/(right-left);		mat[4] = 0;							mat[8] = (right+left)/(right-left);		mat[12] = 0;
	mat[1] = 0;							mat[5] = (2*near)/(top-bottom);		mat[9] = (top+bottom)/(top-bottom);		mat[13] = 0;
	mat[2] = 0;							mat[6] = 0;							mat[10] = (near+far)/(near-far);		mat[14] = (2*near*far)/(near-far);
	mat[3] = 0;							mat[7] = 0;							mat[11] = -1;							mat[15] = 0;

	projMatrix = glm::make_mat4(mat);*/
}

//////////////////////////////////////////////////////////////////////////////////////////
void Camera::InitOrthographicProjection()
{
	// Create Orthographic Projection
	projMatrix = glm::ortho(-1.3333f, 1.3333f, -1.0f, 1.0f, -1.0f, 100.0f);//(45.0f, 800.0f/600.0f, 0.1f, 1000.0f);
}

void Camera::Update(float dt)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////////
glm::mat4x4 Camera::getViewMatrix()
{
	return viewMatrix;
}

//////////////////////////////////////////////////////////////////////////////////////////
glm::mat4x4 Camera::getProjectionMatrix()
{
	return projMatrix;
}

