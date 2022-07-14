#include "steamhelper.h"
#include "strhelper.h"
#include "steam/steam_api.h"
#include "steam/steam_gameserver.h"
#include "msdllheaders.h"

// this context is not available on dedicated servers
// WARNING! always check if interfaces are available before using
static CSteamAPIContext s_SteamAPIContext;
CSteamAPIContext* steamapicontext = &s_SteamAPIContext;

// this context is not available on a pure client connected to a remote server.
// WARNING! always check if interfaces are available before using
static CSteamGameServerAPIContext s_SteamGameServerAPIContext;
CSteamGameServerAPIContext* steamgameserverapicontext = &s_SteamGameServerAPIContext;

ISteamHTTP* steamhttpcontext = NULL;
static bool g_bTryLoadSteamAPI = false;

void SteamHelper::Initialize()
{
	Shutdown();
	g_bTryLoadSteamAPI = true;
}

void SteamHelper::Shutdown()
{
	s_SteamAPIContext.Clear();
	s_SteamGameServerAPIContext.Clear();
	steamhttpcontext = NULL;
	g_bTryLoadSteamAPI = false;
}

void SteamHelper::Think()
{
	if (g_bTryLoadSteamAPI) // Load Steam API
	{
		g_bTryLoadSteamAPI = false;

		s_SteamAPIContext.Init();
		s_SteamGameServerAPIContext.Init();

		steamhttpcontext = (IS_DEDICATED_SERVER() ? steamgameserverapicontext->SteamHTTP() : steamapicontext->SteamHTTP());
	}
}

void SteamHelper::RunCallbacks()
{
	if (steamgameserverapicontext->SteamHTTP())
		SteamGameServer_RunCallbacks();

	if (steamapicontext->SteamHTTP())
		SteamAPI_RunCallbacks();
}