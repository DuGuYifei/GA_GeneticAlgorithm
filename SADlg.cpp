// SADlg.cpp : implementation file
//

#include "stdafx.h"
#include "SA.h"
#include "SADlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSADlg dialog

CSADlg::CSADlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSADlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSADlg)
	DDX_Control(pDX, IDC_CITY_FRAME, m_CityFrame);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSADlg, CDialog)
	//{{AFX_MSG_MAP(CSADlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, OnStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSADlg message handlers

BOOL CSADlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	
	srand((unsigned)time(NULL));

	CRect r;
	m_CityFrame.GetClientRect(&r);

	m_memDC.CreateCompatibleDC(GetDC());
	m_bmp.CreateCompatibleBitmap(GetDC(), r.Width(), r.Height()); 
	m_font.CreatePointFont(110, L"Arial");
	m_pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	
	m_memDC.SelectObject(&m_bmp);
	m_memDC.SelectObject(&m_font);
	m_memDC.SelectObject(&m_pen);
	m_memDC.SelectObject(&m_brush);

	m_CityFrame.GetClientRect(&r);
	m_memDC.Rectangle(&r);

	m_memDC.SetBkMode(TRANSPARENT); 
	
	COLORREF oldTxtCol = (m_memDC.SetTextColor(RGB (0, 100, 10)));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSADlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting

		CRect r;
		m_CityFrame.GetWindowRect(&r);
		ScreenToClient(r);		

		dc.BitBlt(r.left, r.top, r.right - r.left, r.bottom - r.top, &m_memDC, 0, 0, SRCCOPY);
	
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSADlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



/**************************************************************************}
{ODCZYT MAPY Z PLIKU													   }
{**************************************************************************/
BOOL CSADlg::InitializeMap()
{

	int i, j, k;
	double lenergy;
	int stop;


	CFile f;
	if (!f.Open(L"tsp-c.dat", CFile::modeRead | CFile::shareDenyWrite))
	{
		MessageBox(L"Proszê umieœciæ plik tsp-c.dat w katalogu, w którym znajduje siê program.");
		return FALSE;
	}
	
	float fBuf;

	for (i = 0; i < NCITIES; i++)
	{
		f.Read(&fBuf, sizeof(float));
		loc[i][0] = (double)fBuf;
		f.Read(&fBuf, sizeof(float));
		loc[i][1] = (double)fBuf;
	};

	f.Close();

	for (i = 0; i < NCITIES; i++)
		for (j = 0; j < NCITIES; j++)
			distance[i][j]=sqrt((loc[i][0]-loc[j][0])*(loc[i][0]-loc[j][0])+(loc[i][1]-loc[j][1])*(loc[i][1]-loc[j][1]));
	
	for (j = 0; j < NCITIES; j++)
	{
		do
		{
			stop = 1;
			route[j] = rand() % NCITIES;
			k=0;
		 
			while (k<j)
			{
				if (route[k] == route[j])
				{
					stop = 0;
					break;
				}
				k++;
			};
		}
		while (!stop);
	 };

	lenergy=0;
	for (j = 0;j < NCITIES-1; j++)
		lenergy = lenergy + distance[route[j]][route[j+1]];
	lenergy=lenergy+distance[route[NCITIES-1]][route[0]];

	
	 length = lenergy;

	 return TRUE;
}

