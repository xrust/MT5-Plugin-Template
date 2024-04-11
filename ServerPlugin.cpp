//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "stdafx.h"
#include "PluginInstance.h"
//---
#define FILE_VERSION 1011
//+------------------------------------------------------------------+
//| Plugin description structure                                     |
//+------------------------------------------------------------------+
MTPluginInfo ExtPluginInfo= {
	FILE_VERSION,
	MTServerAPIVersion,
	L"CleanPluginProject",
	L"� 2024, ForexRust.",
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
	//--- �������� �������� �������� ����������
	memcpy( info.defaults, ExtPluginDefaults, sizeof( ExtPluginDefaults ) ); // copy default parameters values
	info.defaults_total = _countof( ExtPluginDefaults );
	//---
	return(MT_RET_OK);
}
//+------------------------------------------------------------------+
//| Plugin instance creation entry point                             |
//+------------------------------------------------------------------+
MTAPIENTRY MTAPIRES MTServerCreate( UINT apiversion, IMTServerPlugin **plugin ) {
	//--- ������� � �������������� ������� ���
	MTLogCreate( g_log, g_hmodule, "YMD", ".log", "", FILE_VERSION );
	CLogAppenderFunc _lf( galog, __FUNCTION__, true, false );
	///--- check parameters
	if( !plugin ){
		_lf.MsgA( LL_ERR, "plugin is Null, apiversion=%d", ( int )apiversion );
		return( MT_RET_ERR_PARAMS );
	}
	//--- create plugin instance
	if( ( ( *plugin ) = new( std::nothrow ) CPluginInstance() ) == NULL ){
		_lf.MsgA( LL_ERR, "create plugin object, apiversion=%d", ( int )apiversion );
		return( MT_RET_ERR_MEM );
	}
	//--- ok
	return( MT_RET_OK );
}
//+------------------------------------------------------------------+
