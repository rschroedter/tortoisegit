
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TortoiseGitBlame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("Failed to create Properties Combo \n");
		return -1;      // fail to create
	}

	m_wndObjectCombo.AddString(_T("Application"));
	m_wndObjectCombo.AddString(_T("Properties Window"));
	m_wndObjectCombo.SetFont(CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT)));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Basic Info"));

	
	m_CommitHash = new CMFCPropertyGridProperty(
				_T("Commit Hash"), 
				_T(""),
				_T("Commit Hash")
				);
	pGroup1->AddSubItem(m_CommitHash);

	m_AuthorName = new CMFCPropertyGridProperty(
				_T("Author"), 
				_T(""),
				_T("Author")
				);
	pGroup1->AddSubItem(m_AuthorName);

	m_AuthorDate = new CMFCPropertyGridProperty(
				_T("Author Date"), 
				_T(""),
				_T("Author Date")
				);
	pGroup1->AddSubItem(m_AuthorDate);

	m_AuthorEmail= new CMFCPropertyGridProperty(
				_T("Author Email"), 
				_T(""),
				_T("Author Email")
				);
	pGroup1->AddSubItem(m_AuthorEmail);

	m_CommitterName = new CMFCPropertyGridProperty(
				_T("Committer Name"), 
				_T(""),
				_T("Committer Name")
				);
	pGroup1->AddSubItem(m_CommitterName);

	m_CommitterEmail =new CMFCPropertyGridProperty(
				_T("Committer Email"), 
				_T(""),
				_T("Committer Email")
				);
	pGroup1->AddSubItem(m_CommitterEmail);

	m_CommitterDate = new CMFCPropertyGridProperty(
				_T("Committer Date"), 
				_T(""),
				_T("Committer Date")
				);;
	pGroup1->AddSubItem(m_CommitterDate);

	m_Subject = new CMFCPropertyGridProperty(
				_T("Subject"), 
				_T(""),
				_T("Subject")
				);;;
	pGroup1->AddSubItem(m_Subject);

	m_Body = new CMFCPropertyGridProperty(
				_T("Body"), 
				_T(""),
				_T("Body")
				);;;;
	pGroup1->AddSubItem(m_Body);

	for(int i=0;i<pGroup1->GetSubItemsCount();i++)
	{
		pGroup1->GetSubItem(i)->AllowEdit(FALSE);
	}

	//std::vector<CMFCPropertyGridProperty*> m_ParentHash;
	//std::vector<CMFCPropertyGridProperty*> m_ParentSubject;

	m_wndPropList.AddProperty(pGroup1);
	m_BaseInfoGroup=pGroup1;

	m_ParentGroup=new CMFCPropertyGridProperty(_T("Parent"));

	m_wndPropList.AddProperty(m_ParentGroup);
#if 0
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("Caption"), (_variant_t) _T("About"), _T("Specifies the text that will be displayed in the window's title bar")));

	

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"), (_variant_t) 250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"), (_variant_t) 150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl();
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"), (_variant_t) true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), NULL, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static TCHAR BASED_CODE szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("Hierarchy"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("First sub-level"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("Second sub-level"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 1"), (_variant_t) _T("Value 1"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 2"), (_variant_t) _T("Value 2"), _T("This is a description")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("Item 3"), (_variant_t) _T("Value 3"), _T("This is a description")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
#endif
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}
void CPropertiesWnd::RemoveParent()
{
	for(int i=0;i<m_ParentGroup->GetSubItemsCount();i++)
	{
		CMFCPropertyGridProperty * p=m_ParentGroup->GetSubItem(0);
		m_ParentGroup->RemoveSubItem(p);
	}
	
}
void CPropertiesWnd::UpdateProperties(GitRev *rev)
{
	if(rev)
	{
		m_CommitHash->SetValue(rev->m_CommitHash);
		m_AuthorName->SetValue(rev->m_AuthorName);
		m_AuthorDate->SetValue(rev->m_AuthorDate.Format(_T("%Y-%m-%d %H:%M")));
		m_AuthorEmail->SetValue(rev->m_AuthorEmail);	

		m_CommitterName->SetValue(rev->m_CommitterName);
		m_CommitterEmail->SetValue(rev->m_CommitterEmail);
		m_CommitterDate->SetValue(rev->m_CommitterDate.Format(_T("%Y-%m-%d %H:%M")));

		m_Subject->SetValue(rev->m_Subject);
		m_Body->SetValue(rev->m_Body);

		RemoveParent();

		m_ParentGroup;
	
		CLogDataVector		*pLogEntry = &((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndOutput.m_LogList.m_logEntries;

		for(unsigned int i=0;i<rev->m_ParentHash.size();i++)
		{
			CString str;
			CString parentsubject;
			int index;

			if( pLogEntry->m_HashMap.find(rev->m_ParentHash[i]) == pLogEntry->m_HashMap.end() )
			{ 
				index = -1;
			}
			else
			{
				index=pLogEntry->m_HashMap[rev->m_ParentHash[i]];
			}
			if(index>=0)
				parentsubject=pLogEntry->at(index).m_Subject;

			str.Format(_T("%d - %s \n %s"),i,rev->m_ParentHash[i],parentsubject);
			
			CMFCPropertyGridProperty*p=new CMFCPropertyGridProperty(
											rev->m_ParentHash[i].Left(8), 
												parentsubject,
												str
											);
			m_ParentGroup->AddSubItem(p);
			m_ParentGroup->Expand();
		}
		for(int i=0;i<m_BaseInfoGroup->GetSubItemsCount();i++)
			m_BaseInfoGroup->GetSubItem(i)->SetDescription(m_BaseInfoGroup->GetSubItem(i)->GetValue());

	}else
	{
		m_CommitHash->SetValue(_T(""));
		m_AuthorName->SetValue(_T(""));
		m_AuthorDate->SetValue(_T(""));
		m_AuthorEmail->SetValue(_T(""));

		m_CommitterName->SetValue(_T(""));
		m_CommitterEmail->SetValue(_T(""));
		m_CommitterDate->SetValue(_T(""));

		m_Subject->SetValue(_T(""));
		m_Body->SetValue(_T(""));

		RemoveParent();
		//m_ParentGroup;

		//m_ParentHash->SetValue(_T(""));
		//m_ParentSubject->SetValue(_T(""));		
		for(int i=0;i<m_BaseInfoGroup->GetSubItemsCount();i++)
			m_BaseInfoGroup->GetSubItem(i)->SetDescription(_T(""));
	}
	this->Invalidate();
	m_wndPropList.Invalidate();
}