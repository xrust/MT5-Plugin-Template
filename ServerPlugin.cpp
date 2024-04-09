//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "PluginInstance.h"
//---

//+------------------------------------------------------------------+
//| Plugin description structure                                     |
//+------------------------------------------------------------------+
MTPluginInfo ExtPluginInfo= {
	101,
	MTServerAPIVersion,
	L"CleanPluginProject",
	L"© 2024, ForexRust.",
	L"MT5 Server Plugin Empty Template."
};
//+------------------------------------------------------------------+
//| Plugin description structure                                     |
//+------------------------------------------------------------------+
MTPluginParam ExtPluginDefaults[] =
{
	{ MTPluginParam::TYPE_STRING, L"SymbolsGroups", L",!*Forex*,!*Crypto*,*," },
};
//+------------------------------------------------------------------+
//| DLL Main Function                                                |
//+------------------------------------------------------------------+
BOOL APIENTRY DllMain(HMODULE hModule,DWORD reason,LPVOID lpReserved) {
//---
	switch(reason) {
	case DLL_PROCESS_ATTACH : g_hmodule = (HMODULE)hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return(TRUE);
}
//+------------------------------------------------------------------+
//| Plugin About entry function                                      |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTServerAbout(MTPluginInfo& info) {
	info=ExtPluginInfo;
	//--- копируем дефолные значения параметров
	memcpy( info.defaults, ExtPluginDefaults, sizeof( ExtPluginDefaults ) ); // copy default parameters values
	info.defaults_total = _countof( ExtPluginDefaults );
	//---
	return(MT_RET_OK);
}
//+------------------------------------------------------------------+
//| Plugin instance creation entry point                             |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTServerCreate( UINT apiversion, IMTServerPlugin **plugin ) {
	//--- check parameters
	if (!plugin) return(MT_RET_ERR_PARAMS);
	//--- create plugin instance
	if (((*plugin) = new(std::nothrow) CPluginInstance()) == NULL) return(MT_RET_ERR_MEM);
	//--- создаем и инициализируем главный лог
	CStrW fname = GetModuleName( g_hmodule );
	LogCreate( g_log, g_hmodule, L"YMD", L"log.log", fname );
	galog.Init( &g_log, fname );
	CLogAppenderFunc _lf( galog, __FUNCTION__, true, false );
	//---
	return(MT_RET_OK);
}
//+------------------------------------------------------------------+
