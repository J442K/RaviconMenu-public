#pragma once

#include <imgui.h>

namespace YimMenu
{
	class RavenheartTheme
	{
	private:
		static float s_AnimationTime;
		static float s_LightPosition;
		
	public:
		static void UpdateAnimation()
		{
			s_AnimationTime += 0.016f; // ~60fps
			s_LightPosition = (sin(s_AnimationTime * 2.0f) + 1.0f) * 0.5f; // 0.0 to 1.0
		}
		
		static void ApplyTheme()
		{
			ImGuiStyle& style = ImGui::GetStyle();
			
			// Clean modern colors like the CHERNX interface - more transparent
			ImVec4 darkBackground = ImVec4(0.05f, 0.05f, 0.05f, 0.85f);     // Very dark background - 85% opacity
			ImVec4 darkerBackground = ImVec4(0.02f, 0.02f, 0.02f, 0.80f);   // Almost black - 80% opacity
			ImVec4 glowingBlue = ImVec4(0.4f, 0.2f, 0.8f, 1.0f);           // Blue-purple glow
			ImVec4 brightBlue = ImVec4(0.6f, 0.4f, 1.0f, 1.0f);            // Bright blue-purple
			ImVec4 whiteText = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);             // Clean white text
			
			// Window colors - clean dark theme
			style.Colors[ImGuiCol_WindowBg] = darkBackground;
			style.Colors[ImGuiCol_ChildBg] = darkerBackground;
			style.Colors[ImGuiCol_PopupBg] = darkBackground;
			
