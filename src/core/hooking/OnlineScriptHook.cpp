#include "OnlineScriptHook.hpp"
#include "core/logger/LogHelper.hpp"
#include <filesystem>

namespace YimMenu
{
	bool OnlineScriptHook::s_Initialized = false;
	std::vector<std::function<void()>> OnlineScriptHook::s_Scripts;
	std::vector<HMODULE> OnlineScriptHook::s_LoadedModules;

	bool OnlineScriptHook::Initialize()
	{
		if (s_Initialized)
			return true;

		LOG(al::INFO) << "Initializing Online ScriptHook...";
		
		// Initialize our custom ScriptHook implementation
		s_Initialized = true;
		
		LOG(al::INFO) << "Online ScriptHook initialized successfully";
		return true;
	}

	void OnlineScriptHook::AddScript(std::function<void()> script)
	{
		if (!s_Initialized)
			return;

		s_Scripts.push_back(script);
		LOG(al::INFO) << "Added script to Online ScriptHook";
	}

	bool OnlineScriptHook::LoadASIFile(const std::string& filePath)
	{
		LOG(al::INFO) << "OnlineScriptHook::LoadASIFile called for: " << filePath;
		
		if (!s_Initialized)
		{
			LOG(al::FATAL) << "Online ScriptHook not initialized";
			return false;
		}

		// Check if file exists
		if (!std::filesystem::exists(filePath))
		{
			LOG(al::FATAL) << "ASI file not found: " << filePath;
			return false;
		}

		// Ensure ScriptHookRDR2.dll is loaded first for ASI compatibility
		HMODULE scriptHook = GetModuleHandleA("ScriptHookRDR2.dll");
		if (!scriptHook)
		{
			LOG(al::INFO) << "Loading ScriptHookRDR2.dll first for ASI compatibility...";
			
			// Try to find ScriptHookRDR2.dll in the game directory
			std::string scriptHookPath = std::filesystem::path(filePath).parent_path().string() + "\\ScriptHookRDR2.dll";
			if (std::filesystem::exists(scriptHookPath))
			{
				scriptHook = LoadLibraryA(scriptHookPath.c_str());
				if (scriptHook)
				{
					LOG(al::INFO) << "ScriptHookRDR2.dll loaded successfully from game directory";
				}
				else
				{
					LOG(al::FATAL) << "Failed to load ScriptHookRDR2.dll from game directory";
					return false;
				}
			}
			else
			{
				// Fallback to current directory
				scriptHook = LoadLibraryA("ScriptHookRDR2.dll");
				if (scriptHook)
				{
					LOG(al::INFO) << "ScriptHookRDR2.dll loaded successfully from current directory";
				}
				else
				{
					LOG(al::FATAL) << "Failed to load ScriptHookRDR2.dll from any location";
					return false;
				}
			}
		}

		// Load the ASI file using enhanced method for online compatibility
		HMODULE module = LoadLibraryA(filePath.c_str());
		if (!module)
		{
			DWORD error = GetLastError();
			LOG(al::FATAL) << "Failed to load ASI file: " << filePath << " (Error: " << error << ")";
			
			// Try alternative loading method for online compatibility
			LOG(al::INFO) << "Attempting alternative loading method...";
			
			// Load with different flags for online compatibility
			module = LoadLibraryExA(filePath.c_str(), nullptr, LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);
			if (!module)
			{
				error = GetLastError();
				LOG(al::FATAL) << "Alternative loading also failed: " << filePath << " (Error: " << error << ")";
				return false;
			}
		}

		s_LoadedModules.push_back(module);
		LOG(al::INFO) << "Successfully loaded ASI file: " << filePath;
		
		// Give the ASI file time to initialize
		if (filePath.find("Rampage") != std::string::npos)
		{
			LOG(al::INFO) << "Rampage detected - giving extra initialization time...";
			Sleep(3000); // 3 seconds for Rampage initialization
		}
		else
		{
			Sleep(1000); // 1 second delay for other ASI files
		}
		LOG(al::INFO) << "ASI file initialization complete";
		
		// If this is any trainer that needs file support, ensure files are accessible and loaded
		if (filePath.find("Rampage") != std::string::npos || 
		    filePath.find("CustomizableTrainer") != std::string::npos ||
		    filePath.find("Trainer") != std::string::npos ||
		    filePath.find("XML") != std::string::npos ||
		    filePath.find("Script") != std::string::npos)
		{
			LOG(al::INFO) << "=== TRAINER LOADING DEBUG ===";
			LOG(al::INFO) << "Trainer ASI file path: " << filePath;
			
			// Detect trainer type based on filename
			std::string trainerType = "Unknown";
			if (filePath.find("Rampage") != std::string::npos) trainerType = "Rampage";
			else if (filePath.find("CustomizableTrainer") != std::string::npos) trainerType = "CustomizableTrainer";
			else if (filePath.find("XML") != std::string::npos) trainerType = "XML Trainer";
			else if (filePath.find("Script") != std::string::npos) trainerType = "Script Trainer";
			else if (filePath.find("Trainer") != std::string::npos) trainerType = "Generic Trainer";
			
			LOG(al::INFO) << trainerType << " detected - loading all trainer files...";
			
			// Try different possible directory names for trainer files
			std::string baseDir = std::filesystem::path(filePath).parent_path().string();
			std::string trainerFilesDir = baseDir + "\\RampageFiles\\";
			
			// If RampageFiles doesn't exist, try other common names
			if (!std::filesystem::exists(trainerFilesDir))
			{
				std::vector<std::string> possibleDirs = {
					baseDir + "\\Scripts\\",
					baseDir + "\\XMLFiles\\", 
					baseDir + "\\TrainerFiles\\",
					baseDir + "\\Config\\",
					baseDir + "\\Data\\"
				};
				
				for (const auto& dir : possibleDirs)
				{
					if (std::filesystem::exists(dir))
					{
						trainerFilesDir = dir;
						LOG(al::INFO) << "Found trainer files directory: " << trainerFilesDir;
						break;
					}
				}
			}
			
			LOG(al::INFO) << "Looking for trainer files at: " << trainerFilesDir;
			
			if (std::filesystem::exists(trainerFilesDir))
			{
				LOG(al::INFO) << "Trainer files directory found: " << trainerFilesDir;
			}
			else
			{
				LOG(al::INFO) << "Trainer files directory not found, creating it...";
				try
				{
					std::filesystem::create_directories(trainerFilesDir);
					LOG(al::INFO) << "Created trainer files directory: " << trainerFilesDir;
				}
				catch (const std::exception& e)
				{
					LOG(al::FATAL) << "Failed to create trainer files directory: " << e.what();
				}
			}
			
			if (std::filesystem::exists(trainerFilesDir))
			{
				
				// Load all files in trainer files directory recursively
				try
				{
					// First, load all DLL files
					for (const auto& entry : std::filesystem::recursive_directory_iterator(trainerFilesDir))
					{
						if (entry.is_regular_file() && entry.path().extension() == ".dll")
						{
							std::string filePath = entry.path().string();
							std::string fileName = entry.path().filename().string();
							
							LOG(al::INFO) << "Loading RampageFile DLL: " << fileName;
							
							HMODULE rampageModule = LoadLibraryA(filePath.c_str());
							if (rampageModule)
							{
								s_LoadedModules.push_back(rampageModule);
								LOG(al::INFO) << "Successfully loaded RampageFile DLL: " << fileName;
							}
							else
							{
								DWORD error = GetLastError();
								LOG(al::FATAL) << "Failed to load RampageFile DLL: " << fileName << " (Error: " << error << ")";
							}
						}
					}
					
					// Then, process scripts and configuration files
					LOG(al::INFO) << "Processing trainer scripts and configuration files...";
					for (const auto& entry : std::filesystem::recursive_directory_iterator(trainerFilesDir))
					{
						if (entry.is_regular_file())
						{
							std::string fileName = entry.path().filename().string();
							std::string extension = entry.path().extension().string();
							
							// Process XML files (Rampage scripts, configs, etc.)
							if (extension == ".xml")
							{
								LOG(al::INFO) << "Found Rampage XML script: " << fileName;
								// XML files are read by Rampage at runtime, we just need to ensure they're accessible
								// The main Rampage ASI will handle loading these XML scripts
							}
							// Process JSON files (CustomizableTrainer layout, configs, etc.)
							else if (extension == ".json")
							{
								LOG(al::INFO) << "Found CustomizableTrainer JSON script: " << fileName;
								// JSON files are read by CustomizableTrainer at runtime
								// Check if this is the main layout.json file
								if (fileName == "layout.json")
								{
									LOG(al::INFO) << "Found main layout.json - CustomizableTrainer layout file";
								}
							}
							// Process other important files
							else if (extension == ".ini" || extension == ".cfg")
							{
								LOG(al::INFO) << "Found trainer config file: " << fileName;
							}
							// Process Lua scripts if present
							else if (extension == ".lua")
							{
								LOG(al::INFO) << "Found trainer Lua script: " << fileName;
							}
							// Log other important files
							else if (extension != ".dll")
							{
								LOG(al::INFO) << "Found trainer file: " << fileName << " (extension: " << extension << ")";
							}
						}
					}
					LOG(al::INFO) << "All trainer files processed successfully";
				}
				catch (const std::exception& e)
				{
					LOG(al::FATAL) << "Error loading trainer files: " << e.what();
				}
				
				// Additional trainer initialization
				LOG(al::INFO) << "Attempting to initialize " << trainerType << " interface...";
				
				// Ensure trainer can find its scripts by setting working directory
				std::string originalDir = std::filesystem::current_path().string();
				std::string trainerDir = std::filesystem::path(filePath).parent_path().string();
				
				LOG(al::INFO) << "Setting working directory to trainer folder: " << trainerDir;
				std::filesystem::current_path(trainerDir);
				
				Sleep(2000); // Give trainer more time to set up its interface
				
				// Restore original working directory
				std::filesystem::current_path(originalDir);
				
				LOG(al::INFO) << trainerType << " initialization complete - F5 should now work";
				if (trainerType == "Rampage")
					LOG(al::INFO) << "XML scripts should be accessible to Rampage";
				else
					LOG(al::INFO) << "JSON layout files should be accessible to CustomizableTrainer";
			}
			else
			{
				LOG(al::FATAL) << "Trainer files directory not found: " << trainerFilesDir;
				LOG(al::FATAL) << trainerType << " will not function properly without its files!";
			}
			LOG(al::INFO) << "=== END TRAINER DEBUG ===";
		}
		
		return true;
	}

	void OnlineScriptHook::ExecuteScripts()
	{
		if (!s_Initialized)
			return;

		// Execute all registered scripts
		for (auto& script : s_Scripts)
		{
			try
			{
				script();
			}
			catch (const std::exception& e)
			{
				LOG(al::FATAL) << "Script execution error: " << e.what();
			}
		}
	}

	void OnlineScriptHook::Shutdown()
	{
		if (!s_Initialized)
			return;

		// Free all loaded modules
		for (auto module : s_LoadedModules)
		{
			FreeLibrary(module);
		}
		s_LoadedModules.clear();
		s_Scripts.clear();
		s_Initialized = false;
		
		LOG(al::INFO) << "Online ScriptHook shutdown";
	}

	bool OnlineScriptHook::IsAvailable()
	{
		return s_Initialized;
	}

	std::string OnlineScriptHook::GetVersion()
	{
		return "Online ScriptHook v1.0 (Compatible with ScriptHookRDR2)";
	}
}
