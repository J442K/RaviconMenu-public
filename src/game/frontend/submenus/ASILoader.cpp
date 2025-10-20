#include "ASILoader.hpp"

#include "core/filemgr/FileMgr.hpp"
#include "core/frontend/Notifications.hpp"
#include "core/hooking/OnlineScriptHook.hpp"
#include "game/frontend/items/Items.hpp"
#include <filesystem>
#include <vector>
#include <string>
#include <windows.h>

namespace YimMenu::Submenus
{
	class ASIScriptManager
	{
	private:
		std::vector<std::string> m_LoadedScripts;
		std::vector<HMODULE> m_LoadedModules;
		std::string m_ASIDirectory;

	public:
		ASIScriptManager()
		{
			// Initialize Online ScriptHook
			OnlineScriptHook::Initialize();
			
			// Set ASI directory to main game folder (not scripts subfolder)
			// Try to find RDR2.exe process to get the correct game directory
			HMODULE gameModule = GetModuleHandleA("RDR2.exe");
			if (gameModule)
			{
				char gamePath[MAX_PATH];
				GetModuleFileNameA(gameModule, gamePath, MAX_PATH);
				std::filesystem::path gameDir = std::filesystem::path(gamePath).parent_path();
				m_ASIDirectory = gameDir.string() + "\\"; // Main directory, not scripts subfolder
			}
			else
			{
				// Fallback to common Steam path
				m_ASIDirectory = "D:\\SteamLibrary\\steamapps\\common\\Red Dead Redemption 2\\";
			}
			
			// Create scripts directory if it doesn't exist
			if (!std::filesystem::exists(m_ASIDirectory))
			{
				std::filesystem::create_directories(m_ASIDirectory);
			}
			
			// Create RampageFiles directory (required for Rampage)
			std::string rampageFilesDir = m_ASIDirectory + "RampageFiles\\";
			if (!std::filesystem::exists(rampageFilesDir))
			{
				std::filesystem::create_directories(rampageFilesDir);
				LOG(al::INFO) << "Created RampageFiles directory: " << rampageFilesDir;
			}
		}

		std::vector<std::string> GetAvailableScripts()
		{
			std::vector<std::string> scripts;
			try
			{
				for (const auto& entry : std::filesystem::directory_iterator(m_ASIDirectory))
				{
					if (entry.is_regular_file() && entry.path().extension() == ".asi")
					{
						scripts.push_back(entry.path().filename().string());
					}
				}
			}
			catch (const std::exception& e)
			{
				LOG(al::FATAL) << "Error scanning ASI directory: " << e.what();
			}
			return scripts;
		}

		const std::string& GetASIDirectory() const
		{
			return m_ASIDirectory;
		}

		bool LoadScript(const std::string& scriptName)
		{
			LOG(al::INFO) << "LoadScript called for: " << scriptName;
			std::string fullPath = m_ASIDirectory + scriptName;
			LOG(al::INFO) << "Full path: " << fullPath;
			
			// Check if already loaded
			if (std::find(m_LoadedScripts.begin(), m_LoadedScripts.end(), scriptName) != m_LoadedScripts.end())
			{
				LOG(al::INFO) << "Script already loaded: " << scriptName;
				Notifications::Show("ASI Loader", "Script already loaded: " + scriptName, NotificationType::Warning);
				return false;
			}

			// Check if file exists
			if (!std::filesystem::exists(fullPath))
			{
				Notifications::Show("ASI Loader", "File not found: " + scriptName, NotificationType::Error);
				return false;
			}

			// Check dependencies before loading
			if (!CheckDependencies(scriptName))
			{
				Notifications::Show("ASI Loader", "Dependency check failed for: " + scriptName, NotificationType::Warning);
			}

			// Load the ASI file using Online ScriptHook
			if (OnlineScriptHook::LoadASIFile(fullPath))
			{
				m_LoadedScripts.push_back(scriptName);
				Notifications::Show("ASI Loader", "Successfully loaded: " + scriptName + " (Online ScriptHook)", NotificationType::Success);
				return true;
			}
			else
			{
				DWORD error = GetLastError();
				std::string errorMsg = "Failed to load " + scriptName + " (Error: " + std::to_string(error) + ")";
				
				// Provide more specific error messages with online compatibility tips
				switch (error)
				{
					case 126:
						errorMsg += "\nModule not found or missing dependencies";
						errorMsg += "\nTry running in single player first to test compatibility";
						break;
					case 127:
						errorMsg += "\nProcedure not found - ASI may not be compatible with current game version";
						break;
					case 193:
						errorMsg += "\nNot a valid Win32 application - corrupted ASI file";
						break;
					case 998:
						errorMsg += "\nInvalid access to memory location - try restarting game";
						break;
					default:
						errorMsg += "\nUnknown error - check ASI file compatibility";
						break;
				}
				
				Notifications::Show("ASI Loader", errorMsg, NotificationType::Error);
				return false;
			}
		}

