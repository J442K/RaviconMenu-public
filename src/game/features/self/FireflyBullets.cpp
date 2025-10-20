#include "core/commands/BoolCommand.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/backend/Self.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::Features
{
	// Toggle state for Firefly Bullets (Molotov-like rounds)
	static BoolCommand g_FireflyBullets{"fireflybullets", "Firefly Bullets", "Bullets ignite where they land"};

	static bool RaycastFromCamera(Vector3& outHit, Entity& outEntity)
	{
		// Get camera position and forward vector, cast a ray ahead to detect impact point
		Vector3 camPos = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 camRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		float rx = camRot.x * 0.0174532924f;
		float ry = camRot.y * 0.0174532924f;
		float rz = camRot.z * 0.0174532924f;
		Vector3 dir{
			-cosf(rz) * cosf(ry),
			sinf(rz) * cosf(ry),
			sinf(ry)
		};
		Vector3 to{camPos.x + dir.x * 200.0f, camPos.y + dir.y * 200.0f, camPos.z + dir.z * 200.0f};

		// Shapetest ray
		int ray = SHAPETEST::START_SHAPE_TEST_LOS_PROBE(camPos.x, camPos.y, camPos.z, to.x, to.y, to.z, -1, Self::GetPed().GetHandle(), 7);
		int hit = 0; Vector3 end{}; Entity ent = 0;
		SHAPETEST::GET_SHAPE_TEST_RESULT(ray, &hit, &end, &outHit, &outEntity);
		return hit != 0;
	}

	static void IgniteAt(const Vector3& pos)
	{
		// Start a small script fire at the position; parameters chosen conservatively
		FIRE::START_SCRIPT_FIRE(pos.x, pos.y, pos.z, 1, false, 0.0f, 0.5f);
	}

	// Script loop: when enabled and player is shooting, place a fire at impact
	static void FireflyBulletsLoop()
	{
		static uint32_t lastShotTick = 0;
		while (true)
		{
			if (!*Pointers.IsSessionStarted)
			{
				ScriptMgr::Yield(200ms);
				continue;
			}

			if (g_FireflyBullets.GetState())
			{
				Ped me = Self::GetPed().GetHandle();
				if (me && PED::IS_PED_SHOOTING(me))
				{
					// Debounce per frame via game timer
					uint32_t now = MISC::GET_GAME_TIMER();
					if (now - lastShotTick > 50)
					{
						Vector3 hit{}; Entity ent = 0;
						if (RaycastFromCamera(hit, ent))
						{
							IgniteAt(hit);
							lastShotTick = now;
						}
					}
				}
			}

			ScriptMgr::Yield(25ms);
		}
	}

	// Register the loop at static init time
	struct FireflyBulletsScriptReg
	{
		FireflyBulletsScriptReg()
		{
			ScriptMgr::AddScript(std::make_unique<Script>(&FireflyBulletsLoop));
		}
	} g_FireflyBulletsReg;
}
