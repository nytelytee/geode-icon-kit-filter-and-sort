#pragma once

#include <Geode/Geode.hpp>

class FilterPopup;
class SortPopup;
class DisplayOptionsPopup;

using namespace geode::prelude;

class FilterAndSortPopup : public Popup {
protected:

  friend FilterPopup;
  friend SortPopup;
  friend DisplayOptionsPopup;

  CCMenu *m_buttonMenu;
  CCMenu *m_topMenu;
  CCMenu *m_actionMenu;

  bool init() override;

  void onToggle(CCObject *);
  void onMenuButton(CCObject *);
  void onActionButton(CCObject *);
  void refreshMenuState();

  void onClose(CCObject*) override;

public:
  static FilterAndSortPopup* create();

};
