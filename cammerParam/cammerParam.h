
// cammerParam.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcammerParamApp: 
// �йش����ʵ�֣������ cammerParam.cpp
//

class CcammerParamApp : public CWinApp
{
public:
	CcammerParamApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcammerParamApp theApp;