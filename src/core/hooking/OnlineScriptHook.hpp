#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <functional>
#include "ScriptHookSDK/main.h"

namespace YimMenu
{
	class OnlineScriptHook
	{
	private:
		static bool s_Initialized;
		static std::vector<std::function<void()>> s_Scripts;
		static std::vector<HMODULE> s_LoadedModules;

	public:
		// Initialize the online-compatible ScriptHook
		static bool Initialize();
		
		// Add a script to be executed
		static void AddScript(std::function<void()> script);
		
		// Load an ASI file with online compatibility
		static bool LoadASIFile(const std::string& filePath);
		
		// Execute all registered scripts
		static void ExecuteScripts();
		
		// Cleanup
		static void Shutdown();
		
		// Check if ScriptHook is available (for compatibility)
		static bool IsAvailable();
		
		// Get ScriptHook version (for compatibility)
		static std::string GetVersion();
	};
}

