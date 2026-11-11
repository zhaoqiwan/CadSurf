# CadSurf STL 布局

计算机图形学第二次课程作业。项目在课程提供的 CadSurf 上增加了 STL 导入、多零件布局和交互编辑功能。

## 编译与运行

在 Windows 上使用 Visual Studio，安装「使用 C++ 的桌面开发」工作负载，并包含 MSVC v143、对应的 MFC 组件和 Windows SDK。

1. 克隆仓库，或下载并解压整个仓库。
2. 用 Visual Studio 打开根目录的 `CadSurf.sln`，将 `CadSurf` 设为启动项目。
3. 选择 `Debug | x64`，按 `Ctrl+F5` 生成并运行。

`CadSurf` 是主程序，`VKGeom` 和 `VKGraphic` 分别提供几何计算和图形显示功能。

## 基本操作

| 操作 | 方法 |
| --- | --- |
| 导入 | `File → Import STL...`。支持二进制 STL，可连续导入多个零件。 |
| 选择 | 单击选中零件，按住 `Shift` 单击可多选。 |
| 移动 | 在 `STL → Move` 中输入 XYZ 偏移量，或选择两个已有点或直线端点来指定移动量。 |
| 旋转与缩放 | 使用 `STL → Rotate...` 或 `STL → Scale...`。可绕世界坐标轴或选定直线旋转，缩放按统一比例进行。 |
| 自动排列 | `STL → Auto Arrange on XOY`，以 XOY 平面为基准，沿 X 方向排列零件。 |
| 观察布局 | `STL → View` 提供轴向视图和四个等轴测视图。按住 `Ctrl` 拖动鼠标右键可自由旋转视角。 |
| 删除 | `Delete` 键或 `Edit → Delete Selected` 删除选中对象，`Edit → Delete All` 删除全部对象。 |
| 撤销与重做 | `Ctrl+Z` / `Ctrl+Y`。支持导入、变换、自动排列和删除，多选操作按一步记录。 |

仓库未包含 STL 模型，测试导入时请自行准备二进制 STL 文件。

## 保存与重新打开

按 `Ctrl+S`，将当前 STL 布局保存为 `.srf` 文件。文件内嵌各零件的网格，保留位置、旋转、缩放、颜色和显隐状态。

按 `Ctrl+O` 打开 `.srf` 即可恢复布局，无需原始 STL 文件。打开布局后，编辑操作从新的撤销记录开始。
