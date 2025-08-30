#include <api/api.hpp>
#include <logic.hpp>
#include <iconkit.hpp>
#include <hiimjustin000.more_icons/include/MoreIcons.hpp>


using namespace nytelyte::icon_kit_filter_and_sort;

$execute {

  events::CompareVanillaIconsByNumber::listenGlobal(+[](int a, int b, UnlockType unlockType){
      return ::compareByNumber(a, b, unlockType);
  });

  events::CompareVanillaIconsByLockStatus::listenGlobal(+[](int a, int b, UnlockType unlockType){
      return ::compareByLockStatus(a, b, unlockType);
  });

  events::CompareVanillaIconsByCategory::listenGlobal(+[](int a, int b, UnlockType unlockType){
      return ::compareByCategory(a, b, unlockType);
  });
  
  events::CompareVanillaIconsByAuthor::listenGlobal(+[](int a, int b, UnlockType unlockType){
      return ::compareByAuthor(a, b, unlockType);
  });

  events::CompareVanillaIcons::listenGlobal(+[](int a, int b, UnlockType unlockType){
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
  
  events::RecalculateIconOrderAndTrackIcon::listenGlobal(+[](GJGarageLayer* garage, UnlockType unlockType, int itemID){
      return ::recalculateIconOrderAndTrackIcon(garage, unlockType, itemID);
  });
  
  events::GiveIconAttention::listenGlobal(+[](GJGarageLayer* garage, UnlockType unlockType, int itemID){
      ::giveIconAttention(garage, unlockType, itemID);
      return ListenerResult::Propagate;
  });
  
  events::VanillaIconDisplayToPosition::listenGlobal(+[](UnlockType unlockType, int itemID){
      return ::displayToPosition(unlockType, itemID);
  });
  
  events::VanillaIconPositionToDisplay::listenGlobal(+[](UnlockType unlockType, int itemID){
      return ::positionToDisplay(unlockType, itemID);
  });
  
  events::GetVanillaIconsInOrder::listenGlobal(+[](UnlockType unlockType){
      if (!SHOULD_CHANGE_UNLOCK_TYPE(unlockType)) return std::vector<int>{};
      std::vector<int> result = iconKitState.acceptedIcons[unlockType];
      if (iconKitState.settings.showDenied) {
        for (int icon : iconKitState.deniedIcons[unlockType]) result.push_back(icon);
      }
      return result;
  });
  
  events::GetMoreIconsIconsInOrder::listenGlobal(+[](UnlockType unlockType){
      IconType iconType = UNLOCK_TO_ICON[unlockType];
      std::vector<std::string> result;
      for (IconInfo* icon : MoreIcons::getIcons(iconType)) result.push_back(icon->name);
      return result;
  });

}
