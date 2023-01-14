#include "casengine.h"
#include "addon/scriptmath.h"
#include "addon/scriptmathcomplex.h"
#include "addon/scriptstdstring.h"

bool CASManager::Initialize()
{
	m_ASEngine = asCreateScriptEngine();
	return true;
}

void CASManager::PostInitalize()
{

}

void CASManager::Shutdown()
{
	if (m_ASEngine)
		m_ASEngine->ShutDownAndRelease();
}

void RegisterAddons()
{
	RegisterStdString(m_ASEngine);
	RegisterScriptMath(m_ASEngine);
	RegisterScriptMathComplex(m_ASEngine);
}

void RegisterAPI()
{

}