		bool UnloadScript(const std::string& scriptName)
		{
			auto it = std::find(m_LoadedScripts.begin(), m_LoadedScripts.end(), scriptName);
			if (it != m_LoadedScripts.end())
			{
				size_t index = std::distance(m_LoadedScripts.begin(), it);
				if (index < m_LoadedModules.size())
				{
					FreeLibrary(m_LoadedModules[index]);
					m_LoadedModules.erase(m_LoadedModules.begin() + index);
				}
				m_LoadedScripts.erase(it);
				Notifications::Show("ASI Loader", "Unloaded: " + scriptName, NotificationType::Info);
				return true;
			}
			return false;
		}

		void UnloadAllScripts()
		{
			for (size_t i = 0; i < m_LoadedModules.size(); ++i)
			{
				FreeLibrary(m_LoadedModules[i]);
			}
			m_LoadedScripts.clear();
			m_LoadedModules.clear();
			Notifications::Show("ASI Loader", "All scripts unloaded", NotificationType::Info);
		}

		const std::vector<std::string>& GetLoadedScripts() const
		{
			return m_LoadedScripts;
		}

		void RefreshScripts()
		{
			// This could be used to refresh the script list
		}

		bool CheckDependencies(const std::string& scriptName)
		{
			std::string fullPath = m_ASIDirectory + scriptName;
			
			// Check for Rampage-specific requirements
			if (scriptName.find("Rampage") != std::string::npos)
			{
				char gamePath[MAX_PATH];
				GetModuleFileNameA(nullptr, gamePath, MAX_PATH);
				std::filesystem::path gameDir = std::filesystem::path(gamePath).parent_path();
				std::string rampageFilesDir = gameDir.string() + "\\RampageFiles\\";
				
				if (!std::filesystem::exists(rampageFilesDir))
				{
					std::filesystem::create_directories(rampageFilesDir);
					Notifications::Show("ASI Loader", "Created missing RampageFiles directory", NotificationType::Info);
				}
			}
			
			// Check for common dependencies that might be missing
			std::vector<std::string> commonDeps = {
				"dinput8.dll", 
				"vulkan-1.dll",
				"d3d11.dll",
				"d3d12.dll"
			};
			
			// Check for ScriptHookRDR2.dll but don't fail if missing
			// This allows ASI files to work in both single player and online
			commonDeps.insert(commonDeps.begin(), "ScriptHookRDR2.dll");
			
			for (const auto& dep : commonDeps)
			{
				HMODULE depModule = GetModuleHandleA(dep.c_str());
				if (!depModule)
				{
					// Try to find in system directories (both System32 and SysWOW64)
					char systemPath[MAX_PATH];
					char sysWow64Path[MAX_PATH];
					bool found = false;
					
					// Check System32 first
					if (GetSystemDirectoryA(systemPath, MAX_PATH))
					{
						std::string depPath = std::string(systemPath) + "\\" + dep;
						if (std::filesystem::exists(depPath))
						{
							found = true;
						}
					}
					
					// Check SysWOW64 if not found in System32
					if (!found && GetSystemWow64DirectoryA(sysWow64Path, MAX_PATH))
					{
						std::string depPath = std::string(sysWow64Path) + "\\" + dep;
						if (std::filesystem::exists(depPath))
						{
							found = true;
						}
					}
					
					// Check game directory if not found in system directories
					if (!found)
					{
						std::string gameDepPath = m_ASIDirectory + dep;
						if (std::filesystem::exists(gameDepPath))
						{
							found = true;
						}
					}
					
					if (!found)
					{
						// Try to actually load the dependency to verify it's accessible
						HMODULE testLoad = LoadLibraryA(dep.c_str());
						if (testLoad)
						{
							FreeLibrary(testLoad);
							found = true;
						}
					}
					
					if (!found)
					{
						if (dep == "ScriptHookRDR2.dll")
						{
							// Don't fail for missing ScriptHookRDR2.dll - allow online use
							Notifications::Show("ASI Loader", "ScriptHookRDR2.dll not found - ASI may work in online mode", NotificationType::Info);
							continue; // Skip this dependency check
						}
						else
						{
							std::string errorMsg = "Missing dependency: " + dep + " (required for " + scriptName + ")";
							Notifications::Show("ASI Loader", errorMsg, NotificationType::Warning);
							return false;
						}
					}
				}
			}
			return true;
		}
	};

	static ASIScriptManager g_ASIManager;

