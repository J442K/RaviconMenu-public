# 🏴‍☠️ Modern RDR2 UI + ASI Loader - Build Instructions

## Quick Build Guide

### Prerequisites
1. **Visual Studio 2022 Community** (Free) - Download from Microsoft
2. **CMake** (Already installed via winget)
3. **Git** (For cloning)

### Build Steps

#### Method 1: Visual Studio (Recommended)
1. Open Visual Studio 2022
2. Select "Open a local folder"
3. Navigate to this project folder
4. Visual Studio will automatically detect CMakeLists.txt
5. Select "x64-Debug" or "x64-Release" configuration
6. Click "Build" → "Build All"
7. Find the compiled DLL in: `build/Debug/Terminus.dll` or `build/Release/Terminus.dll`

#### Method 2: Command Line
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

### Output
- **Compiled DLL**: `build/Release/Terminus.dll`
- **Copy this DLL** to your RDR2 directory or injector folder

### Usage
1. **Inject** `Terminus.dll` into `rdr2.exe` using any injector
2. **Press INSERT** to open the modern RDR2-themed menu
3. **Navigate to "ASI Loader"** to load Rampage online!

### Features
- ✅ Modern RDR2 Western-themed UI
- ✅ ASI Script Loader with Script Hook bypass
- ✅ Load Rampage trainer while online
- ✅ Real-time script management
- ✅ Professional gold/brown color scheme

### ⚠️ Warning
Use responsibly - may result in bans when using trainers online.
