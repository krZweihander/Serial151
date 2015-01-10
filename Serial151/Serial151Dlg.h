
// Serial151Dlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"
#include "..\Serial\SerialMFC.h"
#include "afxwin.h"


// CSerial151Dlg ��ȭ ����
class CSerial151Dlg : public CDialog
{
// �����Դϴ�.
public:
	CSerial151Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SERIAL151_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM /*lParam*/);
	afx_msg void OnBnClickedbtnportopen();
	afx_msg void OnBnClickedbtnportclose();
	afx_msg void OnBnClickedbtntx();
	afx_msg void OnEnKillfocusedttx1();
	afx_msg void OnBnClickedbtnread();

	CString hex(int i);
	int dex(CString hexStr);
	void initial(void);
	void autoPortOpen(void);
	void controlEnable(bool e);
	void process(CString instr);
	void done(CString* hdata, int length);
	CString sendformat(int* _idata);
	int getCS(int* idata);
	void send(CString str);
	void Log(const char* str);
	
	CSerialMFC m_Serial;
	CSerial::EBaudrate m_eBaudrate;
	CString m_sReadstack;
};
