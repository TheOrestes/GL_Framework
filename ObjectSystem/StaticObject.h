
#pragma once

#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GameObject.h"

class Model;
struct Material;
class GLSLShader;

class StaticObject : public GameObject
{
public:
	StaticObject();
	StaticObject(const std::string& path, const std::string& shader);
	virtual ~StaticObject();

	virtual void			Init();
	virtual void			Kill();
	virtual void			Update(float dt);
	virtual void			Render();

	virtual void			SetPosition(const glm::vec3& pos);
	virtual void			SetRotation(const glm::vec3& axis, float angle);
	virtual void			SetScale(const glm::vec3& sc);

	inline void				SetPath(const std::string& path){ m_strPath = path; }
	inline void				SetShader(const std::string& shader){ m_strShader = shader; }

private:
	std::string				m_strPath;
	std::string				m_strShader;

	GLSLShader*				m_pShader;
	Model*					m_pModel;
	Material*				m_pMaterial;
};

