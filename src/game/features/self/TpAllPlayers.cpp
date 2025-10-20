#include "core/commands/Command.hpp"
#include "game/backend/Self.hpp"
#include "game/backend/Players.hpp"
#include "util/teleport.hpp"
#include "core/frontend/Notifications.hpp"

namespace YimMenu::Features
{
	class TpAllPlayers : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			auto selfPos = Self::GetPed().GetPosition();
			int playerCount = 0;
			
			FiberPool::Push([selfPos, &playerCount] {
				for (auto& [id, player] : Players::GetPlayers())
				{
					if (player.IsValid() && !player.IsDead())
					{
						// Teleport each player to a slightly offset position around the self
						float angle = (playerCount * 2.0f * 3.14159f) / 8.0f; // Spread in a circle
						float radius = 5.0f + (playerCount * 2.0f); // Increasing radius
						
						Vector3 offsetPos = {
							selfPos.x + radius * cos(angle),
							selfPos.y + radius * sin(angle),
							selfPos.z
						};
						
						Teleport::TeleportPlayerToCoords(player, offsetPos);
						playerCount++;
						ScriptMgr::Yield(100ms); // Small delay between teleports
					}
				}
				
				Notifications::Show("Teleport All", 
					std::format("Teleported {} players to your location", playerCount), 
					NotificationType::Success);
			});
		}
	};

	static TpAllPlayers _TpAllPlayers{"tpallplayers", "Teleport All Players", "Teleport all players to your location"};
}
