# 🛠️ 3D CNC G-Code Interpreter & Visualizer

A modern **3D CNC G-Code Interpreter & Visualizer** that parses CNC G-code files asynchronously and renders the resulting toolpath in an interactive OpenGL window. This project demonstrates **real-world system design**, **modern C++ practices**, and **graphics programming**, making it ideal for learning.

![Demo](assets/demo.png)
*Real-time 3D visualization of CNC toolpaths*

---

## ✨ Features

- 📄 **G-Code Parsing** - Parse standard CNC G-code files (`.gcode`, `.txt`)
- ⚙️ **Command Support** - Supports G0, G1, G2, G3 motion commands
- 🧵 **Asynchronous Processing** - Non-blocking parsing using C++17 `std::async`
- 🧹 **RAII Design** - Safe resource management (GLFW, file I/O)
- 🎮 **Interactive 3D Visualization** - Real-time OpenGL rendering
- 🎥 **Playback Controls** - Step-by-step toolpath animation
- 🖱️ **Camera Controls** - Mouse and keyboard navigation
- 🧩 **Modular Architecture** - Clean, extensible codebase

---

## 🧠 Core Concepts

This project demonstrates professional software engineering practices:

### Modern C++17
- RAII (Resource Acquisition Is Initialization)
- STL containers (`std::vector`, `std::future`)
- Exception safety and error handling
- Smart memory management

### Concurrency
- Background parsing with `std::async`
- Thread-safe design
- Non-blocking UI updates

### Graphics Programming
- OpenGL rendering pipeline
- GLFW window management
- GLEW extension loading
- 3D transformations and camera controls

### Software Architecture
- Separation of concerns
- Modular design patterns
- Decoupled parsing and rendering layers

---

## 🏗️ Architecture Overview

```
┌───────────────────┐
│  G-code File      │
│  (.gcode / .txt)  │
└─────────┬─────────┘
          │
          ▼
┌───────────────────────┐
│  GCodeInterpreter    │◄─── Worker Thread
│  (Parsing Layer)     │     (std::async)
└─────────┬─────────────┘
          │
          │ std::vector<GCommand>
          ▼
┌────────────────────────┐
│  OpenGLVisualizer     │◄─── Main Thread
│  (Rendering Layer)    │     (OpenGL Context)
└────────────────────────┘
```

**Data Flow:**
1. User selects G-code file
2. Parser runs asynchronously in background thread
3. Commands are converted to structured `GCommand` objects
4. Visualizer renders toolpath in real-time
5. User controls playback and camera

---

## 📁 Project Structure

```
3D-CNC-GCode-Visualizer/
│
├── include/
│   ├── GCodeInterpreter.h    # G-code parsing interface
│   ├── OpenGLVisualizer.h    # Rendering engine
│   └── GLWindow.h            # RAII window wrapper
│
├── src/
│   ├── GCodeInterpreter.cpp  # Parser implementation
│   ├── OpenGLVisualizer.cpp  # OpenGL rendering
│   └── main.cpp              # Application entry point
│
├── gcode_files/
│   ├── cube.gcode            # Sample: 3D cube
│   ├── circle.gcode          # Sample: circular path
│   └── complex.gcode         # Sample: complex toolpath
│
├── assets/
│   └── demo.png              # Demo image
│
├── README.md                 # This file
```

---

## 🚀 Getting Started

### Prerequisites