/**************************************************************************}
{RYSOWANIE AKTUALNEJ TRASY, WYŒWIETLENIE JEJ D£UGOŒCI 					   }
{**************************************************************************/
void CSADlg::Draw()
{

	int i, x, y, x1, y1;
	char s[10];
	CString sEn = "Route length: ";

	CRect r;
	m_CityFrame.GetClientRect(&r);
	m_memDC.Rectangle(&r);

	_gcvt_s(s, 10, length, 4);
	sEn += CString(s);

	m_memDC.TextOut(10, r.Height()-20, sEn);

	for (i = 0; i < NCITIES; i++)
	{
		x = 10 + (int)((r.Width()-20)*loc[i][0]);
		y = 10 + (int)((r.Height()-20)*loc[i][1]);
		m_memDC.Ellipse(x-5, y-5, x+5, y+5);
	}

	for (i = 0; i < NCITIES-1; i++)
	{
		x = 10 + (int)((r.Width()-20)*loc[route[i]][0]);
		y = 10 + (int)((r.Height()-20)*loc[route[i]][1]);
		x1 = 10 + (int)((r.Width()-20)*loc[route[i+1]][0]);
		y1 = 10 + (int)((r.Height()-20)*loc[route[i+1]][1]);
		m_memDC.MoveTo(x, y);
		m_memDC.LineTo(x1, y1);
	};

	x = 10 + (int)((r.Width()-20)*loc[route[NCITIES-1]][0]);
	y = 10 + (int)((r.Height()-20)*loc[route[NCITIES-1]][1]);
	x1 = 10 + (int)((r.Width()-20)*loc[route[0]][0]);
	y1 = 10 + (int)((r.Height()-20)*loc[route[0]][1]);
	m_memDC.MoveTo(x, y);
	m_memDC.LineTo(x1, y1);

	CDC* pDC = GetDC();
	m_CityFrame.GetWindowRect(&r);
	ScreenToClient(r);

	pDC->BitBlt(r.left, r.top, r.right - r.left, r.bottom - r.top, &m_memDC, 0, 0, SRCCOPY);

	ReleaseDC(pDC);
}

void CSADlg::InitPopulation(void)
{
	int i, j, k, stop;

	for (i = 0; i < NCREAT; i++)
	{

		for (j = 0; j < NCITIES; j++)
		{
			do
			{
				stop = 1;
				population[i][j] = rand() % NCITIES;
				k=0;
			 
				while (k<j)
				{
					if (population[i][k] == population[i][j])
					{
						stop = 0;
						break;
					}
					k++;
				};
			}
			while (!stop);
		 };
	}
}

BOOL CSADlg::Check(int city, int n, int* route)
{
	BOOL bRes = FALSE;
	int i;

	for (i = 0; i < n; i++)
	{
		if (route[i] == city)
		{
			bRes = TRUE;
			break;
		}
	}

	return bRes;
}

double CSADlg::TotalDistance(int c)
{
	double dis = 0;
	for (int i = 0; i < NCITIES - 1; i++)
	{
		dis += distance[population[c][i]][population[c][i + 1]];
	}
	dis += distance[population[c][NCITIES - 1]][population[c][0]];
	return dis;
}

int CSADlg::SelectBest()
{
	int best = 0;
	double max = dis[0];
	for (int i = 0; i < NCREAT; i++)
	{
		if (dis[i] > max)
		{
			max = dis[i];
			best = i;
		}
	}
	return best;
}

void CSADlg::Fitness()
{
	for (int i = 0; i < NCREAT; i++)
	{
		dis[i] = 1/TotalDistance(i);
	}
}

std::pair<int,int> CSADlg::RouletteWheelSelection()
{
	double sum = 0;
	for (auto& i : dis)
	{
		sum += i;
	}
	std::pair<int, int> parents;
	parents.first = SelectParent(sum);
	parents.second = SelectParent(sum);
	return parents;
}

int CSADlg::SelectParent(double sum)
{
	double r0 = rand() / (double)RAND_MAX;
	double r = r0 * sum;
	double temp = 0;
	int i = 0;
	for (; i < NCREAT; i++)
	{
		temp += dis[i];
		if (temp >= r)
			break;
	}
	return i;
}