			// Frame colors - clean dark with purple hover
			style.Colors[ImGuiCol_FrameBg] = darkerBackground;
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4f, 0.2f, 0.8f, 0.2f); // Purple hover
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4f, 0.2f, 0.8f, 0.3f); // Purple active
			
			// Button colors - clean with blue accents
			style.Colors[ImGuiCol_Button] = darkerBackground;
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4f, 0.2f, 0.8f, 0.3f); // Purple hover
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4f, 0.2f, 0.8f, 0.5f); // Purple active
			
			// Header colors - clean with purple hover
			style.Colors[ImGuiCol_Header] = darkerBackground;
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4f, 0.2f, 0.8f, 0.3f); // Purple hover
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4f, 0.2f, 0.8f, 0.5f); // Purple active
			
			// Text colors - clean white with blue accents
			style.Colors[ImGuiCol_Text] = whiteText;
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2f, 0.1f, 0.4f, 0.5f);
			
			// Border colors - subtle
			style.Colors[ImGuiCol_Border] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); // White
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			
			// Scrollbar colors - clean
			style.Colors[ImGuiCol_ScrollbarBg] = darkerBackground;
			style.Colors[ImGuiCol_ScrollbarGrab] = brightBlue;
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = glowingBlue;
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8f, 0.6f, 1.0f, 1.0f);
			
			// Slider colors - blue accents
			style.Colors[ImGuiCol_SliderGrab] = brightBlue;
			style.Colors[ImGuiCol_SliderGrabActive] = glowingBlue;
			
			// Checkbox colors - blue checkmarks
			style.Colors[ImGuiCol_CheckMark] = brightBlue;
			
			// Separator colors - white outlines
			style.Colors[ImGuiCol_Separator] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); // White
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Bright white
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Bright white
			
			// Resize grip colors - blue accents
			style.Colors[ImGuiCol_ResizeGrip] = brightBlue;
			style.Colors[ImGuiCol_ResizeGripHovered] = glowingBlue;
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.8f, 0.6f, 1.0f, 1.0f);
			
			// Tab colors - clean with purple hover
			style.Colors[ImGuiCol_Tab] = darkerBackground;
			style.Colors[ImGuiCol_TabHovered] = ImVec4(0.4f, 0.2f, 0.8f, 0.3f); // Purple hover
			style.Colors[ImGuiCol_TabActive] = ImVec4(0.4f, 0.2f, 0.8f, 0.5f); // Purple active
			style.Colors[ImGuiCol_TabUnfocused] = darkerBackground;
			style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
			
			// Title bar colors - clean
			style.Colors[ImGuiCol_TitleBg] = darkerBackground;
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = darkerBackground;
			
			// Menu bar colors - clean
			style.Colors[ImGuiCol_MenuBarBg] = darkerBackground;
			
			// Modal colors
			style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7f);
			
			// Table colors
			style.Colors[ImGuiCol_TableHeaderBg] = darkerBackground;
			style.Colors[ImGuiCol_TableBorderStrong] = brightBlue;
			style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f); // White
			style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
			style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.05f, 0.05f, 0.05f, 0.5f);
			
			// Plot colors
			style.Colors[ImGuiCol_PlotLines] = brightBlue;
			style.Colors[ImGuiCol_PlotLinesHovered] = glowingBlue;
			style.Colors[ImGuiCol_PlotHistogram] = brightBlue;
			style.Colors[ImGuiCol_PlotHistogramHovered] = glowingBlue;
			
			// Nav colors
			style.Colors[ImGuiCol_NavHighlight] = glowingBlue;
			style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
			style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
			
			// Drag drop colors
			style.Colors[ImGuiCol_DragDropTarget] = glowingBlue;
			
			// Docking colors (if available)
			// Note: These may not be available in all ImGui versions
			
			// Style settings for clean modern look
			style.WindowRounding = 4.0f;
			style.ChildRounding = 3.0f;
			style.FrameRounding = 2.0f;
			style.PopupRounding = 3.0f;
			style.ScrollbarRounding = 3.0f;
			style.GrabRounding = 2.0f;
			style.TabRounding = 2.0f;
			
			// Border settings for clean look - subtle borders
			style.WindowBorderSize = 1.0f;
			style.ChildBorderSize = 1.0f;
			style.PopupBorderSize = 1.0f;
			style.FrameBorderSize = 0.0f;
			style.TabBorderSize = 0.0f;
			
			// Spacing for clean layout
			style.WindowPadding = ImVec2(8, 8);
			style.FramePadding = ImVec2(6, 4);
			style.ItemSpacing = ImVec2(6, 4);
			style.ItemInnerSpacing = ImVec2(4, 3);
			style.IndentSpacing = 15.0f;
			style.ScrollbarSize = 14.0f;
			style.GrabMinSize = 10.0f;
			
			// Alpha settings
			style.Alpha = 1.0f;
			style.DisabledAlpha = 0.6f;
		}
		
		static void DrawAnimatedLightStrip(ImVec2 pos, ImVec2 size, float thickness = 3.0f)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			
			// Calculate light position along the border
			float perimeter = 2.0f * (size.x + size.y);
			float lightPos = s_LightPosition * perimeter;
			
			// Purple light color with glow
			ImU32 lightColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.8f, 0.2f, 1.0f, 1.0f));
			ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.8f, 0.2f, 1.0f, 0.6f));
			
			// Draw light strip along the border
			if (lightPos < size.x) {
				// Top edge
				ImVec2 start = ImVec2(pos.x + lightPos - thickness, pos.y - 1);
				ImVec2 end = ImVec2(pos.x + lightPos + thickness, pos.y + 1);
				drawList->AddRectFilled(start, end, lightColor);
				// Glow effect
				drawList->AddRectFilled(ImVec2(start.x - 3, start.y - 3), ImVec2(end.x + 3, end.y + 3), glowColor);
			}
			else if (lightPos < size.x + size.y) {
				// Right edge
				float yPos = lightPos - size.x;
				ImVec2 start = ImVec2(pos.x + size.x - 1, pos.y + yPos - thickness);
				ImVec2 end = ImVec2(pos.x + size.x + 1, pos.y + yPos + thickness);
				drawList->AddRectFilled(start, end, lightColor);
				// Glow effect
				drawList->AddRectFilled(ImVec2(start.x - 3, start.y - 3), ImVec2(end.x + 3, end.y + 3), glowColor);
			}
			else if (lightPos < 2.0f * size.x + size.y) {
				// Bottom edge
				float xPos = lightPos - size.x - size.y;
				ImVec2 start = ImVec2(pos.x + size.x - xPos - thickness, pos.y + size.y - 1);
				ImVec2 end = ImVec2(pos.x + size.x - xPos + thickness, pos.y + size.y + 1);
				drawList->AddRectFilled(start, end, lightColor);
				// Glow effect
				drawList->AddRectFilled(ImVec2(start.x - 3, start.y - 3), ImVec2(end.x + 3, end.y + 3), glowColor);
			}
			else {
				// Left edge
				float yPos = lightPos - 2.0f * size.x - size.y;
				ImVec2 start = ImVec2(pos.x - 1, pos.y + size.y - yPos - thickness);
				ImVec2 end = ImVec2(pos.x + 1, pos.y + size.y - yPos + thickness);
				drawList->AddRectFilled(start, end, lightColor);
				// Glow effect
				drawList->AddRectFilled(ImVec2(start.x - 3, start.y - 3), ImVec2(end.x + 3, end.y + 3), glowColor);
			}
		}
		
		static void DrawWhiteGlowBorder(ImVec2 pos, ImVec2 size)
		{
			ImDrawList* drawList = ImGui::GetWindowDrawList();
			
			// White glow border around the entire panel
			ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
			ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.8f));
			
			// Draw glow effect around the border
			for (int i = 2; i >= 1; i--)
			{
				ImVec4 glow = ImVec4(1.0f, 1.0f, 1.0f, 0.2f / i);
				ImU32 glowU32 = ImGui::ColorConvertFloat4ToU32(glow);
				
				// Top edge glow
				drawList->AddRectFilled(
					ImVec2(pos.x - i, pos.y - i), 
					ImVec2(pos.x + size.x + i, pos.y + i), 
					glowU32
				);
				
				// Bottom edge glow
				drawList->AddRectFilled(
					ImVec2(pos.x - i, pos.y + size.y - i), 
					ImVec2(pos.x + size.x + i, pos.y + size.y + i), 
					glowU32
				);
				
				// Left edge glow
				drawList->AddRectFilled(
					ImVec2(pos.x - i, pos.y - i), 
					ImVec2(pos.x + i, pos.y + size.y + i), 
					glowU32
				);
				
				// Right edge glow
				drawList->AddRectFilled(
					ImVec2(pos.x + size.x - i, pos.y - i), 
					ImVec2(pos.x + size.x + i, pos.y + size.y + i), 
					glowU32
				);
			}
			
			// Draw main white border
			drawList->AddRect(
				ImVec2(pos.x, pos.y), 
				ImVec2(pos.x + size.x, pos.y + size.y), 
				borderColor, 0.0f, 0, 2.0f
			);
		}
		
		static void DrawOrnateBorder(const char* title, ImVec2 size)
		{
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.6f, 0.2f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 2.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.0f);
			
			if (ImGui::BeginChild(title, size, true, ImGuiWindowFlags_NoScrollbar))
			{
				// Draw ornate corner decorations
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 windowPos = ImGui::GetWindowPos();
				ImVec2 windowSize = ImGui::GetWindowSize();
				
				// Draw corner scrollwork
				ImVec4 scrollColor = ImVec4(1.0f, 0.4f, 0.1f, 0.8f);
				float scrollSize = 20.0f;
				
				// Top-left corner
				drawList->AddCircleFilled(ImVec2(windowPos.x + scrollSize, windowPos.y + scrollSize), 
					scrollSize * 0.3f, ImGui::ColorConvertFloat4ToU32(scrollColor));
				
				// Top-right corner  
				drawList->AddCircleFilled(ImVec2(windowPos.x + windowSize.x - scrollSize, windowPos.y + scrollSize), 
					scrollSize * 0.3f, ImGui::ColorConvertFloat4ToU32(scrollColor));
				
				// Bottom-left corner
				drawList->AddCircleFilled(ImVec2(windowPos.x + scrollSize, windowPos.y + windowSize.y - scrollSize), 
					scrollSize * 0.3f, ImGui::ColorConvertFloat4ToU32(scrollColor));
				
				// Bottom-right corner
				drawList->AddCircleFilled(ImVec2(windowPos.x + windowSize.x - scrollSize, windowPos.y + windowSize.y - scrollSize), 
					scrollSize * 0.3f, ImGui::ColorConvertFloat4ToU32(scrollColor));
			}
			ImGui::EndChild();
			
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor();
		}
	};
	
	// Static variable definitions
	float RavenheartTheme::s_AnimationTime = 0.0f;
	float RavenheartTheme::s_LightPosition = 0.0f;
}
