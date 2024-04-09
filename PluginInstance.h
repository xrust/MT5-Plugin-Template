//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#pragma once
//---
using namespace std;
//---
#include <cstring>
#include <vector>
#include <string>
//---
#include "Tools-MT45/StrA.h"
#include "Tools-MT45/StrW.h"
#include "Tools-MT45/IniFile.h"
#include "Tools-MT45/LogFile.h"
#include "Tools-MT45/TimeHelper.h"
#include <ULogFile.h>
//---
extern HMODULE g_hmodule;
extern CLogFile g_log;
extern CLogAppender galog;
//+------------------------------------------------------------------+
//| Plugin instance class                                            |
//+------------------------------------------------------------------+
class CPluginInstance : 
	public IMTServerPlugin,
	public IMTConPluginSink,
	public IMTTradeSink {
private:
	MTServerInfo		m_info;
	IMTServerAPI*		m_api;
	IMTConPlugin*		m_config;
	//---
	CULogFile log;
public:
	CPluginInstance(void);
	virtual          ~CPluginInstance(void);
	//--- IMTServerPlugin interface implementation
	virtual void      Release(void);
	virtual MTAPIRES  Start(IMTServerAPI* server);
	virtual MTAPIRES  Stop(void);
	//--- IMTConPluginSinc interface implementation
	virtual void      OnPluginUpdate(const IMTConPlugin* plugin);
	

private:
	MTAPIRES	ParametersRead( void );
};
//+------------------------------------------------------------------+
inline CStrW GetModulePath( const wchar_t *filename, HMODULE h_module = NULL ) {
	wchar_t buff[ _MAX_DIR ], drv[ _MAX_DRIVE ], dir[ _MAX_DIR ], fname[ _MAX_FNAME ], ext[ _MAX_EXT ];
	wchar_t path[ 512 ] = { 0 };
	//---
	GetModuleFileNameW( h_module, buff, sizeof( buff ) );
	_wsplitpath_s( buff, drv, dir, fname, ext );
	//---
	wcscat_s( path, 512, drv );
	wcscat_s( path, 512, dir );
	wcscat_s( path, 512, L"\\" );
	wcscat_s( path, 512, filename );
	//---
	return(path);
}
//+------------------------------------------------------------------+
inline CStrW GetModuleName( HMODULE h_module = NULL ) {
	wchar_t buff[ _MAX_DIR ], drv[ _MAX_DRIVE ], dir[ _MAX_DIR ], fname[ _MAX_FNAME ], ext[ _MAX_EXT ];
	//---
	GetModuleFileNameW( h_module, buff, sizeof( buff ) );
	_wsplitpath_s( buff, drv, dir, fname, ext );
	//---
	return(fname);
}
//+------------------------------------------------------------------+
inline void LogCreate( CLogFile &log, HMODULE h_module, LPCWSTR tm_format, LPCWSTR nm_format, LPCWSTR folder, bool start_msg = true, UINT mode = OPEN_EXISTING ) {
	CStrW path = GetModulePath( L"", h_module );
	//---
	if (!folder || folder != L"") {
		path.Append( folder );
		path.Append( L"\\" );
		_tmkdir( path.StrW() );
	}
	//---
	if (tm_format != L"") {
		path.Append( TimeStampFormat( time( 0 ), tm_format ).StrT() );
		path.Append( L"_" );
	}
	//---
	path.Append( nm_format );
	//---
	log.Open( path, mode, start_msg );
}
//+------------------------------------------------------------------+

