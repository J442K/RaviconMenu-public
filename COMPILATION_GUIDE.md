# 🏴‍☠️ How to Compile ModernRDR2UI DLL - Complete Guide

## 📋 Prerequisites (Download These First)

### 1. Visual Studio Community 2022 (FREE)
- **Download**: https://visualstudio.microsoft.com/vs/community/
- **During installation, select**:
  - ✅ Desktop development with C++
  - ✅ MSVC v143 compiler toolset
  - ✅ Windows 11 SDK (latest)
  - ✅ CMake tools for Visual Studio

### 2. Git (if not already installed)
- **Download**: https://git-scm.com/download/win

## 🔧 Step-by-Step Compilation

### Step 1: Extract Your Files
1. **Extract** `ModernRDR2UI-Complete.zip` from your desktop
2. **Note the folder location** (e.g., `C:\Users\YourName\ModernRDR2UI`)

### Step 2: Open in Visual Studio
1. **Launch Visual Studio 2022**
2. **Click "Open a local folder"**
3. **Navigate to** your extracted folder
4. **Select the folder** containing `CMakeLists.txt`
5. **Click "Select Folder"**

### Step 3: Configure Build
1. **Wait** for Visual Studio to detect CMakeLists.txt
2. **In the toolbar**, you'll see configuration dropdown
3. **Select**: `x64-Debug` or `x64-Release` (Release recommended)
4. **Wait** for CMake to configure (may take 2-3 minutes)

### Step 4: Build the DLL
1. **Go to**: Build → Build All (or press Ctrl+Shift+B)
2. **Wait** for compilation (2-5 minutes)
3. **Check Output window** for any errors

### Step 5: Find Your DLL
**Your compiled DLL will be at**:
```
[YourProjectFolder]\out\build\x64-Release\Terminus.dll
```
OR
```
[YourProjectFolder]\out\build\x64-Debug\Terminus.dll
```

## 🎯 Alternative: Command Line Method

### Open Developer Command Prompt
1. **Search** "Developer Command Prompt" in Start Menu
2. **Run as Administrator**

### Navigate and Build
```cmd
cd "C:\Users\YourName\ModernRDR2UI"
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

**DLL Location**: `build\Release\Terminus.dll`

## ✅ What You'll Get

- **Terminus.dll** - Your modernized HorseMenu with RDR2 UI
- **ASI Loader** - Load Rampage online without Script Hook
- **Modern Western Theme** - Gold/brown RDR2 styling

## 🚀 How to Use

1. **Copy** `Terminus.dll` to your injector folder
2. **Inject** into `rdr2.exe` using any injector
3. **Press INSERT** to open the modern RDR2 menu
4. **Go to "ASI Loader"** to load Rampage online!

## ⚠️ Troubleshooting

### If Build Fails:
- **Check** you have all prerequisites installed
- **Try** x64-Debug instead of Release
- **Restart** Visual Studio and try again
- **Check** Output window for specific error messages

### If DLL Not Found:
- **Look** in `out\build\x64-Release\` folder
- **Check** both Debug and Release folders
- **Verify** build completed successfully

## 🎉 Success!
Once you have `Terminus.dll`, you're ready to load Rampage online with the modern RDR2 UI!

**Total time: 10-15 minutes including downloads**

