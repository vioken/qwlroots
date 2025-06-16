# qwlroots Wrapping Patterns Best Practices Guide

## Overview

qwlroots is a Qt-style wrapper library for wlroots C interfaces, providing multiple wrapping patterns through a macro system to accommodate different types of wlroots structures. This document summarizes the main wrapping patterns used in the project and provides guidance for adding new class wrappers.

## Core Design Philosophy

1. **Qt-style Interface**: Follows Qt specification interface style
2. **Signal-Slot Mechanism**: Wraps wl_signal as Qt signals
3. **Memory Management**: Automatic object lifecycle and ownership handling
4. **Type Safety**: Uses C++ templates and concepts to ensure type safety
5. **Zero-cost Abstraction**: Provides zero-overhead wrapping in Release mode

## Wrapping Pattern Classification

### 1. QW_CLASS_OBJECT Pattern - Complete QObject-based Wrapping

**Use Cases**:
- wlroots structures contain signals (events field)
- Need complete object lifecycle management
- Require Qt signal-slot mechanism

**Features**:
- Inherits from `qw_object<Handle, Derive>`, indirectly inherits QObject
- Automatically connects wlroots destroy signals
- Supports signal binding and forwarding
- Provides complete memory management and ownership control

**Usage Example**:
```cpp
// Header file declaration
class QW_CLASS_OBJECT(compositor)
{
    QW_OBJECT           // Required macro, provides basic functionality
    Q_OBJECT           // Qt meta-object system

    // Signal binding - converts wlroots signals to Qt signals
    QW_SIGNAL(new_surface, wlr_surface*)

public:
    // Static function binding - create function automatically returns qw_compositor*
    QW_FUNC_STATIC(compositor, create, qw_compositor *, wl_display *display, uint32_t version, wlr_renderer *renderer)
};

class QW_CLASS_OBJECT(surface)
{
    QW_OBJECT
    Q_OBJECT

    // Multiple signal bindings
    QW_SIGNAL(client_commit)
    QW_SIGNAL(commit)
    QW_SIGNAL(new_subsurface, wlr_subsurface*)
    QW_SIGNAL(map)
    QW_SIGNAL(unmap)

public:
    // Static functions - from/get functions
    QW_FUNC_STATIC(surface, from_resource, qw_surface *, wl_resource *resource)

    // Member function binding
    QW_FUNC_MEMBER(surface, for_each_surface, void, wlr_surface_iterator_func_t iterator, void *user_data)
    QW_FUNC_MEMBER(surface, has_buffer, bool)
    QW_FUNC_MEMBER(surface, send_frame_done, void, const timespec *when)
    QW_FUNC_MEMBER(surface, map, void)
    QW_FUNC_MEMBER(surface, unmap, void)
};
```

**Key Macro Descriptions**:
- `QW_OBJECT`: Provides basic object functionality, must be in protected section
- `QW_SIGNAL(name, ...)`: Binds wlroots signals to Qt signals
- `QW_FUNC_STATIC`: Binds static functions, create/from/get functions automatically handle return types
- `QW_FUNC_MEMBER`: Binds member functions

### 2. QW_CLASS_REINTERPRET_CAST Pattern - Lightweight Type Conversion

**Use Cases**:
- wlroots structures don't contain signals
- Only need function wrapping, no object lifecycle management
- Pursue minimal memory overhead

**Features**:
- Zero-overhead wrapping based on `reinterpret_cast`
- No additional memory allocation
- No signal mechanism support
- Mainly used for utility classes and simple structures

**Usage Example**:
```cpp
class QW_CLASS_REINTERPRET_CAST(client_buffer)
{
public:
    QW_FUNC_STATIC(client_buffer, get, qw_client_buffer *, wlr_buffer *buffer)
};
```

### 3. QW_CLASS_BOX Pattern - Value Type Wrapping

**Use Cases**:
- Simple data structures
- Need interoperability with Qt types
- Value semantic objects

**Features**:
- Contains actual data members
- Supports conversion with Qt types
- Value semantics, supports copy and assignment

**Usage Example**:
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

### 4. QW_CLASS_INTERFACE Pattern - Interface Implementation Wrapping

**Use Cases**:
- Need to implement wlroots interfaces
- Provide callback function implementations
- Custom behavior extensions

**Features**:
- Template-based interface binding
- Automatic function pointer binding
- Supports virtual function overriding

