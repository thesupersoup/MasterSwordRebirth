#pragma once

#include <angelscript.h>
#include "groupfile.h"

class CASManager
{
public:
	CASManager(){ };
	~CASManager(){ };

	bool Initialize();
	void PostInitalize();
	void Shutdown();

	*asIScriptEngine GetEngine(){ return m_ASEngine; };
	*asIScriptContext GetContext(){ return m_ASContext; };
	*CGameGroupFile GetGroupFile(){ return m_GroupFile; };

	void RegisterAddons();
	void RegisterAPI();

private:
	asIScriptEngine *m_ASEngine;
	asIScriptContext *m_ASContext;
	CGameGroupFile *m_GroupFile;

	void OnMessageCallback(const asSMessageInfo* msg);
	void OnTranslateAppExceptionCallback(asIScriptContext* context);
	void OnThrownExceptionCallback(asIScriptContext* context);
}