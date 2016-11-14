
#pragma once

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GameObject.h"

#include "AntTweakBar.h"

class Model;
struct Material;
class GLSLShader;
class	BBoxCube;
struct	StaticObjectData;

class StaticObject : public GameObject
{
public:
	StaticObject();
	StaticObject(const StaticObjectData& data);
	virtual ~StaticObject();

	virtual void			Init();
	virtual void			Kill();
	virtual void			Update(float dt);
	virtual void			Render();
	virtual void			InitUI();

	virtual void			SetPosition(const glm::vec3& pos);
	virtual void			SetRotation(const glm::vec3& axis, float angle);
	virtual void			SetScale(const float & sc);
	virtual void			SetCentroid(const glm::vec3& center);
	virtual void			ShowBBox(bool flag);

			void			InitBBox(std::vector<glm::vec3> _vecVertexPositions);
			void			UpdateBBox(const glm::mat3& _matWorld);

	inline void				SetPath(const std::string& path){ m_strPath = path; }
	inline void				SetShader(const std::string& shader){ m_strShader = shader; }
	inline void				SetName(const std::string& name) { m_strName = name; };

private:
	std::string				m_strName;
	std::string				m_strPath;
	std::string				m_strShader;

	GLSLShader*				m_pShader;
	Model*					m_pModel;
	Material*				m_pMaterial;
	BBoxCube*				m_pBBoxCube;

	TwBar*					m_pUIBar;
};

