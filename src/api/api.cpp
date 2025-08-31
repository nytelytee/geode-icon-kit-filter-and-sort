#include <api/api.hpp>
#include <logic.hpp>
#include <iconkit.hpp>
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>


using namespace nytelyte::icon_kit_filter_and_sort;

$execute {

  events::CompareVanillaIconsByNumber::sendGlobal(ListenerResult::Propagate, +[](int a, int b, UnlockType unlockType){
      return ::compareByNumber(a, b, unlockType);
  });

  events::CompareVanillaIconsByLockStatus::sendGlobal(ListenerResult::Propagate, +[](int a, int b, UnlockType unlockType){
      return ::compareByLockStatus(a, b, unlockType);
  });

  events::CompareVanillaIconsByCategory::sendGlobal(ListenerResult::Propagate, +[](int a, int b, UnlockType unlockType){
      return ::compareByCategory(a, b, unlockType);
  });
  
  events::CompareVanillaIconsByAuthor::sendGlobal(ListenerResult::Propagate, +[](int a, int b, UnlockType unlockType){
      return ::compareByAuthor(a, b, unlockType);
  });

  events::CompareVanillaIcons::sendGlobal(ListenerResult::Propagate, +[](int a, int b, UnlockType unlockType){
      return ::compareIcons(a, b, unlockType);
  });
  
  events::RecalculateIconOrder::listenGlobal(+[](){
      ::recalculateIconOrder();
      return ListenerResult::Propagate;
  });
  
  events::RecalculateIconOrderAndRemainOnSamePages::listenGlobal(+[](GJGarageLayer* garage){
      ::recalculateIconOrderAndRemainOnSamePages(garage);
      return ListenerResult::Propagate;
  });
  
  events::RecalculateIconOrderAndRemainOnSameSectionPages::listenGlobal(+[](GJGarageLayer* garage){
      ::recalculateIconOrderAndRemainOnSameSectionPages(garage);
      return ListenerResult::Propagate;
  });
  
  events::RecalculateIconOrderAndGoToActiveIconPages::listenGlobal(+[](GJGarageLayer* garage){
      ::recalculateIconOrderAndGoToActiveIconPages(garage);
      return ListenerResult::Propagate;
  });
  
  events::RecalculateIconOrderAndTrackIcon::sendGlobal(ListenerResult::Propagate, +[](GJGarageLayer* garage, UnlockType unlockType, int itemID){
      return ::recalculateIconOrderAndTrackIcon(garage, unlockType, itemID);
  });
  
  events::GiveIconAttention::listenGlobal(+[](GJGarageLayer* garage, UnlockType unlockType, int itemID){
      ::giveIconAttention(garage, unlockType, itemID);
      return ListenerResult::Propagate;
  });
  
  events::VanillaIconDisplayToPosition::sendGlobal(ListenerResult::Propagate, +[](UnlockType unlockType, int itemID){
      return ::displayToPosition(unlockType, itemID);
  });
  
  events::VanillaIconPositionToDisplay::sendGlobal(ListenerResult::Propagate, +[](UnlockType unlockType, int itemID){
      return ::positionToDisplay(unlockType, itemID);
  });
  
  events::GetVanillaIconsInOrder::sendGlobal(ListenerResult::Propagate, +[](UnlockType unlockType){
      if (!SHOULD_CHANGE_UNLOCK_TYPE(unlockType)) return std::vector<int>{};
      std::vector<int> result = iconKitState.acceptedIcons[unlockType];
      if (iconKitState.settings.showDenied) {
        for (int icon : iconKitState.deniedIcons[unlockType]) result.push_back(icon);
      }
      return result;
  });
  
  events::GetMoreIconsIconsInOrder::sendGlobal(ListenerResult::Propagate, +[](UnlockType unlockType){
      IconType iconType = UNLOCK_TO_ICON[unlockType];
      std::vector<std::string> result;
      for (IconInfo* icon : MoreIcons::getIcons(iconType)) result.push_back(icon->name);
      return result;
  });

}
