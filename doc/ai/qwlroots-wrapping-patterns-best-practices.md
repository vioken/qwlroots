# qwlroots 封装模式最佳实践指南

## 概述

qwlroots 是对 wlroots C 接口的 Qt 风格封装库，通过宏系统提供了多种封装模式来适应不同类型的 wlroots 结构体。本文档总结了项目中使用的几种主要封装模式，为添加新的类封装提供指导。

## 核心设计理念

1. **Qt 风格接口**：遵循 Qt 规范的接口风格
2. **信号槽机制**：将 wl_signal 封装为 Qt 信号
3. **内存管理**：自动处理对象生命周期和所有权
4. **类型安全**：利用 C++ 模板和概念确保类型安全
5. **零开销抽象**：在 Release 模式下提供零开销的封装

## 封装模式分类

### 1. QW_CLASS_OBJECT 模式 - 基于 QObject 的完整封装

**适用场景**：
- wlroots 结构体包含信号（events 字段）
- 需要完整的对象生命周期管理
- 需要 Qt 信号槽机制

**特点**：
- 继承自 `qw_object<Handle, Derive>`，间接继承 QObject
- 自动连接 wlroots 的 destroy 信号
- 支持信号绑定和转发
- 提供完整的内存管理和所有权控制

**使用示例**：
```cpp
// 头文件声明
class QW_CLASS_OBJECT(compositor)
{
    QW_OBJECT           // 必需的宏，提供基础功能
    Q_OBJECT           // Qt 元对象系统

    // 信号绑定 - 将 wlroots 信号转换为 Qt 信号
    QW_SIGNAL(new_surface, wlr_surface*)

public:
    // 静态函数绑定 - create 函数自动返回 qw_compositor*
    QW_FUNC_STATIC(compositor, create, qw_compositor *, wl_display *display, uint32_t version, wlr_renderer *renderer)
};

class QW_CLASS_OBJECT(surface)
{
    QW_OBJECT
    Q_OBJECT

    // 多个信号绑定
    QW_SIGNAL(client_commit)
    QW_SIGNAL(commit)
    QW_SIGNAL(new_subsurface, wlr_subsurface*)
    QW_SIGNAL(map)
    QW_SIGNAL(unmap)

public:
    // 静态函数 - from/get 函数
    QW_FUNC_STATIC(surface, from_resource, qw_surface *, wl_resource *resource)

    // 成员函数绑定
    QW_FUNC_MEMBER(surface, for_each_surface, void, wlr_surface_iterator_func_t iterator, void *user_data)
    QW_FUNC_MEMBER(surface, has_buffer, bool)
    QW_FUNC_MEMBER(surface, send_frame_done, void, const timespec *when)
    QW_FUNC_MEMBER(surface, map, void)
    QW_FUNC_MEMBER(surface, unmap, void)
};
```

**关键宏说明**：
- `QW_OBJECT`：提供基础的对象功能，必须在 protected 区域
- `QW_SIGNAL(name, ...)`：绑定 wlroots 信号到 Qt 信号
- `QW_FUNC_STATIC`：绑定静态函数，create/from/get 函数自动处理返回类型
- `QW_FUNC_MEMBER`：绑定成员函数

### 2. QW_CLASS_REINTERPRET_CAST 模式 - 轻量级类型转换

**适用场景**：
- wlroots 结构体不包含信号
- 只需要函数封装，不需要对象生命周期管理
- 追求最小内存开销

**特点**：
- 基于 `reinterpret_cast` 的零开销封装
- 不分配额外内存
- 不支持信号机制
- 主要用于工具类和简单结构体

**使用示例**：
```cpp
class QW_CLASS_REINTERPRET_CAST(client_buffer)
{
public:
    QW_FUNC_STATIC(client_buffer, get, qw_client_buffer *, wlr_buffer *buffer)
};
```

### 3. QW_CLASS_BOX 模式 - 值类型封装

**适用场景**：
- 简单的数据结构
- 需要与 Qt 类型互操作
- 值语义的对象

**特点**：
- 包含实际的数据成员
- 支持与 Qt 类型的转换
- 值语义，支持拷贝和赋值

**使用示例**：
```cpp
class qw_box
{
public:
    explicit qw_box() = default;
    explicit qw_box(const wlr_box &box);
    explicit qw_box(const QRect &box);

    QRect toQRect();
    operator wlr_box*();

private:
    wlr_box m_handle;
};
```

### 4. QW_CLASS_INTERFACE 模式 - 接口实现封装

**适用场景**：
- 需要实现 wlroots 接口
- 提供回调函数实现
- 自定义行为扩展

**特点**：
- 基于模板的接口绑定
- 自动处理函数指针绑定
- 支持虚函数重写

**使用示例**：
```cpp
class QW_CLASS_INTERFACE(output)
{
    QW_INTERFACE_INIT(output)

    // 接口函数实现
    QW_INTERFACE(commit, bool, wlr_output_state *state)
    QW_INTERFACE(set_cursor, bool, wlr_buffer *buffer, int hotspot_x, int hotspot_y)

protected:
    // 实现具体的接口函数
    bool commit(wlr_output_state *state);
    bool set_cursor(wlr_buffer *buffer, int hotspot_x, int hotspot_y);
};
```

