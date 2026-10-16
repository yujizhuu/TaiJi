# TaiJi

用 **MFC** 在 Windows 上绘制并旋转「太极阴阳鱼」的小示例（Visual Studio 2019）。

> 学习 / 演示项目：核心改动在 `TaiJiView` 的绘制与定时器动画；其余多为 MFC 应用程序向导生成的框架代码。

## 效果预览

![太极旋转预览](docs/preview.gif)

上图由与 `DrawObject` 相同的几何算法生成（半圆 `Pie` + 鱼头/鱼眼 `Ellipse`），便于在 GitHub 上直接查看。**真实可执行程序仍需 Windows + Visual Studio + MFC**，本仓库无法在 macOS / Linux 上直接跑 `.exe`。

静态帧见 [`docs/preview.png`](docs/preview.png)。

## 动画实现

- `OnCreate` 中 `SetTimer`（约 16ms，~60fps）驱动旋转
- `OnTimer` 递增角度并 `Invalidate`，由 `OnDraw` 双缓冲绘制当前帧
- **不再**在 `OnDraw` 里用 `Sleep` 死循环（旧写法会卡住 UI 消息泵）

## 环境要求

- Windows
- [Visual Studio 2019](https://visualstudio.microsoft.com/)（或兼容版本）
- 安装工作负载：**使用 C++ 的桌面开发**，并勾选 **MFC** 组件

## 如何运行

1. 克隆本仓库  
   `git clone https://github.com/yujizhuu/TaiJi.git`
2. 用 Visual Studio 打开 `TaiJi.sln`
3. 选择 Debug / Release，生成并运行（本地 Windows 调试）

## 代码说明

| 文件 | 作用 |
|------|------|
| `TaiJiView.cpp` / `TaiJiView.h` | **主要自定义代码**：`OnDraw`、`DrawObject`、`OnTimer` |
| `docs/preview.gif` | 算法匹配的旋转预览 |
| `TaiJi.sln` / `TaiJi.vcxproj` | Visual Studio 解决方案与工程 |
| 其余 `*.cpp` / `*.h` / `res/` | MFC 多文档界面框架与资源（向导生成） |

绘制使用 `Pie` / `Ellipse` 组合阴阳鱼形状；白半圆刷子选择与实心白鱼头/白鱼眼已按意图修正。

## 许可证

本仓库以 [MIT](LICENSE) 发布。
