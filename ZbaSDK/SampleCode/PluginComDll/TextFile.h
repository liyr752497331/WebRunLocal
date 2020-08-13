// TextFile.h : CTextFile ������

#pragma once
#include "resource.h"       // ������
#include <atlcomtime.h>
#ifdef _USRDLL
#include "PluginComDll_i.h"
#else
#include "PluginComExe_i.h"
#endif
#include "_ITextFileEvents_CP.h"
#include "TextFileHelper.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

// CTextFile

class ATL_NO_VTABLE CTextFile :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CTextFile, &CLSID_TextFile>,
	public IConnectionPointContainerImpl<CTextFile>,
	public CProxy_ITextFileEvents<CTextFile>,
#ifdef _USRDLL
	public IDispatchImpl<ITextFile, &IID_ITextFile, &LIBID_PluginComDllLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
#else
	public IDispatchImpl<ITextFile, &IID_ITextFile, &LIBID_PluginComExeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
#endif
	public IDispatchImpl<IWrlConn, &__uuidof(IWrlConn), &LIBID_ZbaBase, /* wMajor = */ 1>
{
protected:

	/// WEB SOCKET���Ӷ���
	IWebSocketConnectPtr m_spiSocketConnect;

public:
	CTextFile()
		:m_spiSocketConnect(NULL)
	{
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_TEXTFILE)

	BEGIN_COM_MAP(CTextFile)
		COM_INTERFACE_ENTRY(ITextFile)
		COM_INTERFACE_ENTRY2(IDispatch, IWrlConn)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IWrlConn)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CTextFile)
		CONNECTION_POINT_ENTRY(__uuidof(_ITextFileEvents))
	END_CONNECTION_POINT_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		if (NULL != m_spiSocketConnect)
			m_spiSocketConnect = NULL;
	}

public:

	// IWrlConn Methods
	STDMETHOD(Load)(LPDISPATCH piDispatch,BSTR bstrAuthInfo,BSTR bstrLang)
	{
		if(NULL == piDispatch)
			return E_POINTER;
		HRESULT hRet = piDispatch->QueryInterface(IID_IWebSocketConnect,(LPVOID *)&m_spiSocketConnect);
		if(NULL != m_spiSocketConnect)
			hRet = m_spiSocketConnect->put_ActiveTime(COleDateTime::GetCurrentTime().m_dt);
		return hRet;
	}

	STDMETHOD(Unload)(EWrlCloseConnType eCloseConnType, BSTR bstrReason)
	{
		if(NULL != m_spiSocketConnect)
			m_spiSocketConnect = NULL;
		return S_OK;
	}

	STDMETHOD(UsbChanged)(BSTR bstrDisk, ULONG nStatus, BSTR bstrName)
	{
		/// �յ�USB�仯֪ͨ
		return S_OK;
	}

	STDMETHOD(SendJson)(BSTR bstrContent)
	{
		HRESULT hRet(S_FALSE);
		if (NULL != m_spiSocketConnect)
		{
			ULONG nRet = 0;
			hRet = m_spiSocketConnect->AsynSendText(bstrContent,&nRet);
		}
		return hRet;
	}

	STDMETHOD(RecText)(BSTR bstrContent);

	STDMETHOD(RecJson)(ULONG nReqID,BSTR bstrReqName,BSTR bstrContent);

	STDMETHOD(RecByte)(BYTE* pContent,ULONG nLen)
	{
		return S_FALSE;
	}

	STDMETHOD(HttpRequst)(BSTR bstrUrl,BSTR bstrPara, BSTR* pVal)
	{
		/// HTTP����������Ӧ
		return E_NOTIMPL;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(TextFile), CTextFile)