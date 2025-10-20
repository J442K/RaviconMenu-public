#include "UIManager.hpp"
#include "../theme/RavenheartTheme.hpp"

namespace YimMenu
{
	void UIManager::AddSubmenuImpl(const std::shared_ptr<Submenu>&& submenu)
	{
		LOG(INFO) << "Adding submenu: " << submenu->m_Name.data();
		if (!m_ActiveSubmenu)
		{
			m_ActiveSubmenu = submenu;
			LOG(INFO) << "Set as active submenu: " << submenu->m_Name.data();
		}

		m_Submenus.push_back(std::move(submenu));
		LOG(INFO) << "Total submenus now: " << m_Submenus.size();
	}

	void UIManager::SetActiveSubmenuImpl(const std::shared_ptr<Submenu> Submenu)
	{
		m_ActiveSubmenu = Submenu;
	}

	void UIManager::DrawImpl()
	{
		// Apply Ravenheart theme
		RavenheartTheme::ApplyTheme();
		
		// Update animation for light strips
		RavenheartTheme::UpdateAnimation();
		
		// RAVENHEART title is now integrated into the vertical banner
		
		// Original functionality with CHERNX-style layout
		DrawOriginalLayout();
	}
	
	void UIManager::DrawGlowingText(const char* text, ImVec2 pos, ImVec4 centerColor, ImVec4 glowColor)
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		
		// Draw multiple glow layers for outline effect
		for (int i = 3; i >= 1; i--)
		{
			ImVec4 glow = ImVec4(glowColor.x, glowColor.y, glowColor.z, glowColor.w * (0.3f / i));
			ImU32 glowU32 = ImGui::ColorConvertFloat4ToU32(glow);
			
			// Draw glow in all directions
			drawList->AddText(ImVec2(pos.x - i, pos.y - i), glowU32, text);
			drawList->AddText(ImVec2(pos.x + i, pos.y - i), glowU32, text);
			drawList->AddText(ImVec2(pos.x - i, pos.y + i), glowU32, text);
			drawList->AddText(ImVec2(pos.x + i, pos.y + i), glowU32, text);
			drawList->AddText(ImVec2(pos.x - i, pos.y), glowU32, text);
			drawList->AddText(ImVec2(pos.x + i, pos.y), glowU32, text);
			drawList->AddText(ImVec2(pos.x, pos.y - i), glowU32, text);
			drawList->AddText(ImVec2(pos.x, pos.y + i), glowU32, text);
		}
		
