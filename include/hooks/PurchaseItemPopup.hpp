#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PurchaseItemPopup.hpp>

#include <iconkit.hpp>
#include <logic.hpp>

struct HookedPurchaseItemPopup : geode::Modify<HookedPurchaseItemPopup, PurchaseItemPopup> {

	static void onModify(auto&);

	$override void onPurchase(cocos2d::CCObject* sender);

};
