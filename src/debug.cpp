#include <Geode/loader/GameEvent.hpp>

#include <geode.devtools/include/API.hpp>

#include <iconkit.hpp>
#include <constants.hpp>
#include <api/api.hpp>

$on_mod(Loaded) {
	devtools::waitForDevTools([] {
		devtools::registerNode<GJGarageLayer>([](GJGarageLayer* node) {
			devtools::label("Icon Kit Filter & Sort\n");
			devtools::label(fmt::format("Current page: {}", node->m_iconPages[node->m_iconType]).c_str());
			devtools::label(fmt::format("Current icon type: {}", ICON_NAMES[ICON_TO_UNLOCK[node->m_iconType]]).c_str());
			devtools::label(fmt::format("Accepted icons: {}", iconKitState.acceptedIcons[ICON_TO_UNLOCK[node->m_iconType]].size()).c_str());
			devtools::label(fmt::format("Denied icons: {}", iconKitState.deniedIcons[ICON_TO_UNLOCK[node->m_iconType]].size()).c_str());
			devtools::button("Reset Supporter Icons", []{
				GameManager::get()->resetAchievement("geometry.ach.rate");
				GameManager::get()->resetAchievement("geometry.ach.mdrate");
			});
			devtools::button("Track icon 0", [node]{
				nytelyte::icon_kit_filter_and_sort::events::GiveIconAttention::send(node, UnlockType::Cube, 1);
			});

		});
		devtools::registerNode<PurchaseItemPopup>([](PurchaseItemPopup* node) {
			devtools::label("Icon Kit Filter & Sort\n");
			devtools::label(fmt::format("Index: {}", node->m_storeItem->m_index.value()).c_str());
			devtools::label(fmt::format("TypeID: {}", node->m_storeItem->m_typeID.value()).c_str());
			devtools::label(fmt::format("Unlock Type: {}", node->m_storeItem->m_unlockType.value()).c_str());
			devtools::label(fmt::format("Price: {}", node->m_storeItem->m_price.value()).c_str());
		});
	});
}

