
// cammerParamDlg.h : ͷ�ļ�
//

#pragma once
#include <windows.h>
#include "afxwin.h"
#include "afxcmn.h"
#include<string>
#include<fstream>
#include <iostream>
enum{
	//RPI_FPS,
	RPI_BR,
	RPI_GAIN,
	RPI_SH,
	RPI_CO,
	RPI_SA,
	RPI_EC,
	RPI_SHUTTER,
	RPI_EM,
	RPI_MM,
	RPI_PARM_SIZE,

	RPI_ADJUST_PARAM,
	RPI_ENHANCE_IMAGE
};

struct _use_flag
{
	bool use_br;
	bool use_gain;
	bool use_sh;
	bool use_co;
	bool use_sa;
	bool use_ec;
	bool use_shutter;
	bool use_em;
	bool use_mm;

	bool use_enhance_image;
};
#define WM_DISPLAY_CHANGE (WM_USER + 100)
// CcammerParamDlg �Ի���
class CcammerParamDlg : public CDialogEx
{
// ����
public:
	CcammerParamDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CAMMERPARAM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg  LRESULT  OnDisplayChange(WPARAM wParam, LPARAM lParam);

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void CcammerParamDlg::initPhase();

public:
	//HANDLE hComm; //���
	//OVERLAPPED m_ov; //�����첽�����������Ϣ�Ľṹ��
	//bool CcammerParamDlg::openport(char *portname);
	//bool CcammerParamDlg::setupdcb(int rate_arg);
	//bool CcammerParamDlg::setuptimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant);
	//void CcammerParamDlg::WriteChar(const char* m_szWriteBuffer, DWORD m_nToSend);              //�ο�ReceiveChar()����

public:
	HANDLE m_Handle_comm;
	bool CcammerParamDlg::loadConfig();

	//CScrollBar m_scroll_fps;
	//CScrollBar m_scroll_brightness;
	//CScrollBar m_scroll_sharpness;
	//CScrollBar m_scroll_contrast;
	//CScrollBar m_scroll_saturation;
	//CScrollBar m_scroll_exposure_compensation;
	// ֡�� 1-30 fps
	int m_edit_fps; 
	// ���� 0, 100 br
	int m_edit_brightness;
	// ����  0, 1000
	int m_edit_GAIN;
	// ��� 0, 200
	int m_edit_sharpness;
	// �Աȶ� 0, 200
	int m_edit_contrast;
	// ���Ͷ� 0, 200
	int m_edit_saturation;
	// �عⲹ�� 0, 255
	int m_edit_exposure_compensation;
	CSliderCtrl m_slider_fps;
	CSliderCtrl m_slider_brightness;
	CSliderCtrl m_slider_gain;
	CSliderCtrl m_slider_sharpness;
	CSliderCtrl m_slider_contrast;
	CSliderCtrl m_slider_saturation;
	CSliderCtrl m_slider_exposure_compensation;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditFps();
	afx_msg void OnEnChangeEditBrightness();
	afx_msg void OnEnChangeEditGain();
	afx_msg void OnEnChangeEditSharpness();
	afx_msg void OnEnChangeEditContrast();
	afx_msg void OnEnChangeEditSaturation();
	afx_msg void OnEnChangeEditExposureCompensation();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	int m_radio_exposure_mode;
	int m_radio_metering;

	int m_button_choice;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio11();
	CButton m_button_adjust_param;
	CEdit m_comm_state;
	afx_msg void OnBnClickedButton1();
	// ���� 0�� 330000us
	int m_edit_shutter;
	CSliderCtrl m_slider_shutter;
	afx_msg void OnEnChangeEditShutter();
	void CcammerParamDlg::saveCammerParam();

	struct _use_flag m_use_param_flag;

	// ƽ���Ҷ�ֵ(RPI-->PC)
	int m_edit_aver_gray;


	BOOL m_check_enhance_image;
	afx_msg void OnBnClickedCheckEnhance();
};
