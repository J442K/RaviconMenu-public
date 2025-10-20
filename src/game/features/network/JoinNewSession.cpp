#include "core/commands/Command.hpp"
#include "core/commands/BoolCommand.hpp"
#include "core/commands/Vector3Command.hpp"
#include "core/frontend/Notifications.hpp"
#include "game/pointers/Pointers.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/rdr/Natives.hpp"

#include <network/rlScSession.hpp>

namespace YimMenu::Features
{
	static Vector3Command _MatchmakingPosition{"newsessionpos", "Matchmaking Position", "Game position to matchmake around"};
	static BoolCommand _JoinRandomPosse{"newsessionposse", "Join Random Posse", "Automatically join a random open posse when entering a new session"};

	class JoinNewSession : public Command
	{
		using Command::Command;

		virtual void OnCall() override
		{
			// Validate pointers
			if (!Pointers.ScSession || !Pointers.RequestSessionSeamless)
			{
				Notifications::Show("Session", "Session pointers unavailable", NotificationType::Error);
				return;
			}

			// Determine matchmaking position
			auto pos = _MatchmakingPosition.GetState();
			if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f)
			{
				// Fallback to local ped position
				if (auto lp = PLAYER::PLAYER_PED_ID())
				{
					Vector3 myPos{};
					myPos = ENTITY::GET_ENTITY_COORDS(lp, true, false);
					pos   = myPos;
				}
			}

			int flags = 0;
			if (_JoinRandomPosse.GetState())
				flags |= 32;

			rage::rlScSessionId id{};
			bool started = Pointers.RequestSessionSeamless(*Pointers.ScSession, &id, flags, &pos, 0);
			if (!started)
			{
				Notifications::Show("Session", "Failed to start session request", NotificationType::Error);
				return;
			}

			// Poll with timeout and proper yielding
			auto deadline = std::chrono::steady_clock::now() + 20s;
			bool transitioned = false;
			while (NETWORK::NETWORK_SESSION_IS_REQUEST_IN_PROGRESS(&id))
			{
				if (std::chrono::steady_clock::now() > deadline)
				{
					Notifications::Show("Session", "Session request timed out", NotificationType::Warning);
					break;
				}

				if (!transitioned && NETWORK::NETWORK_SESSION_IS_REQUEST_PENDING_TRANSITION(&id))
				{
					NETWORK::_NETWORK_SESSION_TRANSITION_TO_SESSION(&id);
					transitioned = true;
					ScriptMgr::Yield(300ms);
				}
				else
				{
					ScriptMgr::Yield(100ms);
				}
			}

			if (!NETWORK::NETWORK_SESSION_IS_REQUEST_IN_PROGRESS(&id))
			{
				Notifications::Show("Session", transitioned ? "Joining new session..." : "Session request finished", NotificationType::Info);
			}
		}
	};

	static JoinNewSession _JoinNewSession{"newsession", "Join New Session", "Seamlessly joins a new session"};
}