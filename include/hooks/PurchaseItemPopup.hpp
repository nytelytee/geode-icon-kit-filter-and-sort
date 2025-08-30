#pragma once

#include <Geode/Geode.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4265)  // non-virtual destructor
#endif

#include <Geode/modify/PurchaseItemPopup.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif // MSVC

#include <iconkit.hpp>
#include <logic.hpp>

using namespace geode::prelude;

struct HookedPurchaseItemPopup : Modify<HookedPurchaseItemPopup, PurchaseItemPopup> {
  
  static void onModify(auto&);
  
  $override void onPurchase(CCObject* sender);

};
