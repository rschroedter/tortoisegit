#pragma once

#include "HintListCtrl.h"
#include "XPTheme.h"
#include "resource.h"
#include "Git.h"
#include "ProjectProperties.h"
#include "StandAloneDlg.h"
#include "TGitPath.h"
#include "registry.h"
#include "SplitterControl.h"
#include "Colors.h"
#include "MenuButton.h"
#include "LogDlgHelper.h"
#include "FilterEdit.h"
#include "GitRev.h"
#include "Tooltip.h"
#include "HintListCtrl.h"
#include "GitLogList.h"
#include "lanes.h"

#include <regex>
// CGitLogList
#if (NTDDI_VERSION < NTDDI_LONGHORN)

enum LISTITEMSTATES_MINE {
	LISS_NORMAL = 1,
	LISS_HOT = 2,
	LISS_SELECTED = 3,
	LISS_DISABLED = 4,
	LISS_SELECTEDNOTFOCUS = 5,
	LISS_HOTSELECTED = 6,
};

#define MCS_NOTRAILINGDATES  0x0040
#define MCS_SHORTDAYSOFWEEK  0x0080
#define MCS_NOSELCHANGEONNAV 0x0100

#define DTM_SETMCSTYLE    (DTM_FIRST + 11)

#endif

#define ICONITEMBORDER 5

#define GITLOG_START 0
#define GITLOG_END   100

typedef void CALLBACK_PROCESS(void * data, int progress);

class CGitLogList : public CHintListCtrl
{
	DECLARE_DYNAMIC(CGitLogList)

public:
	CGitLogList();
	virtual ~CGitLogList();
	volatile LONG		m_bNoDispUpdates;
	BOOL m_bStrictStopped;
	BOOL m_bShowBugtraqColumn;
	BOOL m_bSearchIndex;
	BOOL m_bCancelled;
	bool				m_hasWC;
	GitRev				m_wcRev;
	volatile LONG 		m_bThreadRunning;

	enum
	{
		LOGLIST_GRAPH,
		LOGLIST_ACTION,
		LOGLIST_MESSAGE,
		LOGLIST_AUTHOR,
		LOGLIST_DATE,
		LOGLIST_BUG,
		LOGLIST_MESSAGE_MAX=250
	};

	enum 
	{
	// needs to start with 1, since 0 is the return value if *nothing* is clicked on in the context menu
	ID_COMPARE = 1,
	ID_SAVEAS,
	ID_COMPARETWO,
	ID_UPDATE,
	ID_COPY,
	ID_REVERTREV,
	ID_MERGEREV,
	ID_GNUDIFF1,
	ID_GNUDIFF2,
	ID_FINDENTRY,
	ID_OPEN,
	ID_BLAME,
	ID_REPOBROWSE,
	ID_LOG,
	ID_POPPROPS,
	ID_EDITAUTHOR,
	ID_EDITLOG,
	ID_DIFF,
	ID_OPENWITH,
	ID_COPYCLIPBOARD,
	ID_COPYHASH,
	ID_CHECKOUT,
	ID_REVERTTOREV,
	ID_BLAMECOMPARE,
	ID_BLAMETWO,
	ID_BLAMEDIFF,
	ID_VIEWREV,
	ID_VIEWPATHREV,
	ID_EXPORT,
	ID_COMPAREWITHPREVIOUS,
	ID_BLAMEWITHPREVIOUS,
	ID_GETMERGELOGS,
	ID_REVPROPS,
	ID_CHERRY_PICK,
	ID_CREATE_BRANCH,
	ID_CREATE_TAG,
	ID_SWITCHTOREV
	};
	void InsertGitColumn();
	void ResizeAllListCtrlCols();
	void CopySelectionToClipBoard(bool hashonly=FALSE);
	void DiffSelectedRevWithPrevious();
	bool IsSelectionContinuous();
	int  FillGitLog();
	inline int ShownCountWithStopped() const { return (int)m_arShownList.GetCount() + (m_bStrictStopped ? 1 : 0); }
	int FetchLogAsync(CALLBACK_PROCESS *proc=NULL, void * data=NULL);
	CPtrArray			m_arShownList;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdrawLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnGetdispinfoLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	void OnNMDblclkLoglist(NMHDR * /*pNMHDR*/, LRESULT *pResult);
	afx_msg void OnLvnOdfinditemLoglist(NMHDR *pNMHDR, LRESULT *pResult);
	void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static UINT LogThreadEntry(LPVOID pVoid);
	UINT LogThread();
	void FillBackGround(HDC hdc, int Index,CRect &rect);
	void DrawTagBranch(HDC,CRect &rect,INT_PTR index);
	void DrawGraph(HDC,CRect &rect,INT_PTR index);

	BOOL GetShortName(CString ref, CString &shortname,CString prefix);
	void paintGraphLane(HDC hdc,int laneHeight, int type, int x1, int x2,
                                      const COLORREF& col,int top) ; 
	void DrawLine(HDC hdc, int x1, int y1, int x2, int y2){::MoveToEx(hdc,x1,y1,NULL);::LineTo(hdc,x2,y2);}
	
	CXPTheme			m_Theme;
	BOOL				m_bVista;
	
	HICON				m_hModifiedIcon;
	HICON				m_hReplacedIcon;
	HICON				m_hAddedIcon;
	HICON				m_hDeletedIcon;

	HFONT				m_boldFont;

	CRegDWORD			m_regMaxBugIDColWidth;
	int					m_nSearchIndex;
	CLogDataVector		m_logEntries;
	CALLBACK_PROCESS    *m_ProcCallBack;
	void				*m_ProcData;
	CStoreSelection*	m_pStoreSelection;
	MAP_HASH_NAME		m_HashMap;
};

