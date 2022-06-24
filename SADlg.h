// SADlg.h : header file
//
#include<utility> 
#include<vector>
#include<algorithm>
#if !defined(AFX_SADLG_H__438856B2_C48B_477A_8281_7A2F42FF546D__INCLUDED_)
#define AFX_SADLG_H__438856B2_C48B_477A_8281_7A2F42FF546D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NCITIES 50             /*number of cities - do not change*/
#define NCREAT	20			   /*number of creatures (individuals)*/

/////////////////////////////////////////////////////////////////////////////
// CSADlg dialog

class CSADlg : public CDialog
{
// Construction
public:
	CSADlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSADlg)
	enum { IDD = IDD_SA_DIALOG };
	CStatic	m_CityFrame;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Draw();
	BOOL InitializeMap();
	void InitPopulation(void);
	BOOL Check(int city, int n, int* route);
	void Fitness();
	double TotalDistance(int c);
	std::pair<int,int> RouletteWheelSelection();
	int SelectParent(double sum);
	std::vector<int> Permutation(std::pair<int, int> parents);
	int SelectBest();
	HICON m_hIcon;

	double	loc[NCITIES][2];			/*coordinates of the cities*/
	double	distance[NCITIES][NCITIES];	/*distan ces between cities*/
	int		route[NCITIES];				/*current route*/
	double	length;						/*length of the current route*/
	int		population[NCREAT][NCITIES];
	//std::vector<std::vector<int>> new_population;
	double	dis[NCREAT];
	//std::pair<double, int> dis[NCREAT];
	std::vector<std::vector<int>> children;

		
	CDC m_memDC;
	CBitmap m_bmp;
	CFont m_font;
	CPen m_pen;
	CBrush m_brush;

	// Generated message map functions
	//{{AFX_MSG(CSADlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SADLG_H__438856B2_C48B_477A_8281_7A2F42FF546D__INCLUDED_)
