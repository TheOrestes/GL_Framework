
// ---------------------------------------------------------------------------------------
/** @class GameObject
 * This class is a base class for all the objects in the game. Every game object has a 
 * transformation associated with it. Each GameObject has a Name & unique ID. 
 * Every new object that is used in the game MUST be inherited from GameObject class.
 */

#pragma once

#include <string>
#include "IObject.h"

class GameObject : public IObject
{
public:
	GameObject();
	~GameObject();

	virtual void	Init();
	virtual void	Kill();
	virtual void	Update(float dt);

private:
	int				m_iID;
	std::string		m_strName;
};