## 目录结构和文件命名规律

qwlroots 的目录结构和文件命名完全遵循 wlroots 的组织方式，这确保了项目结构的一致性和可维护性。

### 目录结构对应关系

qwlroots 的 `src/` 目录结构与 wlroots 的 `include/wlr/` 目录结构完全对应：

```
wlroots/include/wlr/          qwlroots/src/
├── types/                 ←→ ├── types/
├── util/                  ←→ ├── util/
├── render/                ←→ ├── render/
├── interfaces/            ←→ ├── interfaces/
├── backend/               ←→ ├── (backend相关文件直接在src/)
└── xwayland/              ←→ └── (xwayland相关文件在types/)
```

### 文件命名规律

1. **头文件命名**：
   - wlroots: `wlr_compositor.h` → qwlroots: `qwcompositor.h`
   - wlroots: `wlr_output.h` → qwlroots: `qwoutput.h`
   - wlroots: `wlr_buffer.h` → qwlroots: `qwbuffer.h`

2. **类名命名**：
   - wlroots: `struct wlr_compositor` → qwlroots: `class qw_compositor`
   - wlroots: `struct wlr_surface` → qwlroots: `class qw_surface`

3. **目录分类规律**：
   - `types/`: 主要的 Wayland 协议类型和核心对象
   - `util/`: 工具类和辅助函数
   - `render/`: 渲染相关的类型
   - `interfaces/`: 接口实现相关的类型

### 查找对应关系的方法

当需要为新的 wlroots 类型创建封装时：

1. **定位源文件**：在 `wlroots/include/wlr/` 中找到对应的头文件
2. **确定目录**：根据 wlroots 的目录结构确定在 qwlroots 中的位置
3. **命名转换**：按照 `wlr_xxx.h` → `qwxxx.h` 的规律命名

**示例**：
```bash
# wlroots 中的文件
wlroots/include/wlr/types/wlr_xdg_shell.h

# 对应的 qwlroots 文件应该是
qwlroots/src/types/qwxdgshell.h
```

## 新增封装的完整操作流程

当用户请求"新增xxx类"或"新增xxxx函数"时，应按照以下流程操作：

### 步骤 0：更新 wlroots 子模块

在开始任何封装工作之前，首先确保 wlroots 子模块是最新的：

```bash
# 进入 wlroots 子模块目录
cd wlroots

# 获取最新的标签和提交
git fetch --tags

# 查看最新的版本标签
git tag --sort=-version:refname | head -5

# 切换到最新的稳定版本（例如 0.19.0）
git checkout 0.19.0

# 返回项目根目录
cd ..

# 提交子模块更新
git add wlroots
git commit -m "submodule: update wlroots to latest version"
```

### 步骤 1：在 wlroots 子模块中查找目标类型

使用以下方法在 wlroots 子模块中定位要封装的类型：

```bash
# 查找特定的头文件
find wlroots/include/wlr -name "*目标名称*" -type f

# 搜索结构体定义
grep -r "struct wlr_目标名称" wlroots/include/wlr/

# 查看具体文件内容
cat wlroots/include/wlr/types/wlr_目标名称.h
```

### 步骤 2：确定版本信息

查找该类型是在哪个版本中引入的：

```bash
# 在 wlroots 目录中查看文件的提交历史
cd wlroots
git log --oneline --follow include/wlr/types/wlr_目标名称.h

# 查看特定提交的详细信息
git show 提交哈希

# 查找包含该提交的最早标签
git tag --contains 提交哈希 --sort=version:refname | head -1
```

这个信息将用于在 qwlroots 中添加版本兼容性宏。

### 步骤 3：分析 wlroots 结构体

1. **检查是否有 events 字段**：
   - 有 events 字段 → 使用 `QW_CLASS_OBJECT`
   - 无 events 字段 → 使用 `QW_CLASS_REINTERPRET_CAST`

2. **检查是否需要接口实现**：
   - 需要实现回调 → 使用 `QW_CLASS_INTERFACE`

3. **检查是否为简单数据结构**：
   - 简单数据结构 → 考虑自定义封装或 `QW_CLASS_BOX`

### 步骤 2：确定文件位置和命名

根据 wlroots 源文件的位置确定对应的 qwlroots 文件位置：

```bash
# 1. 找到 wlroots 中的源文件
find /path/to/wlroots/include/wlr -name "wlr_example.h"
# 例如：wlroots/include/wlr/types/wlr_example.h

# 2. 确定 qwlroots 中的对应位置
# src/types/qwexample.h
```

### 步骤 3：创建头文件

