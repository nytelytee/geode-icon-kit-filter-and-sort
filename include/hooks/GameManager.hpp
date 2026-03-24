#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GameManager.hpp>

struct HookedGameManager : geode::Modify<HookedGameManager, GameManager> {

	static void onModify(auto& self);

	$override int countForType(IconType iconType);

	$override void reportAchievementWithID(char const*, int, bool);

};
