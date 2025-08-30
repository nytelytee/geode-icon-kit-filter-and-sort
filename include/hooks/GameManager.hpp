#pragma once

#include <Geode/Geode.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4265)  // non-virtual destructor
#endif

#include <Geode/modify/GameManager.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // MSVC

using namespace geode::prelude;

struct HookedGameManager : Modify<HookedGameManager, GameManager> {

  static void onModify(auto& self);

  $override int countForType(IconType iconType);

  $override void reportAchievementWithID(char const*, int, bool);

};
