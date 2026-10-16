# TaiJi

用 **MFC** 在 Windows 上绘制并旋转「太极阴阳鱼」的小示例（Visual Studio 2019）。

> 学习 / 演示项目：核心改动在 `TaiJiView` 的绘制逻辑；其余多为 MFC 应用程序向导生成的框架代码。

## 效果

- 在客户区中心绘制黑白阴阳鱼
- 通过角度递增实现连续旋转动画

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
| `TaiJiView.cpp` / `TaiJiView.h` | **主要自定义代码**：`OnDraw`、`DrawObject` 绘制与旋转 |
| `TaiJi.sln` / `TaiJi.vcxproj` | Visual Studio 解决方案与工程 |
| 其余 `*.cpp` / `*.h` / `res/` | MFC 多文档界面框架与资源（向导生成） |

绘制使用 `Pie` / `Ellipse` 组合阴阳鱼形状；`OnDraw` 内用内存 DC 双缓冲，再按角度循环刷新。

## 已知限制

- 动画循环写在 `OnDraw` 里并用 `Sleep` 延时，会卡住 UI 消息泵；更规范的做法是用定时器（`SetTimer` / `OnTimer`）驱动重绘。本仓库保持原学习版逻辑，便于对照教材。
- 仅支持 Windows + MFC，无法在 macOS / Linux 直接运行。

## 许可证

本仓库以 [MIT](LICENSE) 发布。