std::vector<int> CSADlg::Permutation(std::pair<int, int> parents)
{
	std::vector<int> child;
	double r = rand() / (double)RAND_MAX;
	if (r<=0.5)
	{
		for (int i = 0; i < NCITIES / 2; i++)
		{
			child.push_back(population[parents.first][i]);
		}
		for (int i = 0; i < NCITIES; i++)
		{
			if (count(child.begin(), child.end(), population[parents.second][i]) == 0)
			{
				child.push_back(population[parents.second][i]);
			}
		}
	}
	else if(r <= 0.7)
	{
		int r = rand() % 2;
		int r1 = rand() % NCITIES;
		int r2 = rand() % NCITIES;
		while (r2 == r1)
		{
			r2 = rand() % NCITIES;
		}
		int parent = parents.second;
		if (r)
		{
			parent = parents.first;
		}
		for (int i = 0; i < NCITIES; i++)
		{
			if (i == r1)
			{
				child.push_back(population[parent][r2]);
				continue;
			}
			if (i == r2)
			{
				child.push_back(population[parent][r1]);
				continue;
			}
			child.push_back(population[parent][i]);
		}
		//for (int i = 0; i < r1; i++)
		//{
		//	child.push_back(population[parent][i]);
		//}
		//for (int i = r2; i >= r1; i--)
		//{
		//	child.push_back(population[parent][i]);
		//}
		//for (int i = r2 + 1; i < NCITIES; i++)
		//{
		//	child.push_back(population[parent][i]);
		//}
	}
	else
	{
		for (int i = 0; i < NCITIES; i++)
		{
			child.push_back(population[parents.first][i]);
		}
	}
	return child;
}

void CSADlg::OnStart() 
{

	if (!InitializeMap())
		return;
 
	Draw();

	InitPopulation();

	

  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  INSERT THE GENETIC ALGORITHM CODE HERE
  
  The distances between cities are stored in 'distance' array (in SADlg.h file).

  Draw() displays the current route (stored in 'route' array) and its length 
  ('length' variable).
  																			  
  The initial population has already been created and stored in 'population' array.

  The number of creatures is defined by NCREAT constant.

  Check(int city, int n, int* route) allows us to find out if the given 'city' 
  belongs to the first 'n' cities that the 'route' contains.

  --------------------------------------------------------------------------
  Create 10000 populations and for each of them display the best individual
  using Draw().
  
  Set the crossover probability to 0.5 and the mutation probability to 0.2.

  Creating the new population:
  1. Assign the fitness score to each individual (the higher the better).
  -- Repeat following steps until the number of new individuals equals the number of individuals in the old population --
  2. Choose a pair of individuals using "Roulette Wheel Selection" (http://www.obitko.com/tutorials/genetic-algorithms/selection.php)
  3. Perform the crossover with the given probability to crate the new pair of individuals using "Permutation Encoding" (otherwise 
     copy the individuals directly to the new population) - http://www.obitko.com/tutorials/genetic-algorithms/crossover-mutation.php.
  4. Perform the mutation with the given probability.

  When the new population is ready, replace one of the new individuals with the best individual from the previous population ('elitism')
  and then treat the new population as the current one.
    
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	int best = 0;
	for (int j = 0; j < 10000; j++)
	{
		Fitness();
		//Get best individual
		best = SelectBest();
		if(length != TotalDistance(best))
			length = TotalDistance(best);
		std::vector<int>bestV;
		for (int tempIndex = 0; tempIndex < NCITIES; tempIndex++)
		{
			bestV.push_back(population[best][tempIndex]);
		}
		std::vector<std::vector<int>> new_population;
		for (int create = 0; create < NCREAT/2; create++)
		{
			std::pair<int, int> parents = RouletteWheelSelection();
			std::pair<int, int> parents2 = { parents.second,parents.first };
			std::vector<int> child1 = Permutation(parents);
			std::vector<int> child2 = Permutation(parents2);

			new_population.push_back(child1);
			new_population.push_back(child2);
		}
		

		for(int create = 0;create<NCREAT;create++)
			for (int cities = 0; cities < NCITIES; cities++)
			{
				population[create][cities] = new_population[create][cities];
			}

		int randChange = rand() % NCREAT;
		for (int tempIndex = 0; tempIndex < NCITIES; tempIndex++)
			population[randChange][tempIndex] = bestV[tempIndex];
	}
	Fitness();
	best = SelectBest();
	for (int i = 0; i < NCITIES; i++)
	{
		route[i] = population[best][i];
	}
	length = TotalDistance(best);
	Draw();
}


