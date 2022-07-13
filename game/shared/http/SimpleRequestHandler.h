#ifndef HTTP_SIMPLE_REQUEST_HANDLER_H
#define HTTP_SIMPLE_REQUEST_HANDLER_H

#define HTTP_CODE_OK 200
#define HTTP_CONTENT_TYPE "application/json"

namespace HTTPRequestHandler
{
	void Initialize(const char* url);
	void Destroy(void);
	bool GetRequest(const char* url);
	bool PostRequest(const char* url, const char* body);
	bool PutRequest(const char* url, const char* body);
	bool PatchRequest(const char* url);
	bool DeleteRequest(const char* url);
}

#endif // HTTP_SIMPLE_REQUEST_HANDLER_H