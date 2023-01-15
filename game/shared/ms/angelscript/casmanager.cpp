#include "casengine.h"
#include "addon/scriptmath.h"
#include "addon/scriptmathcomplex.h"
#include "addon/scriptstdstring.h"

CASManager::~CASManager()
{
	Shutdown();
}

bool CASManager::Initialize()
{
	m_ASEngine = asCreateScriptEngine();

	if (m_ASEngine)
		m_ASEngine.CreateContext();
	else
		return false;

	m_ASEngine->SetMessageCallback(asMETHOD(CASManager, OnMessageCallback), this, asCALL_THISCALL);
	m_ASEngine->SetTranslateAppExceptionCallback(asMETHOD(CASManager, OnTranslateAppExceptionCallback), this, asCALL_THISCALL);

	PostInitalize();

	return true;
}

void CASManager::PostInitalize()
{
	//place holder in case we want post initalization
}

void CASManager::Shutdown()
{
	if (m_ASEngine)
		m_ASEngine->ShutDownAndRelease();
}

void CASManager::RegisterAddons()
{
	RegisterStdString(m_ASEngine);
	RegisterScriptMath(m_ASEngine);
	RegisterScriptMathComplex(m_ASEngine);
}

void CASManager::RegisterAPI()
{

}

void CASManager::OnMessageCallback(const asSMessageInfo* msg)
{
	const char *type;

	switch(msg->type)
	{
	case asEMsgType::asMSGTYPE_ERROR:
		type = "ERR";
		break;
	case sEMsgType::asMSGTYPE_WARNING:
		type = "WARN";
		break;
	default:
	case asEMsgType::asMSGTYPE_INFORMATION:
		type = "INFO";
		break;
	}

	// The engine will often log information not related to a script by passing an empty section string and 0, 0 for the location.
	// Only prepend this information if it's relevant.
	char buffer[1024];
	if (msg->section && '\0' != msg->section[0])
	{
		snprintf(buffer, 1024, "%s: In section %s, %d:%d - %s", type, msg->section, msg->row, msg->message);
		ALERT(at_console, buffer);
	}
	else
	{
		snprintf(buffer, 1024, "%s: %s", type, msg->message);
		ALERT(at_console, buffer);
	}
}

void CASManager::OnTranslateAppExceptionCallback(asIScriptContext* context)
{
	// See https://www.angelcode.com/angelscript/sdk/docs/manual/doc_cpp_exceptions.html
	try
	{
		throw;
	}
	catch (const std::exception& e)
	{
		// Catch exceptions for logging, but don't allow scripts to catch them.
		context->SetException(e.what(), false);
	}
	catch (...)
	{
		// Rethrow so longjmp reaches its destination.
		throw;
	}
}

void CASManager::OnThrownExceptionCallback(asIScriptContext* context)
{

}