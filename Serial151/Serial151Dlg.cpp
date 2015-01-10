
// Serial151Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Serial151.h"
#include "Serial151Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSerial151Dlg ��ȭ ����




CSerial151Dlg::CSerial151Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerial151Dlg::IDD, pParent)
	, m_sReadstack(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerial151Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSerial151Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SERIAL(OnSerialMsg)
	ON_BN_CLICKED(IDC_btnPortOpen, &CSerial151Dlg::OnBnClickedbtnportopen)
	ON_BN_CLICKED(IDC_btnPortClose, &CSerial151Dlg::OnBnClickedbtnportclose)
	ON_BN_CLICKED(IDC_btnTx, &CSerial151Dlg::OnBnClickedbtntx)
	ON_EN_KILLFOCUS(IDC_edtTx1, &CSerial151Dlg::OnEnKillfocusedttx1)
	ON_BN_CLICKED(IDC_btnRead, &CSerial151Dlg::OnBnClickedbtnread)
END_MESSAGE_MAP()


// CSerial151Dlg �޽��� ó����

BOOL CSerial151Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	initial();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CSerial151Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CSerial151Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CSerial151Dlg::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
		case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
				case VK_ESCAPE:
				case VK_RETURN:
				{
					::TranslateMessage(pMsg);
					::DispatchMessage(pMsg);
					
					return TRUE;
				}     
			}
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

CString CSerial151Dlg::hex(int i)
{
	CString out;
	out.Format(_T("%X"), i);
	return (out);
}

int CSerial151Dlg::dex(CString hexStr)
{
	wchar_t *end = NULL;
	long value = wcstol (hexStr, &end, 16);
	return value;
}
void CSerial151Dlg::initial(void)
{
	GetDlgItem(IDC_cmbPort)->SendMessage(CB_SETCURSEL, 0, 0);
	
	controlEnable(false);

	m_eBaudrate = CSerial::EBaud9600;

	autoPortOpen();
}

void CSerial151Dlg::autoPortOpen(void)
{
	for (int i = 1; i <= 10; i++)
	{
		CString szPort = _T("");
		GetDlgItemText(IDC_cmbPort, szPort);
		if (m_Serial.Open(szPort,this) == ERROR_SUCCESS)
		{
			m_Serial.Setup(m_eBaudrate,
			CSerial::EData8,
			CSerial::EParNone,
			CSerial::EStop1);
			
			m_Serial.SetupHandshaking(CSerial::EHandshakeOff);

			GetDlgItem(IDC_btnPortOpen)->EnableWindow(false);
			GetDlgItem(IDC_btnPortClose)->EnableWindow(true);
			GetDlgItem(IDC_cmbPort)->EnableWindow(false);
			controlEnable(true);

			return;
		}
		if (i == 10) break;
		GetDlgItem(IDC_cmbPort)->SendMessage(CB_SETCURSEL, i, 0);
	}
	AfxMessageBox(_T("�� �� �ִ� ��Ʈ�� �����ϴ�"),MB_ICONSTOP|MB_OK);
	GetDlgItem(IDC_cmbPort)->SendMessage(CB_SETCURSEL, 0, 0);
}

void CSerial151Dlg::controlEnable(bool e)
{
	GetDlgItem(IDC_edtRx1)->EnableWindow(e);
	GetDlgItem(IDC_edtRx2)->EnableWindow(e);
	GetDlgItem(IDC_btnRead)->EnableWindow(e);
	GetDlgItem(IDC_edtTx1)->EnableWindow(e);
	GetDlgItem(IDC_edtTx2)->EnableWindow(e);
	GetDlgItem(IDC_btnTx)->EnableWindow(e);
}

LRESULT CSerial151Dlg::OnSerialMsg (WPARAM wParam, LPARAM /*lParam*/)
{
	CSerial::EEvent eEvent = CSerial::EEvent(LOWORD(wParam));
	CSerial::EError eError = CSerial::EError(HIWORD(wParam));
	
	if (eEvent & CSerial::EEventRecv)
	{
		DWORD dwRead;
		CString szString;
		char szData[101];
		const int nBuflen = sizeof(szData)-1;

		// Obtain the data from the serial port
		do
		{
			m_Serial.Read(szData,nBuflen,&dwRead);
			szData[dwRead] = '\0';

			for (DWORD dwChar=0; dwChar<dwRead; dwChar++)
			{
				CString in;
				in.Format(_T("%c"), szData[dwChar]);
				szString += in;
				if(szData[dwChar] == '\r')
					szString += '\n';
			}

			process(szString);
		} while (dwRead == nBuflen);
	}
	return 0;
}

void CSerial151Dlg::process(CString instr)
{
	instr.TrimLeft();
	instr.TrimRight();
	m_sReadstack += instr;
	
	CString parser = _T("*;");

	if (m_sReadstack.Find(parser) != -1)
	{
		CString nowreading;
		CString afterreading;
		CString* hdata;
		nowreading = m_sReadstack.Left(m_sReadstack.Find(parser));
		m_sReadstack = m_sReadstack.Mid(m_sReadstack.Find(parser) + 2);
		hdata = new CString[nowreading.GetLength()];
		for (int i = 0; i < nowreading.GetLength(); i++)
		{
			hdata[i] = nowreading.Mid((i * 2), 2);
		}
		if (hdata[0] == _T("63"))
			done(hdata, nowreading.GetLength());
		delete[] hdata;
	}
}

