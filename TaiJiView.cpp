
// TaiJiView.cpp: CTaiJiView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TaiJi.h"
#endif

#include "TaiJiDoc.h"
#include "TaiJiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define Round(d) int(floor(d+0.5))
#define PI 3.1415926

static const UINT_PTR kTaiJiTimerId = 1;
static const UINT kTaiJiTimerMs = 16; // ~60fps


// CTaiJiView

IMPLEMENT_DYNCREATE(CTaiJiView, CView)

BEGIN_MESSAGE_MAP(CTaiJiView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTaiJiView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CTaiJiView 构造/析构

CTaiJiView::CTaiJiView() noexcept
	: m_angle(0)
	, m_timerId(0)
{
}

CTaiJiView::~CTaiJiView()
{
}

BOOL CTaiJiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

int CTaiJiView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_timerId = SetTimer(kTaiJiTimerId, kTaiJiTimerMs, NULL);
	return 0;
}

void CTaiJiView::OnDestroy()
{
	if (m_timerId != 0)
	{
		KillTimer(m_timerId);
		m_timerId = 0;
	}
	CView::OnDestroy();
}

void CTaiJiView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == kTaiJiTimerId)
	{
		m_angle = (m_angle + 2) % 360;
		Invalidate(FALSE);
	}
	CView::OnTimer(nIDEvent);
}

// CTaiJiView 绘图

void CTaiJiView::OnDraw(CDC* pDC)
{
	CTaiJiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	// 双缓冲：一次绘制当前角度帧，由 OnTimer 驱动旋转
	CDC memDC;
	CBitmap bm;
	memDC.CreateCompatibleDC(pDC);
	bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBm = memDC.SelectObject(&bm);

	memDC.FillSolidRect(rect, pDC->GetBkColor());
	DrawObject(&memDC, -m_angle);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBm);
}

void CTaiJiView::DrawObject(CDC* pDC, int i) {
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	rect.OffsetRect(-rect.Width() / 2, -rect.Height() / 2);

	CBrush brush1, brush2, * Brush;
	int R1 = 320, R2, R3;
	R2 = R1 / 2;
	R3 = R1 / 8;
	CPoint point1, point2, point3, point4;
	int x1, y1, x2, y2;

	x1 = Round(R1 * cos((i + 90) * PI / 180));
	y1 = Round(R1 * sin((i + 90) * PI / 180));
	x2 = Round(R1 * cos((i - 90) * PI / 180));
	y2 = Round(R1 * sin((i - 90) * PI / 180));

	point1 = CPoint(-R1, -R1);
	point2 = CPoint(R1, R1);
	point3 = CPoint(x1, y1);
	point4 = CPoint(x2, y2);
	COLORREF color1 = RGB(0, 0, 0);
	//半黑圆
	brush1.CreateSolidBrush(color1);
	Brush = pDC->SelectObject(&brush1);
	pDC->Pie(CRect(point1, point2), point4, point3);
	pDC->SelectObject(Brush);
	brush1.DeleteObject();
	//半白圆
	COLORREF color2 = RGB(255, 255, 255);
	brush2.CreateSolidBrush(color2);
	Brush = pDC->SelectObject(&brush2); // 原代码误选 brush1，已修正
	pDC->Pie(CRect(point1, point2), point3, point4);
	pDC->SelectObject(Brush);
	brush2.DeleteObject();
	//黑鱼头
	int m1, n1, m2, n2;
	m1 = Round(R2 * cos((i - 90) * PI / 180) - R2);
	n1 = Round(R2 * sin((i - 90) * PI / 180) - R2);
	m2 = Round(R2 * cos((i - 90) * PI / 180) + R2);
	n2 = Round(R2 * sin((i - 90) * PI / 180) + R2);
	CBrush brush3;
	brush3.CreateSolidBrush(color1);
	Brush = pDC->SelectObject(&brush3);
	pDC->Ellipse(m1, n1, m2, n2);
	pDC->SelectObject(Brush);
	brush3.DeleteObject();
	//白鱼头（实心白刷，原版仅用白笔为空心）
	m1 = Round(R2 * cos((i + 90) * PI / 180) - R2);
	n1 = Round(R2 * sin((i + 90) * PI / 180) - R2);
	m2 = Round(R2 * cos((i + 90) * PI / 180) + R2);
	n2 = Round(R2 * sin((i + 90) * PI / 180) + R2);
	CBrush brushWhiteHead;
	brushWhiteHead.CreateSolidBrush(color2);
	Brush = pDC->SelectObject(&brushWhiteHead);
	pDC->Ellipse(m1, n1, m2, n2);
	pDC->SelectObject(Brush);
	brushWhiteHead.DeleteObject();
	//白鱼眼（实心）
	m1 = Round(R2 * cos((i - 90) * PI / 180) - R3);
	n1 = Round(R2 * sin((i - 90) * PI / 180) - R3);
	m2 = Round(R2 * cos((i - 90) * PI / 180) + R3);
	n2 = Round(R2 * sin((i - 90) * PI / 180) + R3);
	CBrush brushWhiteEye;
	brushWhiteEye.CreateSolidBrush(color2);
	Brush = pDC->SelectObject(&brushWhiteEye);
	pDC->Ellipse(m1, n1, m2, n2);
	pDC->SelectObject(Brush);
	brushWhiteEye.DeleteObject();
	//黑鱼眼
	m1 = Round(R2 * cos((i + 90) * PI / 180) - R3);
	n1 = Round(R2 * sin((i + 90) * PI / 180) - R3);
	m2 = Round(R2 * cos((i + 90) * PI / 180) + R3);
	n2 = Round(R2 * sin((i + 90) * PI / 180) + R3);
	CBrush brush4;
	brush4.CreateSolidBrush(color1);
	Brush = pDC->SelectObject(&brush4);
	pDC->Ellipse(m1, n1, m2, n2);
	pDC->SelectObject(Brush);
	brush4.DeleteObject();
}

// CTaiJiView 打印


void CTaiJiView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTaiJiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTaiJiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTaiJiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTaiJiView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTaiJiView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTaiJiView 诊断

#ifdef _DEBUG
void CTaiJiView::AssertValid() const
{
	CView::AssertValid();
}

void CTaiJiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTaiJiDoc* CTaiJiView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaiJiDoc)));
	return (CTaiJiDoc*)m_pDocument;
}
#endif //_DEBUG


// CTaiJiView 消息处理程序
