#include "UIManager.hpp"
#include "../theme/RavenheartTheme.hpp"
#include <algorithm>
#include <cctype>
#include <string>

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
		const float topBarHeight = 64.0f;
		const float bottomBarHeight = 56.0f;
		const float navWidth = 230.0f;
		const ImVec4 neonBlue = ImVec4(0.25f, 0.65f, 1.0f, 1.0f);
		const ImVec4 neonBlueSoft = ImVec4(0.15f, 0.4f, 0.8f, 0.25f);
		const ImVec4 panelBg = ImVec4(0.07f, 0.09f, 0.14f, 0.92f);
		const ImVec4 panelBgDark = ImVec4(0.05f, 0.07f, 0.12f, 0.9f);
		const ImVec4 textPrimary = ImVec4(0.92f, 0.95f, 1.0f, 1.0f);
		const ImVec4 textSecondary = ImVec4(0.68f, 0.75f, 0.85f, 1.0f);

		// Top bar
		ImGui::BeginChild("##top_bar", ImVec2(0, topBarHeight), true, ImGuiWindowFlags_NoScrollbar);
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 barPos = ImGui::GetWindowPos();
			ImVec2 barSize = ImGui::GetWindowSize();
			ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(ImVec4(neonBlue.x, neonBlue.y, neonBlue.z, 0.9f));
			ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(neonBlueSoft);
			ImU32 bgLeft = ImGui::ColorConvertFloat4ToU32(panelBg);
			ImU32 bgRight = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.13f, 0.2f, 0.92f));

			drawList->AddRectFilledMultiColor(
				barPos,
				ImVec2(barPos.x + barSize.x, barPos.y + barSize.y),
				bgLeft,
				bgRight,
				bgRight,
				bgLeft);
			drawList->AddRect(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), borderColor, 8.0f, 0, 2.0f);
			drawList->AddRect(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), glowColor, 8.0f, 0, 6.0f);

			ImVec2 iconCenter = ImVec2(barPos.x + 24.0f, barPos.y + barSize.y * 0.5f);
			ImU32 iconColor = ImGui::ColorConvertFloat4ToU32(neonBlue);
			drawList->AddCircleFilled(ImVec2(iconCenter.x - 4.0f, iconCenter.y - 4.0f), 6.0f, iconColor);
			drawList->AddCircleFilled(ImVec2(iconCenter.x + 6.0f, iconCenter.y - 2.0f), 5.5f, iconColor);
			drawList->AddCircleFilled(ImVec2(iconCenter.x - 1.0f, iconCenter.y + 6.0f), 5.5f, iconColor);

			ImGui::PushStyleColor(ImGuiCol_Text, textPrimary);
			ImGui::SetCursorPos(ImVec2(50.0f, 18.0f));
			ImGui::TextUnformatted("ChairX");
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, textSecondary);
			ImGui::TextUnformatted("Menu |");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, textPrimary);
			ImGui::TextUnformatted("Red Dead Redemption 2");
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::SetCursorPos(ImVec2(barSize.x - 160.0f, 16.0f));
			static bool enabled = true;
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.14f, 0.22f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.35f, 0.8f, 1.0f, 1.0f));
			ImGui::Checkbox("ChairX", &enabled);
			ImGui::PopStyleColor(2);
		}
		ImGui::EndChild();

		ImGui::Spacing();

		// Middle content area
		ImGui::BeginChild("##main_area", ImVec2(0, -bottomBarHeight - 10.0f), false);
		{
			// Left navigation panel
			ImGui::BeginChild("##nav_panel", ImVec2(navWidth, 0), true);
			ImGui::Separator();
			for (auto& submenu : m_Submenus)
			{
				ImGui::PushID(submenu.get());
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 itemPos = ImGui::GetCursorScreenPos();
				ImVec2 itemSize = ImVec2(navWidth - 16.0f, 32.0f);
				bool pressed = ImGui::InvisibleButton("##nav_item", itemSize);
				bool hovered = ImGui::IsItemHovered();
				bool selected = submenu == m_ActiveSubmenu;

				ImU32 bg = ImGui::ColorConvertFloat4ToU32(selected ? ImVec4(0.16f, 0.32f, 0.55f, 0.85f)
					: hovered ? ImVec4(0.12f, 0.22f, 0.4f, 0.7f)
					: ImVec4(0.06f, 0.08f, 0.12f, 0.2f));
				ImU32 border = ImGui::ColorConvertFloat4ToU32(ImVec4(0.2f, 0.4f, 0.75f, selected ? 0.9f : 0.5f));
				drawList->AddRectFilled(itemPos, ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y), bg, 6.0f);
				drawList->AddRect(itemPos, ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y), border, 6.0f, 0, 1.5f);

				ImVec2 iconCenter = ImVec2(itemPos.x + 16.0f, itemPos.y + itemSize.y * 0.5f);
				ImU32 iconColor = ImGui::ColorConvertFloat4ToU32(neonBlue);
				drawList->AddCircleFilled(iconCenter, 5.0f, iconColor);
				drawList->AddCircleFilled(iconCenter, 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(0.88f, 0.95f, 1.0f, 1.0f)));

				ImU32 textColor = ImGui::ColorConvertFloat4ToU32(selected ? textPrimary : textSecondary);
				drawList->AddText(ImVec2(itemPos.x + 32.0f, itemPos.y + 7.0f), textColor, submenu->m_Name.data());

				if (pressed)
				{
					SetActiveSubmenu(submenu);
				}
				ImGui::PopID();
			}
			ImGui::EndChild();

			ImVec2 navPos = ImGui::GetItemRectMin();
			ImVec2 navSize = ImGui::GetItemRectSize();
			RavenheartTheme::DrawAnimatedLightStrip(navPos, navSize);

			ImGui::SameLine();

			// Right side content
			ImGui::BeginChild("##right_side", ImVec2(0, 0), false);

			// Section header
			ImGui::BeginChild("##section_header", ImVec2(0, 40), true, ImGuiWindowFlags_NoScrollbar);
			if (m_ActiveSubmenu)
			{
				std::string header = m_ActiveSubmenu->m_Name.data();
				std::transform(header.begin(), header.end(), header.begin(), [](unsigned char ch)
				{
					return static_cast<char>(std::toupper(ch));
				});
				ImGui::SetCursorPos(ImVec2(16.0f, 10.0f));
				ImGui::TextUnformatted(header.c_str());
			}
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
		}
		ImGui::EndChild();

		ImGui::Spacing();

		// Bottom action bar
		ImGui::BeginChild("##bottom_bar", ImVec2(0, bottomBarHeight), true, ImGuiWindowFlags_NoScrollbar);
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			ImVec2 barPos = ImGui::GetWindowPos();
			ImVec2 barSize = ImGui::GetWindowSize();
			ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(ImVec4(neonBlue.x, neonBlue.y, neonBlue.z, 0.9f));
			ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(neonBlueSoft);
			ImU32 bgLeft = ImGui::ColorConvertFloat4ToU32(panelBgDark);
			ImU32 bgRight = ImGui::ColorConvertFloat4ToU32(panelBg);

			drawList->AddRectFilledMultiColor(
				barPos,
				ImVec2(barPos.x + barSize.x, barPos.y + barSize.y),
				bgLeft,
				bgRight,
				bgRight,
				bgLeft);
			drawList->AddRect(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), borderColor, 8.0f, 0, 2.0f);
			drawList->AddRect(barPos, ImVec2(barPos.x + barSize.x, barPos.y + barSize.y), glowColor, 8.0f, 0, 6.0f);

			ImGui::SetCursorPos(ImVec2(12.0f, 12.0f));
			ImGui::Button("Spawn Horse", ImVec2(160, 32));
			ImGui::SameLine();
			ImGui::Button("Infinite Ammo", ImVec2(170, 32));
			ImGui::SameLine();
			ImGui::Button("MAX Cash", ImVec2(140, 32));
			ImGui::SameLine();
			ImGui::SetCursorPosX(barSize.x - 100.0f);
			ImGui::Button("Close", ImVec2(80, 32));
		}
		ImGui::EndChild();

		// Draw blue glow border around the entire menu
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
