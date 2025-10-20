#include "Train.hpp"

#include "core/commands/BoolCommand.hpp"
#include "core/commands/Commands.hpp"
#include "core/commands/IntCommand.hpp"
#include "game/backend/FiberPool.hpp"
#include "game/backend/ScriptMgr.hpp"
#include "game/backend/Self.hpp"
#include "game/features/Features.hpp"
#include "game/frontend/items/Items.hpp"
#include "game/rdr/Natives.hpp"

namespace YimMenu::Submenus
{
	Train::Train() :
	    Submenu::Submenu("TRAIN")
	{
		auto main = std::make_shared<Category>("Main");
		auto trainGroup = std::make_shared<Group>("Train Options");

		trainGroup->AddItem(std::make_shared<BoolCommandItem>("spawn_train"_J));
		trainGroup->AddItem(std::make_shared<BoolCommandItem>("teleport_to_train"_J));
		trainGroup->AddItem(std::make_shared<BoolCommandItem>("train_godmode"_J));

		main->AddItem(trainGroup);

		AddCategory(std::move(main));
	}
}














