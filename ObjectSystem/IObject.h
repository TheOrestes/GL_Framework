// ---------------------------------------------------------------------------------------
/** @class IObject
 * This is a general Interface for any Object.
 */

#pragma once

class IObject
{
public:
	IObject();
	~IObject();

	virtual void Init() = 0;
	virtual void Kill() = 0;
};

