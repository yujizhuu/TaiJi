
// TaiJiView.h: CTaiJiView 类的接口
//

#pragma once


class CTaiJiView : public CView
{
protected: // 仅从序列化创建
	CTaiJiView() noexcept;
	DECLARE_DYNCREATE(CTaiJiView)

// 特性
public:
	CTaiJiDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual void DrawObject(CDC* pDC, int i);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTaiJiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int m_angle;      // 当前旋转角度（度）
	UINT_PTR m_timerId;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // TaiJiView.cpp 中的调试版本
inline CTaiJiDoc* CTaiJiView::GetDocument() const
   { return reinterpret_cast<CTaiJiDoc*>(m_pDocument); }
#endif

