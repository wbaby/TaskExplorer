#include "stdafx.h"
#include "WinDriver.h"
#include "WinModule.h"
#include "WindowsAPI.h"

#include "ProcessHacker.h"

CWinDriver::CWinDriver(QObject *parent) 
	: CDriverInfo(parent) 
{
	m_ImageBase = 0;
	m_ImageSize = 0;

	m_pModuleInfo = CModulePtr(new CWinModule());
	connect(m_pModuleInfo.data(), SIGNAL(AsyncDataDone(bool, quint32, quint32)), this, SLOT(OnAsyncDataDone(bool, quint32, quint32)));
}

CWinDriver::~CWinDriver()
{
	
}

bool CWinDriver::InitStaticData(struct _RTL_PROCESS_MODULE_INFORMATION* Module)
{
	QWriteLocker Locker(&m_Mutex);

	PPH_STRING kernelFileName = PhConvertMultiByteToUtf16((PSTR)Module->FullPathName);
	PPH_STRING newFileName = PhGetFileName(kernelFileName);
	PhDereferenceObject(kernelFileName);

	m_BinaryPath = CastPhString(newFileName);
	m_FileName = (char*)Module->FullPathName + Module->OffsetToFileName;
	m_ImageBase = (quint64)Module->ImageBase;
	m_ImageSize = (quint64)Module->ImageSize;

	m_CreateTimeStamp = GetTime() * 1000;

	qobject_cast<CWinModule*>(m_pModuleInfo)->InitStaticData(m_BinaryPath);
	qobject_cast<CWinModule*>(m_pModuleInfo)->InitAsyncData();

	return true;
}

void CWinDriver::OnAsyncDataDone(bool IsPacked, quint32 ImportFunctions, quint32 ImportModules)
{
}

//bool CWinDriver::UpdateDynamicData()
//{
//}