		// Draw center text
		ImU32 centerU32 = ImGui::ColorConvertFloat4ToU32(centerColor);
		drawList->AddText(pos, centerU32, text);
	}
	
	void UIManager::DrawOriginalLayout()
	{
		// Vertical RAVENHEART text box on the far left
		ImGui::BeginChild("##ravenheart_vertical", ImVec2(60, 0), true);
		DrawVerticalRavenheart();
		ImGui::EndChild();
		
		ImVec2 ravenPos = ImGui::GetItemRectMin();
		ImVec2 ravenSize = ImGui::GetItemRectSize();
		RavenheartTheme::DrawWhiteGlowBorder(ravenPos, ravenSize);

		ImGui::SameLine();

		// Left navigation panel with submenus
		ImGui::BeginChild("##submenus", ImVec2(200, 0), true);
		ImGui::Text("Submenus: %d", (int)m_Submenus.size());
		ImGui::Text("Active: %s", m_ActiveSubmenu ? m_ActiveSubmenu->m_Name.data() : "None");
		ImGui::Separator();
		
		for (auto& submenu : m_Submenus)
		{
			if (ImGui::Selectable(submenu->m_Name.data(), (submenu == m_ActiveSubmenu)))
			{
				SetActiveSubmenu(submenu);
			}
		}
		ImGui::EndChild();
		
		ImVec2 navPos = ImGui::GetItemRectMin();
		ImVec2 navSize = ImGui::GetItemRectSize();
		RavenheartTheme::DrawAnimatedLightStrip(navPos, navSize);

		ImGui::SameLine();

		// Right side content
		ImGui::BeginChild("##right_side", ImVec2(0, 0), false);
		
		// Category selector at top
		ImGui::BeginChild("##minisubmenus", ImVec2(0, 50), true, ImGuiWindowFlags_NoScrollbar);
		if (m_ActiveSubmenu)
			m_ActiveSubmenu->DrawCategorySelectors();
		ImGui::EndChild();
		
		ImVec2 catPos = ImGui::GetItemRectMin();
		ImVec2 catSize = ImGui::GetItemRectSize();
		RavenheartTheme::DrawAnimatedLightStrip(catPos, catSize);

		// Main content area
		ImGui::BeginChild("##options", ImVec2(0, 0), true);
		if (m_OptionsFont)
			ImGui::PushFont(m_OptionsFont);

		if (m_ActiveSubmenu)
			m_ActiveSubmenu->Draw();

		if (m_OptionsFont)
			ImGui::PopFont();
		ImGui::EndChild();
		
		ImVec2 contentPos = ImGui::GetItemRectMin();
		ImVec2 contentSize = ImGui::GetItemRectSize();
		RavenheartTheme::DrawAnimatedLightStrip(contentPos, contentSize);
		
		ImGui::EndChild(); // right_side
		
		// Draw white glow border around the entire menu
		ImVec2 menuPos = ImGui::GetWindowPos();
		ImVec2 menuSize = ImGui::GetWindowSize();
		RavenheartTheme::DrawWhiteGlowBorder(menuPos, menuSize);
	}
	
	void UIManager::DrawVerticalRavenheart()
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		
		// Load and display your RAVENHEART artwork image
		// The image should be placed in the game directory as "ravenheart_banner.png"
		static ImTextureID ravenheartTexture = nullptr;
		static bool textureLoaded = false;
		
		if (!textureLoaded)
		{
			// Try to load the RAVENHEART banner image
			std::string imagePath = "ravenheart_banner.png";
			if (std::filesystem::exists(imagePath))
			{
				// Load the image using ImGui's image loading
				// For now, we'll use a placeholder texture ID
				// In a full implementation, you would load the actual image data
				LOG(al::INFO) << "RAVENHEART banner image found: " << imagePath;
				// TODO: Implement proper texture loading with actual image data
				ravenheartTexture = (ImTextureID)1; // Placeholder texture ID
				textureLoaded = true;
			}
			else
			{
				LOG(al::INFO) << "RAVENHEART banner image not found, using fallback text";
				LOG(al::INFO) << "Place your RAVENHEART banner image as 'ravenheart_banner.png' in the game directory";
			}
		}
		
		// If we have the texture, draw it vertically
		if (ravenheartTexture)
		{
			// Calculate position for vertical banner
			float bannerWidth = 60.0f; // Width of the vertical banner
			float bannerHeight = windowSize.y - 20.0f; // Full height minus padding
			float bannerX = windowPos.x + 10.0f; // Left side of the menu
			float bannerY = windowPos.y + 10.0f; // Top of the menu
			
			// Draw the RAVENHEART banner image vertically
			drawList->AddImage(ravenheartTexture, 
				ImVec2(bannerX, bannerY), 
				ImVec2(bannerX + bannerWidth, bannerY + bannerHeight),
				ImVec2(0, 0), ImVec2(1, 1), // UV coordinates
				IM_COL32_WHITE); // White tint to preserve original colors
		}
		else
		{
			// Fallback: Draw text if image not found
			float centerX = windowPos.x + windowSize.x / 2;
			const char* text = "RAVENHEART";
			float letterSpacing = 35.0f;
			float totalHeight = (strlen(text) - 1) * letterSpacing;
			float availableHeight = windowSize.y - 20;
			float centerY = windowPos.y + 10 + (availableHeight - totalHeight) / 2;
			
			for (int i = 0; i < strlen(text); i++)
			{
				char letter[2] = { text[i], '\0' };
				float yPos = centerY + (i * letterSpacing);
				
				// Deep purple color matching your artwork
				ImVec4 deepPurple = ImVec4(0.34f, 0.09f, 0.50f, 1.0f);
				ImU32 colorU32 = ImGui::ColorConvertFloat4ToU32(deepPurple);
				drawList->AddText(ImVec2(centerX, yPos), colorU32, letter);
			}
		}
	}
	

	std::shared_ptr<Submenu> UIManager::GetActiveSubmenuImpl()
	{
		if (m_ActiveSubmenu)
		{
			return m_ActiveSubmenu;
		}

		return nullptr;
	}

	std::shared_ptr<Category> UIManager::GetActiveCategoryImpl()
	{
		if (m_ActiveSubmenu)
		{
			return m_ActiveSubmenu->GetActiveCategory();
		}

		return nullptr;
	}
}
