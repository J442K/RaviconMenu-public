#include "Items.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/Command.hpp"
#include "core/commands/LoopedCommand.hpp"
#include "core/frontend/widgets/toggle/imgui_toggle.hpp"
#include "core/frontend/widgets/toggle/imgui_toggle_presets.hpp"

namespace YimMenu
{
	BoolCommandItem::BoolCommandItem(joaat_t id, std::optional<std::string> label_override) :
	    m_Command(Commands::GetCommand<BoolCommand>(id)),
	    m_LabelOverride(label_override)
	{
	}

	void BoolCommandItem::Draw()
	{
		if (!m_Command)
		{
			ImGui::Text("Unknown!");
			return;
		}

		bool enabled = m_Command->GetState();
		const char* label = m_LabelOverride.has_value() ? m_LabelOverride.value().data() : m_Command->GetLabel().data();

		ImGuiToggleConfig config = ImGuiTogglePresets::GlowingStyle();
		config.Flags |= ImGuiToggleFlags_Bordered | ImGuiToggleFlags_Shadowed;
		config.A11yStyle = ImGuiToggleA11yStyle_Glyph;
		config.FrameRounding = 1.0f;
		config.KnobRounding = 1.0f;
		config.WidthRatio = 2.6f;

		static ImGuiTogglePalette onPalette = {
			ImVec4(0.92f, 0.95f, 1.0f, 1.0f),
			ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImVec4(0.16f, 0.5f, 0.95f, 1.0f),
			ImVec4(0.2f, 0.65f, 1.0f, 1.0f),
			ImVec4(0.3f, 0.75f, 1.0f, 1.0f),
			ImVec4(0.15f, 0.35f, 0.6f, 0.6f),
			ImVec4(0.65f, 0.85f, 1.0f, 1.0f),
			ImVec4(0.15f, 0.25f, 0.4f, 0.6f),
			ImVec4(0.9f, 0.95f, 1.0f, 1.0f),
		};

		static ImGuiTogglePalette offPalette = {
			ImVec4(0.65f, 0.7f, 0.8f, 1.0f),
			ImVec4(0.72f, 0.78f, 0.86f, 1.0f),
			ImVec4(0.12f, 0.16f, 0.22f, 1.0f),
			ImVec4(0.18f, 0.24f, 0.32f, 1.0f),
			ImVec4(0.25f, 0.35f, 0.5f, 0.8f),
			ImVec4(0.05f, 0.08f, 0.12f, 0.6f),
			ImVec4(0.3f, 0.4f, 0.55f, 0.9f),
			ImVec4(0.05f, 0.08f, 0.12f, 0.6f),
			ImVec4(0.6f, 0.7f, 0.8f, 1.0f),
		};

		config.On.Palette = &onPalette;
		config.Off.Palette = &offPalette;

		ImGui::PushID(static_cast<int>(m_Command->GetHash()));
		bool rowHovered = false;
		if (ImGui::BeginTable("##bool_cmd_row", 2, ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(label);
			rowHovered = rowHovered || ImGui::IsItemHovered();
			ImGui::TableSetColumnIndex(1);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth(1) - 60.0f);
			if (ImGui::Toggle("##toggle", &enabled, config, ImVec2(52.0f, 22.0f)))
			{
				m_Command->SetState(enabled);
			}
			rowHovered = rowHovered || ImGui::IsItemHovered();
			ImGui::EndTable();
		}
		ImGui::PopID();

		// TODO: refactor this

		auto windowLabel = std::format("{} Hotkey", m_Command->GetLabel());

		if (rowHovered)
		{
			ImGui::SetTooltip(m_Command->GetDescription().data());
			if (GetAsyncKeyState(VK_OEM_3) & 0x8000)
				ImGui::OpenPopup(std::format("{} Hotkey", m_Command->GetLabel()).data());
		}

		ImGui::SetNextWindowSize(ImVec2(500, 120));
		if (ImGui::BeginPopupModal(windowLabel.data(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::BulletText("Hover over the command name to change its hotkey");
			ImGui::BulletText("Press any registered key to remove");
			ImGui::Separator();

			HotkeySetter(m_Command->GetHash()).Draw();

			
			ImGui::Spacing();
			if (ImGui::Button("Close") || ((!ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered()) && ImGui::IsMouseClicked(ImGuiMouseButton_Left)))
				ImGui::CloseCurrentPopup();

			ImGui::EndPopup();
		}
	}
}