```cpp
// 文件：src/types/qwexample.h
#pragma once

#include <qwobject.h>

extern "C" {
#include <wlr/types/wlr_example.h>
}

QW_BEGIN_NAMESPACE

class QW_CLASS_OBJECT(example)
{
    QW_OBJECT
    Q_OBJECT

    // 绑定所有相关信号
    QW_SIGNAL(signal_name, param_type)

public:
    // 绑定静态函数
    QW_FUNC_STATIC(example, create, qw_example *, param_type param)
    QW_FUNC_STATIC(example, from_resource, qw_example *, wl_resource *resource)

    // 绑定成员函数
    QW_FUNC_MEMBER(example, method_name, return_type, param_type param)

    // 如果有 destroy 方法，析构函数应该是 protected
protected:
    QW_FUNC_MEMBER(example, destroy, void)
};

QW_END_NAMESPACE
```

### 步骤 4：处理版本兼容性

```cpp
// 处理不同 wlroots 版本的 API 差异
#if WLR_VERSION_MINOR < 18
    QW_SIGNAL(old_signal, param_type)
#else
    QW_SIGNAL(new_signal, param_type)
#endif

#if WLR_VERSION_MINOR < 19
    QW_FUNC_MEMBER(example, old_method, void, param_type)
#else
    QW_FUNC_MEMBER(example, new_method, void, param_type)
#endif
```

### 步骤 5：添加到构建系统

在 `src/CMakeLists.txt` 中添加新的头文件：

```cmake
set(HEADERS
    # ... 其他头文件
    types/qwexample.h
)
```

## 最佳实践和注意事项

### 1. 命名规范

- 类名：`qw_` + wlroots 类型名（去掉 `wlr_` 前缀）
- 信号名：`notify_` + 原始信号名
- 函数名：保持与 wlroots 函数名一致（去掉类型前缀）

### 2. 信号处理

```cpp
// 正确的信号绑定
QW_SIGNAL(destroy)           // 自动处理，无需手动绑定
QW_SIGNAL(custom_event, param_type)  // 自定义信号

// 信号连接示例
connect(surface, &qw_surface::notify_commit, this, [](){
    // 处理提交事件
});
```

### 3. 内存管理

```cpp
// 创建对象（拥有所有权）
auto *compositor = qw_compositor::create(display, version, renderer);

// 从现有句柄获取对象（不拥有所有权）
auto *surface = qw_surface::from(wlr_surface);

// 检查对象是否存在
auto *existing = qw_surface::get(wlr_surface);  // 可能返回 nullptr
```

### 4. 函数绑定原则

```cpp
// 静态函数绑定
QW_FUNC_STATIC(type, create, qw_type *, ...)    // 创建函数，返回 qw 类型
QW_FUNC_STATIC(type, from, qw_type *, ...)      // 转换函数，返回 qw 类型
QW_FUNC_STATIC(type, get, qw_type *, ...)       // 获取函数，返回 qw 类型
QW_FUNC_STATIC(type, utility, return_type, ...) // 工具函数，返回原始类型

// 成员函数绑定
QW_FUNC_MEMBER(type, method, return_type, ...)  // 实例方法
```

### 5. 错误处理

```cpp
// 检查对象有效性
if (!surface->is_valid()) {
    return;
}

// 安全的句柄访问
if (auto *handle = surface->handle()) {
    // 使用 handle
}
```

### 6. 版本兼容性处理

- 使用条件编译处理 API 变化
- 优先支持最新版本的 API
- 为旧版本提供兼容性宏

### 7. 测试

为新的封装类创建测试：

```cpp
// tests/test_qwexample.cpp
#include "qwexample.h"
#include <QtTest>

class TestQwExample : public QObject
{
    Q_OBJECT

private slots:
    void testCreate();
    void testSignals();
    void testMethods();
};
```

## 常见问题和解决方案

### 1. 编译错误：找不到 wlroots 函数

**问题**：使用 `QW_FUNC_STATIC` 或 `QW_FUNC_MEMBER` 时编译失败。

**解决方案**：
- 检查 wlroots 版本兼容性
- 确认函数名拼写正确
- 使用条件编译处理版本差异

### 2. 信号连接失败

**问题**：Qt 信号无法正常触发。

**解决方案**：
- 确保使用了 `QW_OBJECT` 和 `Q_OBJECT` 宏
- 检查信号参数类型是否正确
- 验证 wlroots 结构体确实有对应的信号

### 3. 内存泄漏或崩溃

**问题**：对象销毁时出现问题。

**解决方案**：
- 正确处理对象所有权
- 对于有 destroy 方法的类，将析构函数设为 protected
- 使用智能指针管理生命周期

## 总结

qwlroots 的封装模式设计精巧，通过模板和宏系统提供了灵活而高效的 C++ 封装。在添加新的类封装时，应该：

1. 仔细分析 wlroots 结构体的特性
2. 选择合适的封装模式
3. 遵循项目的命名和代码规范
4. 处理版本兼容性问题
5. 编写相应的测试代码

通过遵循这些最佳实践，可以确保新添加的封装类与项目整体架构保持一致，并提供良好的开发体验。