**Usage Example**:
```cpp
class QW_CLASS_INTERFACE(output)
{
    QW_INTERFACE_INIT(output)

    // Interface function implementations
    QW_INTERFACE(commit, bool, wlr_output_state *state)
    QW_INTERFACE(set_cursor, bool, wlr_buffer *buffer, int hotspot_x, int hotspot_y)

protected:
    // Implement specific interface functions
    bool commit(wlr_output_state *state);
    bool set_cursor(wlr_buffer *buffer, int hotspot_x, int hotspot_y);
};
```

## Directory Structure and File Naming Patterns

qwlroots' directory structure and file naming completely follow wlroots' organization, ensuring project structure consistency and maintainability.

### Directory Structure Correspondence

qwlroots' `src/` directory structure completely corresponds to wlroots' `include/wlr/` directory structure:

```
wlroots/include/wlr/          qwlroots/src/
├── types/                 ←→ ├── types/
├── util/                  ←→ ├── util/
├── render/                ←→ ├── render/
├── interfaces/            ←→ ├── interfaces/
├── backend/               ←→ ├── (backend-related files directly in src/)
└── xwayland/              ←→ └── (xwayland-related files in types/)
```

### File Naming Patterns

1. **Header File Naming**:
   - wlroots: `wlr_compositor.h` → qwlroots: `qwcompositor.h`
   - wlroots: `wlr_output.h` → qwlroots: `qwoutput.h`
   - wlroots: `wlr_buffer.h` → qwlroots: `qwbuffer.h`

2. **Class Name Naming**:
   - wlroots: `struct wlr_compositor` → qwlroots: `class qw_compositor`
   - wlroots: `struct wlr_surface` → qwlroots: `class qw_surface`

3. **Directory Classification Patterns**:
   - `types/`: Main Wayland protocol types and core objects
   - `util/`: Utility classes and helper functions
   - `render/`: Rendering-related types
   - `interfaces/`: Interface implementation-related types

### Method for Finding Correspondences

When creating wrappers for new wlroots types:

1. **Locate Source File**: Find the corresponding header file in `wlroots/include/wlr/`
2. **Determine Directory**: Determine the location in qwlroots based on wlroots' directory structure
3. **Name Conversion**: Follow the `wlr_xxx.h` → `qwxxx.h` pattern for naming

**Example**:
```bash
# File in wlroots
wlroots/include/wlr/types/wlr_xdg_shell.h

# Corresponding qwlroots file should be
qwlroots/src/types/qwxdgshell.h
```

## Complete Workflow for Adding New Wrappers

When users request "add xxx class" or "add xxxx function", follow this workflow:

### Step 0: Update wlroots Submodule

Before starting any wrapping work, ensure the wlroots submodule is up to date:

```bash
# Enter wlroots submodule directory
cd wlroots

# Fetch latest tags and commits
git fetch --tags

# View latest version tags
git tag --sort=-version:refname | head -5

# Checkout to latest stable version (e.g., 0.19.0)
git checkout 0.19.0

# Return to project root
cd ..

# Commit submodule update
git add wlroots
git commit -m "submodule: update wlroots to latest version"
```

### Step 1: Locate Target Type in wlroots Submodule

Use the following methods to locate the type to be wrapped in the wlroots submodule:

```bash
# Find specific header files
find wlroots/include/wlr -name "*target_name*" -type f

# Search for struct definitions
grep -r "struct wlr_target_name" wlroots/include/wlr/

# View specific file content
cat wlroots/include/wlr/types/wlr_target_name.h
```

### Step 2: Determine Version Information

Find out which version introduced this type:

```bash
# View file commit history in wlroots directory
cd wlroots
git log --oneline --follow include/wlr/types/wlr_target_name.h

# View detailed information of specific commit
git show commit_hash

# Find earliest tag containing this commit
git tag --contains commit_hash --sort=version:refname | head -1
```

This information will be used to add version compatibility macros in qwlroots.

### Step 3: Analyze wlroots Structure

1. **Check for events field**:
   - Has events field → Use `QW_CLASS_OBJECT`
   - No events field → Use `QW_CLASS_REINTERPRET_CAST`

2. **Check if interface implementation is needed**:
   - Need callback implementation → Use `QW_CLASS_INTERFACE`

3. **Check if it's a simple data structure**:
   - Simple data structure → Consider custom wrapping or `QW_CLASS_BOX`

### Step 2: Determine File Location and Naming

