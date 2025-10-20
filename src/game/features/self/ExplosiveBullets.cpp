#include "core/commands/LoopedCommand.hpp"
#include "core/commands/ListCommand.hpp"
#include "game/backend/Self.hpp"
#include "game/rdr/Enums.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::Features
{
	class ExplosiveBullets : public LoopedCommand
	{
		using LoopedCommand::LoopedCommand;

		virtual void OnTick() override
		{
			Ped playerPed = Self::GetPed().GetHandle();
			Vector3 impactCoords;

			if (WEAPON::GET_PED_LAST_WEAPON_IMPACT_COORD(playerPed, &impactCoords))
			{
				auto bulletType = _BulletType.GetState();

				if (bulletType == 1) // Explosive
				{
					FIRE::ADD_EXPLOSION(impactCoords.x, impactCoords.y, impactCoords.z, (int)ExplosionTypes::BIG_EXPLOSION, 5.0f, true, false, 2.0f);
				}
				else if (bulletType == 2) // Molotov
				{
					FIRE::ADD_EXPLOSION(impactCoords.x, impactCoords.y, impactCoords.z, (int)ExplosionTypes::MOLOTOV_TYPE_EXPLOSION_FIRE, 3.0f, true, false, 1.0f);
				}
			}
		}
	};

	static ListCommand _BulletType{"bullettype", "Bullet Type", "Type of bullet effect", 0, 0, 2, {"Normal", "Explosive", "Molotov"}};
	static ExplosiveBullets _ExplosiveBullets{"explosivebullets", "Explosive Bullets", "Makes your bullets explode or burn on impact"};
}

