# 🏴‍☠️ Terminus - Red Dead Redemption 2 Mod Menu

A modern, sleek mod menu for Red Dead Redemption 2 and Red Dead Online with RDR2-themed UI and ASI script loading capabilities.

## ✨ Features

- **Modern RDR2-Themed UI**: Beautiful Western/Autumn color scheme with gold accents and brown tones
- **ASI Script Loader**: Load offline trainers like Rampage while playing Red Dead Online
- **Script Hook RDR2 Bypass**: No need for Script Hook RDR2 - direct ASI loading
- **Enhanced Visual Design**: Rounded corners, improved spacing, and immersive styling
- **All Original Features**: Self, Teleport, Network, Players, World, Recovery, Settings, Debug

## 🎯 Loading Rampage Online

### Step 1: Setup
1. Download and extract Terminus to your desired location
2. Download Rampage trainer (any version)
3. Place Rampage's `.asi` file in your RDR2 game directory under `scripts/` folder
   - Example: `C:\Program Files\Rockstar Games\Red Dead Redemption 2\scripts\Rampage.asi`

### Step 2: Injection
1. Use any popular injector (Xenos, Extreme Injector, etc.)
2. Inject `Terminus.dll` into `rdr2.exe`
3. Press **INSERT** to open the menu

### Step 3: Load Rampage
1. Navigate to **ASI Loader** in the menu
2. You'll see Rampage listed in "Available Scripts"
3. Click **Load** next to Rampage
4. Rampage will now be active in your online session!

## 🎨 UI Theme

The menu features a sophisticated RDR2-inspired design:
- **Colors**: Dark brown/black backgrounds with gold text accents
- **Style**: Western/Autumn theme with saddle brown borders
- **Typography**: Enhanced spacing and rounded corners
- **Icons**: Western-themed emojis and symbols

## ⚠️ Important Warnings

- **Use at your own risk**: Using trainers online may result in bans
- **Backup saves**: Always backup your game saves before using
- **Moderate usage**: Avoid obvious cheating behaviors
- **Single player recommended**: For safest experience, use in single player

## 🛠️ Technical Details

### ASI Loader Features
- **Direct Loading**: Bypasses Script Hook RDR2 requirements
- **Process Injection**: Loads ASI files directly into the game process
- **Real-time Management**: Load/unload scripts without restarting
- **Error Handling**: Detailed error messages and status notifications
- **Script Persistence**: Scripts remain loaded until manually unloaded

### Supported File Types
- `.asi` files (primary support)
- Compatible with most Script Hook RDR2 mods
- Works with offline trainers like Rampage

## 📁 File Structure

```
HorseMenu/
├── src/
│   ├── game/frontend/submenus/
│   │   ├── ASILoader.hpp
│   │   └── ASILoader.cpp
│   └── game/frontend/
│       └── Menu.cpp (updated with RDR2 theme)
└── README.md
```

## 🎮 Controls

- **INSERT**: Open/Close menu
- **Mouse**: Navigate menu (cursor appears when menu is open)
- **ESC**: Close menu (if supported by injector)

## 🔧 Building

The project uses CMake for building. Ensure you have:
- Visual Studio 2019 or later
- CMake 3.20+
- All dependencies properly configured

## 📝 Changelog

### Version 2.0 - Modern RDR2 Theme
- ✅ Complete UI redesign with RDR2 Western theme
- ✅ Added ASI Script Loader with Script Hook bypass
- ✅ Enhanced visual styling with rounded corners
- ✅ Gold/brown color scheme throughout
- ✅ Improved spacing and typography
- ✅ Added Rampage online loading support

## 🤝 Contributing

This project is for educational purposes. Contributions are welcome for:
- UI improvements
- Additional ASI compatibility
- Bug fixes
- Feature enhancements

## 📄 License

This project is for educational purposes only. Use responsibly and at your own risk.

---

**Happy modding! 🤠**