Determine the corresponding qwlroots file location based on the wlroots source file location:

```bash
# 1. Find the source file in wlroots
find /path/to/wlroots/include/wlr -name "wlr_example.h"
# Example: wlroots/include/wlr/types/wlr_example.h

# 2. Determine corresponding location in qwlroots
# src/types/qwexample.h
```

### Step 3: Create Header File

```cpp
// File: src/types/qwexample.h
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

    // Bind all related signals
    QW_SIGNAL(signal_name, param_type)

public:
    // Bind static functions
    QW_FUNC_STATIC(example, create, qw_example *, param_type param)
    QW_FUNC_STATIC(example, from_resource, qw_example *, wl_resource *resource)

    // Bind member functions
    QW_FUNC_MEMBER(example, method_name, return_type, param_type param)

    // If there's a destroy method, destructor should be protected
protected:
    QW_FUNC_MEMBER(example, destroy, void)
};

QW_END_NAMESPACE
```

### Step 4: Handle Version Compatibility

```cpp
// Handle API differences between different wlroots versions
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

### Step 5: Add to Build System

Add the new header file to `src/CMakeLists.txt`:

```cmake
set(HEADERS
    # ... other header files
    types/qwexample.h
)
```

## Best Practices and Considerations

### 1. Naming Conventions

- Class names: `qw_` + wlroots type name (remove `wlr_` prefix)
- Signal names: `notify_` + original signal name
- Function names: Keep consistent with wlroots function names (remove type prefix)

### 2. Signal Handling

```cpp
// Correct signal binding
QW_SIGNAL(destroy)           // Automatically handled, no manual binding needed
QW_SIGNAL(custom_event, param_type)  // Custom signals

// Signal connection example
connect(surface, &qw_surface::notify_commit, this, [](){
    // Handle commit event
});
```

### 3. Memory Management

```cpp
// Create object (owns ownership)
auto *compositor = qw_compositor::create(display, version, renderer);

// Get object from existing handle (doesn't own ownership)
auto *surface = qw_surface::from(wlr_surface);

// Check if object exists
auto *existing = qw_surface::get(wlr_surface);  // May return nullptr
```

### 4. Function Binding Principles

```cpp
// Static function binding
QW_FUNC_STATIC(type, create, qw_type *, ...)    // Create function, returns qw type
QW_FUNC_STATIC(type, from, qw_type *, ...)      // Conversion function, returns qw type
QW_FUNC_STATIC(type, get, qw_type *, ...)       // Get function, returns qw type
QW_FUNC_STATIC(type, utility, return_type, ...) // Utility function, returns original type

// Member function binding
QW_FUNC_MEMBER(type, method, return_type, ...)  // Instance method
```

### 5. Error Handling

```cpp
// Check object validity
if (!surface->is_valid()) {
    return;
}

// Safe handle access
if (auto *handle = surface->handle()) {
    // Use handle
}
```

### 6. Version Compatibility Handling

- Use conditional compilation to handle API changes
- Prioritize supporting the latest version API
- Provide compatibility macros for older versions

### 7. Testing

Create tests for new wrapper classes:

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

## Common Issues and Solutions

### 1. Compilation Error: wlroots Function Not Found

**Problem**: Compilation fails when using `QW_FUNC_STATIC` or `QW_FUNC_MEMBER`.

**Solution**:
- Check wlroots version compatibility
- Confirm function name spelling is correct
- Use conditional compilation to handle version differences

### 2. Signal Connection Failure

**Problem**: Qt signals don't trigger properly.

**Solution**:
- Ensure `QW_OBJECT` and `Q_OBJECT` macros are used
- Check signal parameter types are correct
- Verify wlroots structure actually has corresponding signals

### 3. Memory Leaks or Crashes

**Problem**: Issues occur during object destruction.

**Solution**:
- Handle object ownership correctly
- For classes with destroy methods, make destructor protected
- Use smart pointers to manage lifecycle

## Summary

qwlroots' wrapping pattern design is sophisticated, providing flexible and efficient C++ wrapping through template and macro systems. When adding new class wrappers, you should:

1. Carefully analyze wlroots structure characteristics
2. Choose appropriate wrapping patterns
3. Follow project naming and coding conventions
4. Handle version compatibility issues
5. Write corresponding test code

By following these best practices, you can ensure that newly added wrapper classes remain consistent with the overall project architecture and provide a good development experience.
