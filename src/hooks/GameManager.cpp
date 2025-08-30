#include <Geode/Geode.hpp>

#include <hooks/GameManager.hpp>

#include <algorithm>

#include <constants.hpp>
#include <iconkit.hpp>
#include <logic.hpp>

using namespace geode::prelude;

void HookedGameManager::onModify(auto& self) {
  Result<> result = self.setHookPriority("GameManager::countForType", INT_MIN);
  if (!result) log::error("Failed to set hook priority. Icon Kit Filter & Sort may act weird.");
}

int HookedGameManager::countForType(IconType iconType) {
  if ((!SHOULD_CHANGE_ICON_TYPE(iconType) || iconKitState.shouldChangeIcons == 0))
    return GameManager::countForType(iconType);
  return adjustedCountForType(iconType);
}

void HookedGameManager::reportAchievementWithID(char const* achievement, int p1, bool p2) {
  GameManager::reportAchievementWithID(achievement, p1, p2);

  GJGarageLayer* garage = CCScene::get()->getChildByType<GJGarageLayer>(0);
  if (!garage) return;

  if (std::string_view(achievement) == "geometry.ach.rate") {
    bool positionChanged = recalculateIconOrderAndTrackIcon(garage, UnlockType::Cube, 13);
    garage->selectTab(IconType::Cube);
    if (positionChanged) giveIconAttention(garage, UnlockType::Cube, 13);
  
  // Lite Ad Chests compatibility
  } else if (std::string_view(achievement) == "geometry.ach.mdrate") {

    bool positionChanged = recalculateIconOrderAndTrackIcon(garage, UnlockType::Cube, 71);
    garage->selectTab(IconType::Cube);
    if (positionChanged) giveIconAttention(garage, UnlockType::Cube, 71);

    Mod* liteAdChests = Loader::get()->getLoadedMod("colon.lite_ad_chests");
    if (liteAdChests && liteAdChests->getVersion() == *VersionInfo::parse("v1.0.0")) {
      iconKitState.stubOutSetupPageOnce = true;
    }

  }
  
}