**Required:**
- C++17 compatible compiler 
- OpenGL 3.3 or higher
- [GLFW](https://www.glfw.org/) 3.3+
- [GLEW](http://glew.sourceforge.net/) 2.1+

## 🧰 Environment Setup

### 1️⃣ Install MSYS2

Download and install MSYS2 from:  
👉 https://www.msys2.org/

After installation, **open the MSYS2 UCRT64 terminal**  
(do NOT use MinGW64 or MSYS).

---

### 2️⃣ Install Dependencies

Run the following command in the **UCRT64 terminal**:

```bash
pacman -S --needed \
    mingw-w64-ucrt-x86_64-gcc \
    mingw-w64-ucrt-x86_64-glfw \
    mingw-w64-ucrt-x86_64-glew \
    mingw-w64-ucrt-x86_64-opengl


## 🎯 Quick Start Commands

**Full build and run sequence:**

```bash
# 1. Clone repository
git clone https://github.com/praveshmishra-coder/3D-CNC-Gcode-Interpreter-Visualizer
cd 3D-CNC-Gcode-Interpreter-Visualizer

# 2. Build
g++ -std=c++17 \
    src/main.cpp src/GCodeInterpreter.cpp src/OpenGLVisualizer.cpp \
    -Iinclude \
    -lglfw3 -lglew32 -lopengl32 \
    -o cnc_3d

# 3. Run
./cnc_3d.exe
```

### Controls

#### Keyboard
| Key         | Action                    |
|-------------|---------------------------|
| `SPACE`     | Play / Pause animation    |
| `→` (RIGHT) | Step forward one command  |
| `←` (LEFT)  | Step backward one command |
| `R`         | Restart from beginning    |
| `ESC`       | Exit application          |

#### Mouse
| Action              | Effect            |
|---------------------|-------------------|
| Left Click + Drag   | Rotate camera     |
| Right Click + Drag  | Pan camera        |
| Scroll Wheel        | Zoom in/out       |

---

## 📄 G-Code Support

### Supported Commands

| Command | Description              | Example          |
|---------|--------------------------|------------------|
| `G0`    | Rapid positioning        | `G0 X10 Y20 Z5`  |
| `G1`    | Linear interpolation     | `G1 X30 Y40 F500`|
| `G2`    | Clockwise arc            | `G2 X20 Y20 I10` |
| `G3`    | Counter-clockwise arc    | `G3 X0 Y0 J-10`  |
| `G21`   | Set units to millimeters | `G21`            |
| `G90`   | Absolute positioning     | `G90`            |
| `G91`   | Relative positioning     | `G91`            |

### Sample G-Code

```gcode
; Draw a 30mm cube
G21        ; Set units to mm
G90        ; Absolute positioning
G0 X0 Y0 Z0    ; Move to origin
G1 X30 Y0 Z0   ; Draw first edge
G1 X30 Y30 Z0  ; Second edge
G1 X0 Y30 Z0   ; Third edge
G1 X0 Y0 Z0    ; Close square
G1 X0 Y0 Z30   ; Move up
G1 X30 Y0 Z30  ; Top square
G1 X30 Y30 Z30
G1 X0 Y30 Z30
G1 X0 Y0 Z30
```

---

## 🧩 Module Details

### GCodeInterpreter

**Responsibilities:**
- File I/O and parsing
- Token extraction
- Command validation
- State tracking (position, units, mode)

**Key Methods:**
```cpp
std::vector<GCommand> parseFile(const std::string& filename);
GCommand parseLine(const std::string& line);
```

**Thread Safety:** Stateless parser design ensures thread-safe operation.

### OpenGLVisualizer

**Responsibilities:**
- OpenGL context management
- 3D rendering pipeline
- Camera transformations
- Animation playback

**Key Methods:**
```cpp
void render(const std::vector<GCommand>& commands);
void update(float deltaTime);
void handleInput();
```

### GLWindow (RAII Wrapper)

**Responsibilities:**
- GLFW initialization and cleanup
- OpenGL context creation
- Resource lifetime management

**Benefits:**
- Exception-safe resource handling
- Automatic cleanup on scope exit
- No manual memory management

---

## ⚙️ Configuration

### Rendering Settings

Edit `src/OpenGLVisualizer.cpp`:

```cpp
// Line width
glLineWidth(2.5f);

// Background color
glClearColor(0.1f, 0.1f, 0.15f, 1.0f);

// Camera settings
float cameraDistance = 100.0f;
float cameraPitch = 30.0f;
float cameraYaw = 45.0f;
```

### Parsing Options

Edit `src/GCodeInterpreter.cpp`:

```cpp
// Skip comments
bool skipComments = true;

// Strict mode (error on unknown commands)
bool strictMode = false;
```

---

## ❗ Error Handling

The application handles errors gracefully:

- **File Not Found** - Clear error message with path
- **Invalid G-Code** - Warning logged, parsing continues
- **OpenGL Errors** - Caught and reported
- **Resource Leaks** - RAII ensures cleanup

**Example Output:**
```
[WARNING] Unknown command: G99 at line 42
[ERROR] Failed to open file: toolpath.gcode
[INFO] Successfully parsed 1523 commands
```

---


## 🔮 Roadmap

### Planned Features

- [ ] **Extended G/M Code Support** (G17-G19, M3-M5)
- [ ] **Live Streaming Parser** (real-time CNC monitoring)
- [ ] **Tool Visualization** (diameter, shape)
- [ ] **Collision Detection** (workpiece boundaries)
- [ ] **Export Options** (STL, OBJ mesh generation)

### Contributions Welcome!

## 📚 Resources

### Learning Materials
- [G-Code Reference](https://www.reprap.org/wiki/G-code)
- [OpenGL Tutorial](https://learnopengl.com/)
- [Modern C++ Best Practices](https://isocpp.org/wiki/faq/cpp11)

---

## 👤 Author

**Pravesh Mishra**

- 💼 Specializations: C++ | Systems Programming | Graphics | Backend Development
- 🔗 GitHub: [@praveshmishra-coder](https://github.com/praveshmishra-coder)
---


<div align="center">

**[Documentation](docs/) • [Report Bug](issues) • [Request Feature](issues)**

Made with ❤️ and C++

</div>