void CSerial151Dlg::done(CString* hdata, int length)
{
	int* idata = new int[length];

	for (int i = 0; i < length; i++)
	{
		idata[i] = dex(hdata[i]);
	}
	int from = idata[1];
	int des = idata[2];
	int mode = idata[3];
	int com1 = idata[4];
	int com2 = idata[5];
	int* data = new int[12];
	for (int i = 0; i <= 11; i++)
		data[i] = idata[i + 6];
	int cs = idata[18];
	CString hfcs = _T("00") + hex(getCS(idata));
	hfcs = hfcs.Right(2);
	int ics = dex(hfcs);
	if (cs == ics)
		SetDlgItemText(IDC_lblCS, _T(""));
	else
		SetDlgItemText(IDC_lblCS, _T("CheckSum Failure!"));
	
	switch(mode)
	{
	case 1:
		switch(com1)
		{
		case 1:
			break;
		case 2:
			CString out;
			out.Format(_T("%d"), data[0]);
			SetDlgItemText(IDC_edtRx1, out);
			out.Format(_T("%d"), data[1]);
			SetDlgItemText(IDC_edtRx2, out);
		}
		break;
	case 2:
		break;
	}
	delete[] idata;
}

CString CSerial151Dlg::sendformat(int* _idata)
{
	int* idata = new int[19];
	for (int i = 0; i <= 17; i++)
		idata[i] = _idata[i];
	Log("c");
	idata[18] = getCS(idata);
	Log("g");
	CString* hdata = new CString[20];
	Log("h");
	for (int i = 0; i <= 18; i++)
	{
		hdata[i] = hex(idata[i]);
		hdata[i] = _T("00") + hdata[i];
		hdata[i] = hdata[i].Right(2);
		Log("f");
	}
	Log("i");
	hdata[19] = L"*;";
	Log("j");
	
	CString out = _T("");
	Log("k");
	for (int i = 0; i <= 19; i++)
	{
		out += hdata[i];
		Log("j");
	}
	Log("l");
	
	delete[] idata;
	delete[] hdata;

	return out;
}

int CSerial151Dlg::getCS(int* idata)
{
	Log("d");
	int sum = 0;
	for (int i = 0; i <= 17; i++)
	{
		sum += idata[i];
		Log("e");
	}
	Log("f");
	//CString str;
	//str.Format(_T("%d"), sum);
	//SetDlgItemText(IDC_edtTx1, str);
	return sum;
}

void CSerial151Dlg::send(CString str)
{
	Log("m");
	char* ss = LPSTR(LPCTSTR(str));
	Log("n");
	m_Serial.Write(ss, str.GetLength() * 2);
	Log("o");
}

void CSerial151Dlg::OnBnClickedbtnportopen()
{
	CString szPort = _T("");
	GetDlgItemText(IDC_cmbPort, szPort);
	if (m_Serial.Open(szPort,this) != ERROR_SUCCESS)
	{
		szPort += _T(" ��Ʈ�� �� �� �����ϴ�.");
		AfxMessageBox(szPort,MB_ICONSTOP|MB_OK);
		return;
	}

	m_Serial.Setup(m_eBaudrate,
		CSerial::EData8,
		CSerial::EParNone,
		CSerial::EStop1);

	m_Serial.SetupHandshaking(CSerial::EHandshakeOff);

	GetDlgItem(IDC_btnPortOpen)->EnableWindow(false);
	GetDlgItem(IDC_btnPortClose)->EnableWindow(true);
	GetDlgItem(IDC_cmbPort)->EnableWindow(false);
	controlEnable(true);
}

void CSerial151Dlg::OnBnClickedbtnportclose()
{
	m_Serial.Close();

	GetDlgItem(IDC_btnPortOpen)->EnableWindow(true);
	GetDlgItem(IDC_btnPortClose)->EnableWindow(false);
	GetDlgItem(IDC_cmbPort)->EnableWindow(true);
	controlEnable(false);
}

void CSerial151Dlg::OnBnClickedbtntx()
{
	if (!m_Serial.IsOpen()) return;

	int* idata = new int[18];
	for (int i = 0; i <= 17; i++)
		idata[i] = 0;
	idata[0] = 99;
	idata[1] = 0;
	idata[2] = 1;
	idata[3] = 2;
	idata[4] = 1;
	idata[5] = 0;
	CString str;
	GetDlgItemText(IDC_edtTx1, str);
	idata[6] = _ttoi(str);
	GetDlgItemText(IDC_edtTx2, str);
	idata[7] = _ttoi(str);

	send(sendformat(idata));
	delete[] idata;
}
void CSerial151Dlg::OnEnKillfocusedttx1()
{
	CString str;
	GetDlgItemText(IDC_edtTx1, str);
	int i = _ttoi(str);
	if (i > 255)
		i = 255;
	else if (i < 0)
		i = 0;
	str.Format(_T("%d"), i);
	SetDlgItemText(IDC_edtTx1, str);
}

void CSerial151Dlg::OnBnClickedbtnread()
{
	Log("start\r\n");
	Log("a");
	if (!m_Serial.IsOpen()) return;
	
	int* idata = new int[18];
	for (int i = 0; i <= 17; i++)
		idata[i] = 0;
	idata[0] = 99;
	idata[1] = 0;
	idata[2] = 1;
	idata[3] = 1;
	idata[4] = 1;
	idata[5] = 0;
	
	Log("b");

	send(sendformat(idata));
	Log("\r\nend\r\n");
	delete[] idata;
}

void CSerial151Dlg::Log(const char* str)
{
	CString in;
	in.Format(_T("%s"), str);
	CString s;
	GetDlgItemText(IDC_edtConsole, s);
	s += str;
	SetDlgItemText(IDC_edtConsole, s);
}
