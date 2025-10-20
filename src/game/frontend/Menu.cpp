#include "Menu.hpp"

#include "core/commands/Commands.hpp"
#include "core/frontend/manager/UIManager.hpp"
#include "core/renderer/Renderer.hpp"
#include "game/backend/FiberPool.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/frontend/fonts/Fonts.hpp"
#include "game/pointers/Pointers.hpp"
#include "submenus/Debug.hpp"
#include "submenus/Network.hpp"
#include "submenus/Players.hpp"
#include "submenus/Self.hpp"
#include "submenus/Settings.hpp"
#include "submenus/Teleport.hpp"
#include "submenus/World.hpp"
#include "submenus/Recovery.hpp"
#include "submenus/ASILoader.hpp"
#include "submenus/Effects.hpp"

namespace YimMenu
{
	void Menu::Init()
	{
		// Arguably the only place this file should be edited at for more menus
		UIManager::AddSubmenu(std::make_shared<Submenus::Self>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Teleport>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Network>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Players>());
		UIManager::AddSubmenu(std::make_shared<Submenus::World>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Recovery>());
		UIManager::AddSubmenu(std::make_shared<Submenus::ASILoader>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Effects>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Settings>());
		UIManager::AddSubmenu(std::make_shared<Submenus::Debug>());

		Renderer::AddRendererCallBack(
		    [&] {
			    if (!GUI::IsOpen())
				    return;

			    ImGui::PushFont(Menu::Font::g_DefaultFont);
			    
			    // RAVENHEART theme is applied in UIManager
			    
			    // Set window size and position for better aesthetics - BIGGER WINDOW
			    ImGui::SetNextWindowSize(ImVec2((*Pointers.ScreenResX / 1.5), (*Pointers.ScreenResY / 1.3)), ImGuiCond_Once);
			    ImGui::SetNextWindowPos(ImVec2((*Pointers.ScreenResX / 2) - (*Pointers.ScreenResX / 3), (*Pointers.ScreenResY / 2) - (*Pointers.ScreenResY / 2.6)), ImGuiCond_Once);
			    
			    if (ImGui::Begin("RAVENHEART", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
			    {

				    UIManager::Draw();

				    ImGui::End();
			    }

			    // No style colors to pop - RAVENHEART theme handles everything
			    ImGui::PopFont();
		    },
		    -1);
	}

	void Menu::SetupStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		
		// RDR2-inspired color scheme - Western/Autumn theme
		style.Colors[ImGuiCol_Text]                  = ImVec4(0.85f, 0.65f, 0.13f, 1.00f); // Gold text
		style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.40f, 0.20f, 1.00f); // Muted gold
		style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.08f, 0.06f, 0.04f, 0.95f); // Dark brown/black
		style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.10f, 0.08f, 0.06f, 0.90f); // Slightly lighter brown
		style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.12f, 0.10f, 0.08f, 0.95f); // Popup background
		style.Colors[ImGuiCol_Border]                = ImVec4(0.55f, 0.27f, 0.07f, 0.80f); // Saddle brown border
		style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.15f, 0.12f, 0.08f, 1.00f); // Frame background
		style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.20f, 0.16f, 0.10f, 1.00f); // Hovered frame
		style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.25f, 0.20f, 0.12f, 1.00f); // Active frame
		style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.06f, 0.05f, 0.03f, 1.00f); // Title background
		style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.06f, 0.04f, 1.00f); // Active title
		style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.04f, 0.03f, 0.02f, 0.75f); // Collapsed title
		style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.10f, 0.08f, 0.06f, 1.00f); // Menu bar
		style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.04f, 0.02f, 0.60f); // Scrollbar background
		style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.55f, 0.27f, 0.07f, 1.00f); // Scrollbar grab
		style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.65f, 0.35f, 0.10f, 1.00f); // Scrollbar hover
		style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.75f, 0.45f, 0.15f, 1.00f); // Scrollbar active
		style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.85f, 0.65f, 0.13f, 1.00f); // Gold checkmark
		style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.55f, 0.27f, 0.07f, 1.00f); // Slider grab
		style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.65f, 0.35f, 0.10f, 1.00f); // Slider active
		style.Colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.15f, 0.08f, 1.00f); // Button background
		style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.30f, 0.22f, 0.12f, 1.00f); // Button hover
		style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.40f, 0.30f, 0.16f, 1.00f); // Button active
		style.Colors[ImGuiCol_Header]                = ImVec4(0.18f, 0.14f, 0.08f, 1.00f); // Header background
		style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.25f, 0.20f, 0.12f, 1.00f); // Header hover
		style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.35f, 0.28f, 0.16f, 1.00f); // Header active
		style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
		style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.65f, 0.35f, 0.10f, 1.00f); // Separator hover
		style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.75f, 0.45f, 0.15f, 1.00f); // Separator active
		style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.25f, 0.20f, 0.12f, 0.67f); // Resize grip hover
		style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.35f, 0.28f, 0.16f, 0.95f); // Resize grip active
		style.Colors[ImGuiCol_Tab]                   = ImVec4(0.12f, 0.10f, 0.06f, 0.83f); // Tab background
		style.Colors[ImGuiCol_TabHovered]            = ImVec4(0.20f, 0.16f, 0.10f, 0.83f); // Tab hover
		style.Colors[ImGuiCol_TabActive]             = ImVec4(0.25f, 0.20f, 0.12f, 1.00f); // Tab active
		style.Colors[ImGuiCol_TabUnfocused]          = ImVec4(0.08f, 0.06f, 0.04f, 1.00f); // Tab unfocused
		style.Colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.15f, 0.12f, 0.08f, 1.00f); // Tab unfocused active
		style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.85f, 0.65f, 0.13f, 1.00f); // Plot lines
		style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.75f, 0.20f, 1.00f); // Plot lines hover
		style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.85f, 0.65f, 0.13f, 1.00f); // Plot histogram
		style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.75f, 0.20f, 1.00f); // Plot histogram hover
		style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.55f, 0.27f, 0.07f, 0.35f); // Text selection
		style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(0.85f, 0.65f, 0.13f, 1.00f); // Drag drop target
		style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.85f, 0.65f, 0.13f, 1.00f); // Navigation highlight
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		
		// Enhanced rounding for a more modern look
		style.WindowRounding = 8.0f;
		style.ChildRounding = 6.0f;
		style.FrameRounding = 4.0f;
		style.GrabRounding = 4.0f;
		style.PopupRounding = 6.0f;
		style.ScrollbarRounding = 4.0f;
		style.TabRounding = 4.0f;
		
		// Enhanced spacing and padding
		style.WindowPadding = ImVec2(12, 12);
		style.FramePadding = ImVec2(8, 6);
		style.ItemSpacing = ImVec2(8, 6);
		style.ItemInnerSpacing = ImVec2(6, 4);
		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 16.0f;
		style.GrabMinSize = 12.0f;
		
		// Border settings
		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;
	}

	void Menu::SetupFonts()
	{
		auto& IO = ImGui::GetIO();
		ImFontConfig FontCfg{};
		FontCfg.FontDataOwnedByAtlas = false;

		Menu::Font::g_DefaultFont = IO.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::MainFont), sizeof(Fonts::MainFont), Menu::Font::g_DefaultFontSize, &FontCfg);
		Menu::Font::g_OptionsFont = IO.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::MainFont), sizeof(Fonts::MainFont), Menu::Font::g_OptionsFontSize, &FontCfg);
		Menu::Font::g_ChildTitleFont = IO.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::MainFont), sizeof(Fonts::MainFont), Menu::Font::g_ChildTitleFontSize, &FontCfg);
		Menu::Font::g_ChatFont = IO.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::MainFont), sizeof(Fonts::MainFont), Menu::Font::g_ChatFontSize, &FontCfg);
		Menu::Font::g_OverlayFont = IO.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Fonts::MainFont), sizeof(Fonts::MainFont), Menu::Font::g_OverlayFontSize, &FontCfg);
		UIManager::SetOptionsFont(Menu::Font::g_OptionsFont);
	}
}
