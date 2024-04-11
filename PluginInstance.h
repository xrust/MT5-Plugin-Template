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
#include <StrA.h>
#include <StrW.h>
#include <IniFile.h>
#include <LogFile.h>
#include <MtLogCreate.h>
#include <TimeHelper.h>
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

