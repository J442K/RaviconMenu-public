#include "core/frontend/Notifications.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/commands/PlayerCommand.hpp"
#include "game/rdr/Natives.hpp"
#include "util/Rewards.hpp"

namespace YimMenu::Features
{
	class GiveXP : public PlayerCommand
	{
		using PlayerCommand::PlayerCommand;

		virtual void OnCall(Player player) override
		{
			if (!player.IsValid())
			{
				Notifications::Show("Give XP", "Invalid player selected", NotificationType::Error);
				return;
			}

			// Give XP rewards to the player
			FiberPool::Push([player] {
				// Give various XP types
				Rewards::GiveRequestedRewards({
					Rewards::eRewardType::XP,           // 25K XP
					Rewards::eRewardType::MOONSHINERXP, // 200 Moonshiner XP
					Rewards::eRewardType::TRADERXP,     // 200 Trader XP
					Rewards::eRewardType::COLLECTORXP,  // 200 Collector XP
					Rewards::eRewardType::NATURALISTXP, // 300 Naturalist XP
					Rewards::eRewardType::BOUNTYHUNTERXP // 200 Bounty Hunter XP
				});

				Notifications::Show("Give XP", "XP rewards given to " + player.GetName(), NotificationType::Success);
			});
		}
	};

	static GiveXP _GiveXP{"givexp", "Give XP", "Give XP rewards to the selected player"};
}
