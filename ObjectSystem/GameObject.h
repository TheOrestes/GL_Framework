
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

class GameObject : public IObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void			Init();
	virtual void			Kill();
	virtual void			Update(float dt);
	virtual void			Render();

	// Setter functions
	virtual void			SetPosition(const glm::vec3& pos);
	virtual void			SetRotation(const glm::vec3& axis, float angle);
	virtual void			SetScale(const glm::vec3& sc);

	void					SetID(int id);
	void					SetName(const std::string& name);

private:

	int						m_iID;
	std::string				m_strName;

protected:
	glm::vec3				m_vecPosition;
	glm::vec3				m_vecRotation;
	float					m_fAngle;
	glm::vec3				m_vecScale;
};