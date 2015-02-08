// ---------------------------------------------------------------------------------------
/** @class IObject
 * This is a general Interface for any Object.
 */

#pragma once

class IObject
{
public:
	virtual ~IObject(){};

	virtual void	Init() = 0;
	virtual void	Kill() = 0;
	virtual void	Update(float dt) = 0;
	virtual void	Render() = 0;
};

