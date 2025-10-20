#include "Crash.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/FiberPool.hpp"
#include "game/backend/Players.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/rdr/Natives.hpp"
#include "util/RateLimiter.hpp"

namespace YimMenu::Submenus
{
	static void RequestModel(Hash model, int timeout_ms = 1000)
	{
		STREAMING::REQUEST_MODEL(model, false);
		const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			if (std::chrono::steady_clock::now() > deadline)
				break;
			ScriptMgr::Yield(10ms);
		}
	}

	static void ReleaseModel(Hash model)
	{
		if (STREAMING::HAS_MODEL_LOADED(model))
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::shared_ptr<Category> BuildCrashMenu()
	{
		auto menu = std::make_shared<Category>("Crash");

		// 1) Attach Spam Crash
		menu->AddItem(std::make_shared<ImGuiItem>([] {
			if (ImGui::Button("Attach Spam"))
			{
				FiberPool::Push([] {
					auto sel = Players::GetSelected();
					if (!sel.IsValid() || !sel.GetPed())
						return;

					auto ped = sel.GetPed();
					auto pos = ped.GetPosition();
					const Hash prop = "p_steamer_trunk_001"_J; // simple small prop
					RequestModel(prop, 800);

					std::vector<ENTITY::Entity> spawned;
					spawned.reserve(8);
					for (int i = 0; i < 8; ++i)
					{
						auto obj = OBJECT::CREATE_OBJECT(prop, pos.x + 0.2f * i, pos.y, pos.z + 0.1f * i, true, true, false, false, false);
						if (obj)
						{
							ENTITY::SET_ENTITY_VISIBLE(obj, false);
							ENTITY::ATTACH_ENTITY_TO_ENTITY(obj, ped, -1, 0.05f * i, 0.03f * i, 0.02f * i, 10.0f * i, 0.0f, 0.0f, false, false, true, false, 2, true, false, false);
							spawned.push_back(obj);
						}
						ScriptMgr::Yield(25ms);
					}
					Notifications::Show("Crash", "Attach spam executed on " + sel.GetName(), NotificationType::Info);
					ScriptMgr::Yield(500ms);
					for (auto e : spawned)
					{
						if (e)
						{
							ENTITY::DETACH_ENTITY(e, true, true);
							ENTITY::DELETE_ENTITY(&e);
						}
					}
					ReleaseModel(prop);
				});
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Attach small invisible props to the target in quick succession");
		}));

		// 2) Rope Spam Crash
		menu->AddItem(std::make_shared<ImGuiItem>([] {
			if (ImGui::Button("Rope Spam"))
			{
				FiberPool::Push([] {
					auto sel = Players::GetSelected();
					if (!sel.IsValid() || !sel.GetPed())
						return;

					auto ped = sel.GetPed();
					auto pos = ped.GetPosition();
					const Hash dummy = "p_basket01x"_J;
					RequestModel(dummy, 800);

					ENTITY::Entity anchor = OBJECT::CREATE_OBJECT(dummy, pos.x + 1.0f, pos.y, pos.z, false, true, false, false, false);
					if (!anchor)
						return;
					ENTITY::SET_ENTITY_VISIBLE(anchor, false);

					std::array<PHYSICS::Rope, 6> ropes{};
					for (int i = 0; i < 6; ++i)
					{
						ropes[i] = PHYSICS::ADD_ROPE(pos.x, pos.y, pos.z + 0.2f * i, 0.0f, 0.0f, 0.0f, 1.0f, 1, 1.0f, 0.1f, 0.0f, false, false, true, 1.0f, false, false);
						if (ropes[i])
						{
							PHYSICS::ATTACH_ENTITIES_TO_ROPE(ropes[i], ped, anchor, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.1f, 1.0f, false, false);
						}
						ScriptMgr::Yield(20ms);
					}
					Notifications::Show("Crash", "Rope spam executed on " + sel.GetName(), NotificationType::Info);
					ScriptMgr::Yield(600ms);
					for (auto& r : ropes)
					{
						if (r)
							PHYSICS::DELETE_ROPE(&r);
					}
					if (anchor)
						ENTITY::DELETE_ENTITY(&anchor);
					ReleaseModel(dummy);
				});
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Create short ropes between target and an invisible anchor");
		}));

		// 3) Particle FX Spam
		menu->AddItem(std::make_shared<ImGuiItem>([] {
			if (ImGui::Button("Particle Spam"))
			{
				FiberPool::Push([] {
					auto sel = Players::GetSelected();
					if (!sel.IsValid() || !sel.GetPed())
						return;

					auto ped = sel.GetPed();
					std::array<int, 6> handles{};
					GRAPHICS::USE_PARTICLE_FX_ASSET("scr_distance_smoke");
					for (int i = 0; i < 6; ++i)
					{
						handles[i] = GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY("ent_amb_smoke_generator", ped, 0.0f, 0.0f, 0.05f * i, 0.0f, 0.0f, 0.0f, 1.0f, false, false, false);
						ScriptMgr::Yield(30ms);
					}
					Notifications::Show("Crash", "Particle FX spam executed on " + sel.GetName(), NotificationType::Info);
					ScriptMgr::Yield(700ms);
					for (auto h : handles)
					{
						if (h)
							GRAPHICS::STOP_PARTICLE_FX_LOOPED(h, true);
					}
				});
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Start several looped PFX on the target then stop them");
		}));

		// 4) Invalid Anim Attempt
		menu->AddItem(std::make_shared<ImGuiItem>([] {
			if (ImGui::Button("Invalid Anim"))
			{
				FiberPool::Push([] {
					auto sel = Players::GetSelected();
					if (!sel.IsValid() || !sel.GetPed())
						return;

					auto ped = sel.GetPed();
					const char* dict = "amb_rest@world_human_smoke@male_b@idle_a"; // common dict
					STREAMING::REQUEST_ANIM_DICT(dict);
					for (int i = 0; i < 30; ++i)
					{
						TASK::TASK_PLAY_ANIM(ped, dict, "clip_does_not_exist", 4.0f, -4.0f, 1000, 0, 0.0f, false, false, false, false, 0, false);
						ScriptMgr::Yield(50ms);
					}
					Notifications::Show("Crash", "Invalid anim attempts on " + sel.GetName(), NotificationType::Info);
				});
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Attempts to play a non-existent clip on the target repeatedly");
		}));

		return menu;
	}
}
