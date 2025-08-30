#pragma once

#include <Geode/Geode.hpp>
#include <utils/BoundCCMenu.hpp>

class FilterAndSortPopup;

using namespace geode::prelude;

class FilterPopup : public Popup<FilterAndSortPopup *> {
protected:

  CCMenu *m_actionButtonMenu;
  BoundCCMenu *m_buttonMenu;
  ScrollLayer *m_scrollLayer;
  CCSize m_scrollLayerSize;
  CCSize m_buttonMenuSize;
  FilterAndSortPopup *parentPopup;

  virtual void onTopMenuButton(CCObject *) = 0;
  void onClose(CCObject *) override;

  bool setup(FilterAndSortPopup *parent) override { parentPopup = parent; preCustomSetup(); customSetup(); postCustomSetup(); return true; }

  void preCustomSetup();
  virtual void customSetup() = 0;
  void postCustomSetup();

};
