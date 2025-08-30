#pragma once

#include <Geode/Geode.hpp>

#pragma warning(push)
#pragma warning(disable: 4265)  // non-virtual destructor
#include <Geode/modify/GameManager.hpp>
#pragma warning(pop)

using namespace geode::prelude;

struct HookedGameManager : Modify<HookedGameManager, GameManager> {

  static void onModify(auto& self);

  $override int countForType(IconType iconType);

  $override void reportAchievementWithID(char const*, int, bool);

};
