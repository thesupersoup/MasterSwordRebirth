//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#include "wrect.h"
#include "cl_dll.h"
#include "vgui.h"
#include "vgui_loadtga.h"
#include "vgui_inputstream.h"
#include "ms/filesystem_shared.h"

//Deuplicated from msdebug.h
// #ifdef DEV_BUILD
// void *operator new(size_t size, const char *pszSourceFile, int LineNum);
// void operator delete(void *ptr, const char *pszSourceFile, int LineNum);
// #define msnew new (__FILE__, __LINE__)
// #else
// #define msnew new
// #endif

// ---------------------------------------------------------------------- //
// Helper class for loading tga files.
// ---------------------------------------------------------------------- //
class MemoryInputStream : public vgui::InputStream
{
public:
	MemoryInputStream() = default;

	MemoryInputStream(const uchar* data, std::size_t dataLen)
		: m_pData(data), m_DataLen(dataLen)
	{
	}

	void seekStart(bool& success) override
	{
		m_ReadPos = 0;
		success = true;
	}
	void seekRelative(int count, bool& success) override
	{
		m_ReadPos += count;
		success = true;
	}
	void seekEnd(bool& success) override
	{
		m_ReadPos = m_DataLen;
		success = true;
	}
	int getAvailable(bool& success) override
	{
		success = false;
		return 0;
	} // This is what vgui does for files...

	uchar readUChar(bool& success) override
	{
		if (m_ReadPos < m_DataLen)
		{
			success = true;
			uchar ret = m_pData[m_ReadPos];
			++m_ReadPos;
			return ret;
		}
		else
		{
			success = false;
			return 0;
		}
	}

	void readUChar(uchar* buf, int count, bool& success) override
	{
		for (int i = 0; i < count; i++)
			buf[i] = readUChar(success);
	}

	void close(bool& success) override
	{
		m_pData = nullptr;
		m_DataLen = m_ReadPos = 0;
	}

	const uchar* m_pData = nullptr;
	std::size_t m_DataLen = 0;
	std::size_t m_ReadPos = 0;
};

vgui::BitmapTGA *vgui_LoadTGA(char const *pFilename, bool invertAlpha)
{
	const auto fileContents = FileSystem_LoadFileIntoBuffer(pFilename, FileContentFormat::Binary);

	if (fileContents.empty())
	{
		return nullptr;
	}

	MemoryInputStream stream{reinterpret_cast<const uchar*>(fileContents.data()), fileContents.size()};

	vgui::BitmapTGA* pRet = new vgui::BitmapTGA(&stream, invertAlpha);

	return pRet;
}

vgui::BitmapTGA *vgui_LoadTGANoInvertAlpha(char const *pFilename)
{
	return vgui_LoadTGA(pFilename, false);
}
