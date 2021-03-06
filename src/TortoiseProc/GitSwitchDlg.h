#pragma once

#include "StandAloneDlg.h"
#include "HistoryCombo.h"

#include "ChooseVersion.h"
// CGitSwitchDlg dialog

class CGitSwitchDlg : public CResizableStandAloneDialog,public CChooseVersion
{
	DECLARE_DYNAMIC(CGitSwitchDlg)

public:
	CGitSwitchDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGitSwitchDlg();

// Dialog Data
	enum { IDD = IDD_GITSWITCH };

	BOOL m_bForce;
	BOOL m_bTrack;
	BOOL m_bBranch;
	CString m_NewBranch;
	CString m_Base;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void OnBnClickedOk();

	afx_msg void OnBnClickedChooseRadioHost();
	afx_msg void OnBnClickedShow();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckBranch();
	afx_msg void OnCbnSelchangeComboboxexBranch();
};
