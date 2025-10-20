#include "core/commands/BoolCommand.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/backend/Self.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::Features
{
	static BoolCommand g_HeadOnFire{"headonfire", "Head On Fire", "Attach a burning torch to your head"};
	static BoolCommand g_Pentagram{"pentagramfeet", "Pentagram Under Feet", "Summon a pentagram beneath your feet"};

	static ENTITY::Entity g_TorchEntity = 0;
	static int g_PentagramFxHandle = 0;

	static void RequestModel(Hash model, int timeout_ms = 1000)
	{
		STREAMING::REQUEST_MODEL(model, false);
		auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			if (std::chrono::steady_clock::now() > deadline) break;
			ScriptMgr::Yield(10ms);
		}
	}

	static void ReleaseModel(Hash model)
	{
		if (STREAMING::HAS_MODEL_LOADED(model))
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	static void EnsureHeadTorch(Ped ped)
	{
		if (g_TorchEntity) return;
		const Hash torchHash = "p_torch01x"_J; // torch prop from dumps
		RequestModel(torchHash, 1200);
		Vector3 pos = ENTITY::GET_ENTITY_COORDS(ped, true, false);
		g_TorchEntity = OBJECT::CREATE_OBJECT(torchHash, pos.x, pos.y, pos.z + 0.2f, true, false, true, false, false);
		if (g_TorchEntity)
		{
			int headBone = PED::GET_PED_BONE_INDEX(ped, "SKEL_Head"_J);
			// Offset slightly above the head, minimal rotation
			ENTITY::ATTACH_ENTITY_TO_ENTITY(g_TorchEntity, ped, headBone, 0.0f, 0.02f, 0.18f, 0.0f, 0.0f, 0.0f, false, false, false, false, 2, true, false, false);
		}
	}

	static void StopHeadTorch()
	{
		if (g_TorchEntity)
		{
			ENTITY::DETACH_ENTITY(g_TorchEntity, true, true);
			ENTITY::DELETE_ENTITY(&g_TorchEntity);
			g_TorchEntity = 0;
			ReleaseModel("p_torch01x"_J);
		}
	}

	static void EnsurePentagramFx(Ped ped)
	{
		if (g_PentagramFxHandle)
			return;
		// Placeholder until correct pentagram FX is identified; attaches under feet
		GRAPHICS::USE_PARTICLE_FX_ASSET("scr_distance_smoke");
		g_PentagramFxHandle = GRAPHICS::START_PARTICLE_FX_LOOPED_ON_ENTITY("ent_amb_smoke_generator", ped, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.2f, false, false, false);
	}

	static void StopPentagramFx()
	{
		if (g_PentagramFxHandle)
		{
			GRAPHICS::STOP_PARTICLE_FX_LOOPED(g_PentagramFxHandle, true);
			g_PentagramFxHandle = 0;
		}
	}

	static void EffectsLoop()
	{
		while (true)
		{
			if (!*Pointers.IsSessionStarted)
			{
				ScriptMgr::Yield(200ms);
				continue;
			}

			Ped me = Self::GetPed().GetHandle();
			if (!me)
			{
				ScriptMgr::Yield(200ms);
				continue;
			}

			if (g_HeadOnFire.GetState())
				EnsureHeadTorch(me);
			else
				StopHeadTorch();

			if (g_Pentagram.GetState())
				EnsurePentagramFx(me);
			else
				StopPentagramFx();

			ScriptMgr::Yield(100ms);
		}
	}

	struct EffectsScriptReg
	{
		EffectsScriptReg()
		{
			ScriptMgr::AddScript(std::make_unique<Script>(&EffectsLoop));
		}
	} g_EffectsReg;
}
