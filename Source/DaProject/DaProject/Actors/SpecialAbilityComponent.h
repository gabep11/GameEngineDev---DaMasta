#pragma once
#include "AbilitiesInterface.h"

class SpecialAbilityComponent : public AbilitiesInterface 
{
	
public:
	SpecialAbilityComponent();
	~SpecialAbilityComponent() {}

	virtual void Activate() override;

};
