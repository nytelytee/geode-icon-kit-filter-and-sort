#include <api/events.hpp>
#include <logic.hpp>
#include <iconkit.hpp>
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

using namespace geode::prelude;
using namespace nytelyte::icon_kit_filter_and_sort;

$execute {

	events::CompareVanillaIconsByNumber::listenAndReturn<ListenerResult::Propagate>(compareByNumber).leak();
	events::CompareVanillaIconsByLockStatus::listenAndReturn<ListenerResult::Propagate>(compareByLockStatus).leak();
	events::CompareVanillaIconsByCategory::listenAndReturn<ListenerResult::Propagate>(compareByCategory).leak();
	events::CompareVanillaIconsByAuthor::listenAndReturn<ListenerResult::Propagate>(compareByAuthor).leak();
	events::CompareVanillaIcons::listenAndReturn<ListenerResult::Propagate>(compareIcons).leak();

	events::RecalculateIconOrder::listen<ListenerResult::Propagate>(recalculateIconOrder).leak();
	events::RecalculateIconOrderAndRemainOnSamePages::listen<ListenerResult::Propagate>(recalculateIconOrderAndRemainOnSamePages).leak();
	events::RecalculateIconOrderAndRemainOnSameSectionPages::listen<ListenerResult::Propagate>(recalculateIconOrderAndRemainOnSameSectionPages).leak();
	events::RecalculateIconOrderAndGoToActiveIconPages::listen<ListenerResult::Propagate>(recalculateIconOrderAndGoToActiveIconPages).leak();
	events::RecalculateIconOrderAndTrackIcon::listenAndReturn<ListenerResult::Propagate>(recalculateIconOrderAndTrackIcon).leak();

	events::GiveIconAttention::listen<ListenerResult::Propagate>(giveIconAttention).leak();

	events::VanillaIconDisplayToPosition::listenAndReturn<ListenerResult::Propagate>(displayToPosition).leak();
	events::VanillaIconPositionToDisplay::listenAndReturn<ListenerResult::Propagate>(positionToDisplay).leak();

	events::GetVanillaIconsInOrder::listenAndReturn<ListenerResult::Propagate>(+[](UnlockType unlockType){
		if (!SHOULD_CHANGE_UNLOCK_TYPE(unlockType)) return std::vector<int>{};
		std::vector<int> result = iconKitState.acceptedIcons[unlockType];
		if (!iconKitState.settings.showDenied) return result;
		for (int icon : iconKitState.deniedIcons[unlockType]) result.push_back(icon);
		return result;
	}).leak();

	// as you can see, this simply just returns them in order for now
	// in the future, if i implement more icons sorting and filtering,
	// you will get them only the displayed ones, and in the sorting order
	events::GetMoreIconsIconsInOrder::listenAndReturn<ListenerResult::Propagate>(+[](UnlockType unlockType){
		IconType iconType = UNLOCK_TO_ICON[unlockType];
		std::vector<std::string> result;
		std::vector<IconInfo>* moreIconsIcons = more_icons::getIcons(iconType);
		if (!moreIconsIcons) return result;
		for (const IconInfo& icon : *moreIconsIcons) result.push_back(icon.getName());
		return result;
	}).leak();

}
