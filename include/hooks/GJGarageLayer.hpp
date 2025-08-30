#pragma once

#include <Geode/Geode.hpp>

#pragma warning(push)
#pragma warning(disable: 4265)  // non-virtual destructor
#include <Geode/modify/GJGarageLayer.hpp>
#pragma warning(pop)

using namespace geode::prelude;

struct HookedGJGarageLayer : Modify<HookedGJGarageLayer, GJGarageLayer> {

  static void onModify(auto&);

  $override void onArrow(CCObject*);
  
  $override void onNavigate(CCObject*);

  $override void selectTab(IconType);
  
  $override CCArray* getItems(int, int, IconType, int);
  
  $override void setupPage(int, IconType);
  
  $override bool init() override;

  void toggleNavigationMenus(bool, bool);
  void defaultToggleNavigationMenus(IconType);
  void recalculateNavdotMenu(int, IconType);
  void onFilterAndSort(CCObject*);
  void createFilterAndSortButton();

};


// remove this and depend on misc bugfixes, if cvolton merges my pr
struct OtherHookedGJGarageLayer : Modify<OtherHookedGJGarageLayer, GJGarageLayer> {
  static void onModify(auto& self) {
    // this is a reimplementation to fix a bug with robtop improperly calculating the pages, make sure it runs last
    // also make it run after more icons, which uses Priority::Replace and reimplements
    // (i.e. make it not run at all if more icons is installed, since MI also does this fix)
    Result<> result = self.setHookPriority("GJGarageLayer::onArrow", Priority::Replace + 1);
    if (!result) log::error("Failed to set hook priority, arrows may not work correctly.");
  }

  $override void onArrow(CCObject *sender) {
    CCMenuItemSpriteExtra *arrow = static_cast<CCMenuItemSpriteExtra *>(sender);
    // robtop forgot to subtract 1 here, that is the only change.
    // if the icon count is, say, 36, robtop would just divide by 36,
    // and get that the max page is 1, when, in fact, 36 icons fit onto page 0
    // so there would be a page 1 that is just blank
    int maxPage = (GameManager::get()->countForType(m_iconType) - 1)/36;
    int page = m_iconPages[m_iconType] + arrow->getTag();
    page = page > maxPage ? 0 : page < 0 ? maxPage : page;
    GJGarageLayer::setupPage(page, m_iconType);
  }
};
