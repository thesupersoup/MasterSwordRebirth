#ifndef MS_STEAM_HELPER_H
#define MS_STEAM_HELPER_H

namespace SteamHelper
{
	void Initialize();
	void Shutdown();
	void Think();
	void RunCallbacks();
}

class CSteamAPIContext;
class CSteamGameServerAPIContext;
class ISteamHTTP;

extern CSteamAPIContext* steamapicontext;
extern CSteamGameServerAPIContext* steamgameserverapicontext;
extern ISteamHTTP* steamhttpcontext;

#endif // MS_STEAM_HELPER_H