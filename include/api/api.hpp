#pragma once

#include <Geode/loader/Mod.hpp>
#include <api/base.hpp>
#include <api/events.hpp>

// declaration and documentation
namespace nytelyte {
  namespace icon_kit_filter_and_sort {

    // the comparison functions used in sorting
    geode::Result<std::optional<bool>> compareVanillaIconsByNumber(int, int, UnlockType);
    geode::Result<std::optional<bool>> compareVanillaIconsByLockStatus(int, int, UnlockType);
    geode::Result<std::optional<bool>> compareVanillaIconsByCategory(int, int, UnlockType);
    geode::Result<std::optional<bool>> compareVanillaIconsByAuthor(int, int, UnlockType);
    // the main comparison function, calls the ones above in the user defined sorting order
    geode::Result<bool> compareVanillaIcons(int, int, UnlockType);
    
    // recalculates the icon order, does nothing else
    geode::Result<> recalculateIconOrder();
    
    // more specialized versions of recalculation of the icon order are below here
    // do note that none of them call GJGarageLayer::selectTab, you have to do that yourself
    // after you recalculate the order (`GJGarageLayer::selectTab(iconType);`)

    // recalculate icon order, update the garage's pages and try to stay on the same page,
    // clamping it to the maximum page if out of bounds
    geode::Result<> recalculateIconOrderAndRemainOnSamePages(GJGarageLayer*);

    // this one is hard to explain i'll do it later
    geode::Result<> recalculateIconOrderAndRemainOnSameSectionPages(GJGarageLayer*);
    
    // recalculate the icon order, and go to the page the specified icon is now on
    // if the specified icon is not on any page, acts like
    // recalculateIconOrderAndRemainOnSameSectionPages
    // returns true if the icon:
    //   was previously filtered out, and now is no longer filtered out
    //   was previously not filtered out, and still is not filtered out, but its position changed
    // returns false otherwise
    geode::Result<bool> recalculateIconOrderAndTrackIcon(GJGarageLayer*, UnlockType, int);
    
    // recalculate the icon order, update all the pages to point to the active icon page
    geode::Result<> recalculateIconOrderAndGoToActiveIconPages(GJGarageLayer*);

    // flashes the specified icon with a red cursor briefly
    // meant to be used with recalculateIconOrderAndTrackIcon if it returns true
    // (since the icon switched positions, it makes sure the user can still find it)
    geode::Result<> giveIconAttention(GJGarageLayer*, UnlockType, int);
    
    // input:  the current icon position
    // output: the icon number that should be displayed in that position or
    //         0 if the given position is out of bounds
    geode::Result<int> vanillaIconPositionToDisplay(UnlockType, int);
    
    // input:  an icon number
    // output: the position of that icon in the icon kit or
    //         0 if the given position is not displayed in the icon kit
    // inverse of vanillaIconPositionToDisplay
    geode::Result<int> vanillaIconDisplayToPosition(UnlockType, int);
    
    // get the currently active icon's page, based on the position it is on
    // accounts for both Separate Dual Icons and More Icons
    geode::Result<int> getActiveIconPage(UnlockType);
    
    // get all of the icons of the given unlock type visible in the garage in the order they're displayed in
    geode::Result<std::vector<Icon>> getAllIconsInOrder(UnlockType);

    // get all of the vanilla icons of the given unlock type visible in the garage in the order they're displayed in
    geode::Result<std::vector<int>> getVanillaIconsInOrder(UnlockType);

    // get all of the more icons icons of the given unlock type visible in the garage in the order they're displayed in
    geode::Result<std::vector<std::string>> getMoreIconsIconsInOrder(UnlockType);

  }
}

// implementation
namespace nytelyte {
  namespace icon_kit_filter_and_sort {
      
    inline geode::Result<std::optional<bool>> compareVanillaIconsByNumber(int a, int b, UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::CompareVanillaIconsByNumber::post(a, b, unlockType));
    }
    
    inline geode::Result<std::optional<bool>> compareVanillaIconsByLockStatus(int a, int b, UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::CompareVanillaIconsByLockStatus::post(a, b, unlockType));
    }
    
    inline geode::Result<std::optional<bool>> compareVanillaIconsByCategory(int a, int b, UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::CompareVanillaIconsByCategory::post(a, b, unlockType));
    }
    
    inline geode::Result<std::optional<bool>> compareVanillaIconsByAuthor(int a, int b, UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::CompareVanillaIconsByAuthor::post(a, b, unlockType));
    }
    
    inline geode::Result<bool> compareVanillaIcons(int a, int b, UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::CompareVanillaIcons::post(a, b, unlockType));
    }
    
    inline geode::Result<> recalculateIconOrder() {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      events::RecalculateIconOrder::post();
      return geode::Ok();
    }
    
    inline geode::Result<> recalculateIconOrderAndRemainOnSamePages(GJGarageLayer* garage) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      events::RecalculateIconOrderAndRemainOnSamePages::post(garage);
      return geode::Ok();
    }

    inline geode::Result<> recalculateIconOrderAndRemainOnSameSectionPages(GJGarageLayer* garage) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      events::RecalculateIconOrderAndRemainOnSameSectionPages::post(garage);
      return geode::Ok();
    }
    
    inline geode::Result<bool> recalculateIconOrderAndTrackIcon(GJGarageLayer* garage, UnlockType unlockType, int iconID) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::RecalculateIconOrderAndTrackIcon::post(garage, unlockType, iconID));
    }
    
    inline geode::Result<> recalculateIconOrderAndGoToActiveIconPages(GJGarageLayer* garage) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      events::RecalculateIconOrderAndGoToActiveIconPages::post(garage);
      return geode::Ok();
    }

    inline geode::Result<> giveIconAttention(GJGarageLayer* garage, UnlockType unlockType, int iconID) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      events::GiveIconAttention::post(garage, unlockType, iconID);
      return geode::Ok();
    }
    
    inline geode::Result<int> vanillaIconPositionToDisplay(UnlockType unlockType, int iconID) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::VanillaIconDisplayToPosition::post(unlockType, iconID));
    }
    
    inline geode::Result<int> vanillaIconDisplayToPosition(UnlockType unlockType, int iconID) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::VanillaIconPositionToDisplay::post(unlockType, iconID));
    }
    
    inline geode::Result<int> getActiveIconPage(UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::GetActiveIconPage::post(unlockType));
    }

    inline geode::Result<std::vector<Icon>> getAllIconsInOrder(UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      std::vector<int> vanillaIcons = events::GetVanillaIconsInOrder::post(unlockType);
      std::vector<std::string> moreIconsIcons = events::GetMoreIconsIconsInOrder::post(unlockType);
      std::vector<Icon> result;
      for (int icon : vanillaIcons) result.push_back(Icon{icon});
      for (std::string& icon : moreIconsIcons) result.push_back(Icon{icon});
      return geode::Ok(result);
    }

    inline geode::Result<std::vector<int>> getVanillaIconsInOrder(UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::GetVanillaIconsInOrder::post(unlockType));
    }
    
    inline geode::Result<std::vector<std::string>> getMoreIconsIconsInOrder(UnlockType unlockType) {
      if (!geode::Loader::get()->isModLoaded(ICON_KIT_FILTER_AND_SORT_ID)) return geode::Err("Mod not loaded");
      return geode::Ok(events::GetMoreIconsIconsInOrder::post(unlockType));
    }

  }
}
