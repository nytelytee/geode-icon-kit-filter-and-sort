#pragma once

#include <Geode/Geode.hpp>

#pragma warning(push)
#pragma warning(disable: 4265)  // non-virtual destructor
#include <Geode/modify/PurchaseItemPopup.hpp>
#pragma warning(pop)

#include <iconkit.hpp>
#include <logic.hpp>

using namespace geode::prelude;

struct HookedPurchaseItemPopup : Modify<HookedPurchaseItemPopup, PurchaseItemPopup> {
  
  static void onModify(auto&);
  
  $override void onPurchase(CCObject* sender);

};
