//+------------------------------------------------------------------+
//|                                  MetaTrader 5 API Server Example |
//|                   Copyright 2001-2017, MetaQuotes Software Corp. |
//|                                        http://www.metaquotes.net |
//+------------------------------------------------------------------+
#include "StdAfx.h"
#include "PluginInstance.h"
//----
HMODULE g_hmodule = nullptr;
CLogFile g_log;
CLogAppender galog;
//+------------------------------------------------------------------+
//| Constructor                                                      |
//+------------------------------------------------------------------+
CPluginInstance::CPluginInstance(void) :
	m_api(NULL),
	m_config(NULL) {
	ZeroMemory(&m_info,sizeof(m_info));
}
//+------------------------------------------------------------------+
//| Destructor                                                       |
//+------------------------------------------------------------------+
CPluginInstance::~CPluginInstance(void) {
	Stop();
}
//+------------------------------------------------------------------+
//| Plugin release function                                          |
//+------------------------------------------------------------------+
void CPluginInstance::Release(void) {
	delete this;
}
//+------------------------------------------------------------------+
//| Plugin start notification function                               |
//+------------------------------------------------------------------+
MTAPIRES CPluginInstance::Start(IMTServerAPI* api) {
	CLogAppenderFunc _lf( galog, __FUNCTION__, true, false );
	MTAPIRES retcode;
	//--- check SERVER API
	if (!api) {
		return(MT_RET_ERR_PARAMS);
	}
	//--- store API pointer
	m_api=api;
	//m_api->LoggerOut( MTLogOK, L"Start 0" );
	//--- receive server information
	if ((retcode=m_api->About(m_info))!=MT_RET_OK) {
		m_api->LoggerOut(MTLogOK, L"Server info failed [%d]", retcode);
		return retcode;
	}
	//---
	if ((retcode=m_api->PluginSubscribe(this)) != MT_RET_OK) {
		m_api->LoggerOut(MTLogOK, L"PluginSubscribe failed [%d]", retcode);
		return(retcode);
	}
	//--- create plugin config object
	if ( ( m_config = m_api->PluginCreate() ) == NULL ) {
		m_api->LoggerOut(MTLogOK, L"PluginConfig failed [%d]", retcode);
		return(MT_RET_ERR_PARAMS);
	}
	//---
	retcode=m_api->TradeSubscribe(this);
	if (retcode != MT_RET_OK) {
		m_api->LoggerOut(MTLogOK, L"TradeSubscribe failed [%d]", retcode);
		return(MT_RET_ERR_PARAMS);
	}
	//--- читаем параметры плагина
	if ( ( retcode = ParametersRead()) != MT_RET_OK ) {
		return( retcode );
	}
	//--- создаем и инициализируем лог
	//CStrW fname = GetModuleName( g_hmodule );
	//LogCreate( oklog, g_hmodule, L"YMD", L"Ok.log", fname );
	log.Open();
	//---
	return(MT_RET_OK);
}
//+------------------------------------------------------------------+
//| Plugin stop notification function                                |
//+------------------------------------------------------------------+
MTAPIRES CPluginInstance::Stop(void) {
	CLogAppenderFunc _lf( galog, __FUNCTION__, true, false );
	//---
	
	//---
	if (m_config != NULL) {
		m_config->Release();
		m_config=NULL;
	}
	//--- unsubscribe from all events
	if(m_api) {
		m_api->PluginUnsubscribe(this);
		m_api->TradeUnsubscribe(this);
		//--- clear API
		m_api=NULL;
		//m_group.Clear();
	}
	//--- ok
	return(MT_RET_OK);
}
//+------------------------------------------------------------------+
//| Plugin config update notification                                |
//+------------------------------------------------------------------+
void CPluginInstance::OnPluginUpdate(const IMTConPlugin* plugin) {
	CLogAppenderFunc _lf( galog, __FUNCTION__, true, false );
	if (!m_api) { // || ! || !m_config) {
		return;
	}
	m_api->LoggerOut( MTLogOK, L"OnPluginUpdate 0" );
	if (!plugin) {// || ! || !m_config) {
		return;
	}
	if (!m_config) {// || !m_api || !m_config) {
		return;
	}
	//---
	if (CMTStr::Compare(plugin->Name(), m_config->Name()) || (plugin->Server() != m_config->Server())) {
		return;
	}
	//--- перечитываем параметры плагина
	ParametersRead();
	
}
//+------------------------------------------------------------------+
//| Plugin Parameters Read                                           |
//+------------------------------------------------------------------+
MTAPIRES CPluginInstance::ParametersRead( void ) {
	if( !m_api )return(MT_RET_ERR_PARAMS);
	//---
	IMTConParam*  param = NULL;
	MTAPIRES res;
	//---
	if( (res = m_api->PluginCurrent( m_config )) != MT_RET_OK ) {
		m_api->LoggerOut( MTLogErr, L"failed to get current plugin configuration [%s (%u)]", SMTFormat::FormatError( res ), res );
		return(res);
	}
	//---
	wchar_t name[ 63 ] = { 0 };
	wchar_t value[ 255 ] = { 0 };
	//---
	if( (param = m_api->PluginParamCreate()) != NULL ) {
		//--- read plugin parameters

		//--- release parameter
		param->Release();
	} else {
		return(MT_RET_ERR_PARAMS);
	}

	//---
	return(MT_RET_OK);
}
//+------------------------------------------------------------------+

