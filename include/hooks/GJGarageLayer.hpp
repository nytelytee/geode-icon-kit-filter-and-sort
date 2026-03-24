#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

struct HookedGJGarageLayer : geode::Modify<HookedGJGarageLayer, GJGarageLayer> {

	static void onModify(auto&);

	$override void onArrow(cocos2d::CCObject*);

	$override void onNavigate(cocos2d::CCObject*);

	$override void selectTab(IconType);

	$override cocos2d::CCArray* getItems(int, int, IconType, int);

	$override void setupPage(int, IconType);

	$override bool init() override;

	void toggleNavigationMenus(bool, bool);
	void defaultToggleNavigationMenus(IconType);
	void recalculateNavdotMenu(int, IconType);
	void onFilterAndSort(CCObject*);
	void createFilterAndSortButton();

};


// BUGFIX
struct OtherHookedGJGarageLayer : geode::Modify<OtherHookedGJGarageLayer, GJGarageLayer> {
	static void onModify(auto& self);

	$override void onArrow(cocos2d::CCObject *sender);

};
