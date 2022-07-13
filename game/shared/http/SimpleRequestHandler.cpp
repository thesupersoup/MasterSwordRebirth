//
// Simple HTTP Request handling using httplib (https://github.com/yhirose/cpp-httplib)!
// Superceeded by Steam HTTP @ MSR due to Linux compatiblity...
// This lib requires GCC 4.9 and up, which breaks compat. with ReHLDS for now.
//

#define CPPHTTPLIB_NO_EXCEPTIONS

#include "httplib/httplib.h"
#include "SimpleRequestHandler.h"
#include <string>

static std::string g_pDataBuffer;
static httplib::Client* g_pHttpClient = NULL;

static bool IsValidResponse(const httplib::Result& result)
{
	g_pDataBuffer.clear();
	if (result && (result->status == HTTP_CODE_OK))
	{
		g_pDataBuffer = result->body;
		return true;
	}
	return false;
}

void HTTPRequestHandler::Initialize(const char* url)
{
	if ((g_pHttpClient != NULL) || !url || !url[0])
		return;
	g_pHttpClient = new httplib::Client(url);
}

void HTTPRequestHandler::Destroy(void)
{
	delete g_pHttpClient;
	g_pHttpClient = NULL;
}

bool HTTPRequestHandler::GetRequest(const char* url)
{
	auto result = g_pHttpClient->Get(url);
	return IsValidResponse(result);
}

bool HTTPRequestHandler::PostRequest(const char* url, const char* body)
{
	auto result = g_pHttpClient->Post(url, body, HTTP_CONTENT_TYPE);
	return IsValidResponse(result);
}

bool HTTPRequestHandler::PutRequest(const char* url, const char* body)
{
	auto result = g_pHttpClient->Put(url, body, HTTP_CONTENT_TYPE);
	return IsValidResponse(result);
}

bool HTTPRequestHandler::PatchRequest(const char* url)
{
	auto result = g_pHttpClient->Patch(url);
	return IsValidResponse(result);
}

bool HTTPRequestHandler::DeleteRequest(const char* url)
{
	auto result = g_pHttpClient->Delete(url);
	return IsValidResponse(result);
}