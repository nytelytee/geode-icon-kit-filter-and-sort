#include <Geode/Geode.hpp>
#include <iconkit.hpp>

#include <api/api.hpp>

using namespace geode::prelude;

[[maybe_unused]] static inline void dumpAuthorJson() {
  GameStatsManager* gsm = GameStatsManager::get();
  std::map<std::string, std::map<std::string, std::vector<std::string>>> intermediate;
  
  for (auto& [icon, count] : VANILLA_MAX_ICONS) {
    for (int i = 1; i <= count; i++) {
      // this is going to implicitly generate a 0 for all the unassigned authors, and 0 is assumed to be Robert Nicholas Christian Topala
      std::pair<int, UnlockType> temp{i, icon};
      int author = gsm->m_accountIDForIcon[temp];
      intermediate[ICON_NAMES[icon]][fmt::format("{}", author)].push_back(fmt::format("{}", i));
    }
  }

  matjson::Value value = intermediate;
  log::info("{}", value.dump(matjson::NO_INDENTATION));
}

$on_mod(Loaded) {

  if (Mod::get()->hasSavedValue("iconKitSettings")) iconKitState.pendingSettings = Mod::get()->getSavedValue<IconKitSettings>("iconKitSettings");
  else iconKitState.pendingSettings.initDefault();
  
  // get this from the game at the start; i used to hardcode it
  // not doing that again ever since i forgot to change it and did not notice for days
  for (UnlockType unlockType : UNLOCK_TYPES_TO_CHANGE)
    VANILLA_MAX_ICONS[unlockType] = GameManager::get()->countForType(UNLOCK_TO_ICON[unlockType]);

  //dumpAuthorJson();
  
}
