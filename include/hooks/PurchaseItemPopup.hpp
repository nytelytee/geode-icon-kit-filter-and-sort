#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PurchaseItemPopup.hpp>

#include <iconkit.hpp>
#include <logic.hpp>

using namespace geode::prelude;

struct HookedPurchaseItemPopup : Modify<HookedPurchaseItemPopup, PurchaseItemPopup> {
  
  static void onModify(auto&);
  
  $override void onPurchase(CCObject* sender);

};
