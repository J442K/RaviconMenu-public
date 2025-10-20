#include "Effects.hpp"

#include "core/commands/BoolCommand.hpp"
#include "game/frontend/items/Items.hpp"

namespace YimMenu::Submenus
{
	Effects::Effects() : Submenu::Submenu("EFFECTS")
	{
		auto cat = std::make_shared<Category>("Visual Effects");
		cat->AddItem(std::make_shared<BoolCommandItem>("headonfire"_J, "Head On Fire"));
		cat->AddItem(std::make_shared<BoolCommandItem>("pentagramfeet"_J, "Pentagram Under Feet"));
		AddCategory(std::move(cat));
	}
}
