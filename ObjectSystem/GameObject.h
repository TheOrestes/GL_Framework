
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
	virtual void			SetCentroid(const glm::vec3& center);

	void					SetID(int id);

private:

	int						m_iID;

protected:
	glm::vec3				m_vecPosition;
	glm::vec3				m_vecRotation;
	float					m_fAngle;
	float					m_fScale;
	glm::mat4				m_matWorld;
	glm::vec3				m_vecCentroid;
};