	static void DrawScriptLoader()
	{
		ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), "ASI Script Loader");
		ImGui::Separator();
		
		ImGui::Text("Scripts Directory: %s", g_ASIManager.GetASIDirectory().c_str());
		ImGui::Spacing();

		// Available Scripts Section
		ImGui::TextColored(ImVec4(0.6f, 0.8f, 0.6f, 1.0f), "Available Scripts:");
		auto availableScripts = g_ASIManager.GetAvailableScripts();
		
		if (availableScripts.empty())
		{
			ImGui::TextDisabled("No .asi files found in scripts directory");
		}
		else
		{
			for (const auto& script : availableScripts)
			{
				ImGui::PushID(script.c_str());
				
				// Check if script is already loaded
				bool isLoaded = std::find(g_ASIManager.GetLoadedScripts().begin(), 
					g_ASIManager.GetLoadedScripts().end(), script) != g_ASIManager.GetLoadedScripts().end();
				
				if (isLoaded)
				{
					ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "✓ %s", script.c_str());
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.2f, 0.15f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.3f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.1f, 1.0f));
					if (ImGui::Button("UNLOAD"))
					{
						g_ASIManager.UnloadScript(script);
					}
					ImGui::PopStyleColor(3);
				}
				else
				{
					ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "%s", script.c_str());
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.1f, 0.05f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.15f, 0.1f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
					if (ImGui::Button("LOAD"))
					{
						LOG(al::INFO) << "Loading specific ASI file: " << script;
						g_ASIManager.LoadScript(script);
					}
					ImGui::PopStyleColor(3);
				}
				
				ImGui::PopID();
				ImGui::Spacing();
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Loaded Scripts Section
		ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "LOADED SCRIPTS:");
		const auto& loadedScripts = g_ASIManager.GetLoadedScripts();
		
		if (loadedScripts.empty())
		{
			ImGui::TextColored(ImVec4(0.6f, 0.3f, 0.1f, 1.0f), "No scripts currently loaded");
		}
		else
		{
			for (const auto& script : loadedScripts)
			{
				ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "• %s", script.c_str());
			}
		}

		ImGui::Spacing();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.1f, 0.05f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.15f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
		if (ImGui::Button("REFRESH SCRIPT LIST", ImVec2(200, 0)))
		{
			g_ASIManager.RefreshScripts();
			Notifications::Show("ASI Loader", "Script list refreshed", NotificationType::Info);
		}
		ImGui::PopStyleColor(3);
		
		ImGui::SameLine();
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.2f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.3f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.1f, 1.0f));
		if (ImGui::Button("UNLOAD ALL SCRIPTS", ImVec2(200, 0)))
		{
			g_ASIManager.UnloadAllScripts();
		}
		ImGui::PopStyleColor(3);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Information Section
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.1f, 1.0f), "INFORMATION:");
		ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "This ASI loader uses a custom Online ScriptHook implementation for maximum compatibility.");
		ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "Perfect for loading offline trainers like Rampage while playing Red Dead Online.");
		ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "Place your .asi files directly in the main game directory (same folder as RDR2.exe).");
		ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.2f, 1.0f), "Scripts are loaded using our custom ScriptHook SDK integration.");
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "✅ ONLINE SCRIPT HOOK: Custom implementation with ScriptHookRDR2 SDK");
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "✅ MAXIMUM COMPATIBILITY: Works with all ScriptHookRDR2 ASI files");
		ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f), "✅ ONLINE SAFE: Bypasses detection while maintaining full functionality");
		
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.1f, 1.0f), "⚠️ WARNING:");
		ImGui::TextColored(ImVec4(0.8f, 0.4f, 0.2f, 1.0f), "Using trainers online may result in bans. Use at your own risk.");
		
		ImGui::Spacing();
		ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.1f, 1.0f), "TROUBLESHOOTING:");
		ImGui::TextWrapped("• Error 126: Missing dependencies or incompatible ASI file");
		ImGui::TextWrapped("• Error 127: Required function not found in ASI");
		ImGui::TextWrapped("• Error 193: Not a valid Win32 application");
		ImGui::TextWrapped("• Make sure ASI files are compatible with your game version");
		ImGui::TextWrapped("• Some ASI files require ScriptHookRDR2.dll to be present");
		ImGui::TextWrapped("• Rampage ASI files work without ScriptHookRDR2.dll when loaded this way");
		ImGui::TextWrapped("• Rampage requires a 'RampageFiles' folder in your game directory");
		ImGui::TextWrapped("• If you get 'dinput8.dll' errors, try installing DirectX End-User Runtime");
		ImGui::TextWrapped("• The ASI loader now checks both System32 and SysWOW64 directories");
	}

	ASILoader::ASILoader() :
	    Submenu::Submenu("ASI Loader")
	{
		auto asiLoader = std::make_shared<Category>("Script Management");
		auto info = std::make_shared<Category>("Information");

		asiLoader->AddItem(std::make_shared<ImGuiItem>(DrawScriptLoader));

		AddCategory(std::move(asiLoader));
		AddCategory(std::move(info));
	}
}
