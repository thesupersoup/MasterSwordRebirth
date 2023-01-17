//Credit to Solokiller https://github.com/SamVanheer/HLEnhanced/
#pragma once

#include "CASBaseModuleBuilder.h"

/**
*	Builder for the map script.
*/
class CASMapModuleBuilder : public CASBaseModuleBuilder
{
public:
	/**
	*	Constructor.
	*	@param pszMapScript Map script to load.
	*/
	CASMapModuleBuilder( const char* const pszMapScript );

	bool DefineWords( CScriptBuilder& builder ) override;
};