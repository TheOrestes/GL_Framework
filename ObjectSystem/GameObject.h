
// ---------------------------------------------------------------------------------------
/** @class GameObject
 * This class is a base class for all the objects in the game. Every game object has a 
 * transformation associated with it. Each GameObject has a Name & unique ID. 
 * Every new object that is used in the game MUST be inherited from GameObject class.
 */

#pragma once

#include <string>
#include "IObject.h"
#include "glm/glm.hpp"

class Model;
class GLSLShader;

class GameObject// : public IObject
{
public:
	GameObject(int id, const std::string& name, const std::string& path);
	virtual ~GameObject();

	virtual void			Init();
	virtual void			Kill();
	virtual void			Update(float dt);
	virtual void			Render();

	// Setter functions
	void					SetPosition(const glm::vec3& pos);
	void					SetRotation(const glm::vec3& axis, float angle);
	void					SetScale(const glm::vec3& sc);

private:

	int						m_iID;
	std::string				m_strName;
	std::string				m_strPath;

	GLSLShader*				m_pShader;
	Model*					m_pModel;

	// Transformation
	glm::vec3				m_vecPosition;
	glm::vec3				m_vecRotation;
	float					m_fAngle;
	glm::vec3				m_vecScale;
};