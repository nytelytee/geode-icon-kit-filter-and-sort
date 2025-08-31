#include <api/events.hpp>
#include <logic.hpp>
#include <iconkit.hpp>
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>

using namespace nytelyte::icon_kit_filter_and_sort;

$execute {
  
  events::CompareVanillaIconsByNumber::globalSend<ListenerResult::Propagate>(compareByNumber);
  events::CompareVanillaIconsByLockStatus::globalSend<ListenerResult::Propagate>(compareByLockStatus);
  events::CompareVanillaIconsByCategory::globalSend<ListenerResult::Propagate>(compareByCategory);
  events::CompareVanillaIconsByAuthor::globalSend<ListenerResult::Propagate>(compareByAuthor);
  events::CompareVanillaIcons::globalSend<ListenerResult::Propagate>(compareIcons);

  events::RecalculateIconOrder::globalListen<ListenerResult::Propagate>(recalculateIconOrder);
  events::RecalculateIconOrderAndRemainOnSamePages::globalListen<ListenerResult::Propagate>(recalculateIconOrderAndRemainOnSamePages);
  events::RecalculateIconOrderAndRemainOnSameSectionPages::globalListen<ListenerResult::Propagate>(recalculateIconOrderAndRemainOnSameSectionPages);
  events::RecalculateIconOrderAndGoToActiveIconPages::globalListen<ListenerResult::Propagate>(recalculateIconOrderAndGoToActiveIconPages);
  events::RecalculateIconOrderAndTrackIcon::send<ListenerResult::Propagate>(recalculateIconOrderAndTrackIcon);

  events::GiveIconAttention::globalListen<ListenerResult::Propagate>(giveIconAttention);

  events::VanillaIconDisplayToPosition::globalSend<ListenerResult::Propagate>(displayToPosition);
  events::VanillaIconPositionToDisplay::globalSend<ListenerResult::Propagate>(positionToDisplay);
  
  events::GetVanillaIconsInOrder::globalSend<ListenerResult::Propagate>(+[](UnlockType unlockType){
      if (!SHOULD_CHANGE_UNLOCK_TYPE(unlockType)) return std::vector<int>{};
      std::vector<int> result = iconKitState.acceptedIcons[unlockType];
      if (iconKitState.settings.showDenied) {
        for (int icon : iconKitState.deniedIcons[unlockType]) result.push_back(icon);
      }
      return result;
  });
  
  events::GetMoreIconsIconsInOrder::globalSend<ListenerResult::Propagate>(+[](UnlockType unlockType){
      IconType iconType = UNLOCK_TO_ICON[unlockType];
      std::vector<std::string> result;
      for (IconInfo* icon : MoreIcons::getIcons(iconType)) result.push_back(icon->name);
      return result;
  });

}
