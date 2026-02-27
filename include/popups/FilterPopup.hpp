#pragma once

#include <Geode/Geode.hpp>
#include <utils/BoundCCMenu.hpp>

class FilterAndSortPopup;

using namespace geode::prelude;

class FilterPopup : public Popup {
protected:

  CCMenu *m_actionButtonMenu;
  BoundCCMenu *m_buttonMenu;
  ScrollLayer *m_scrollLayer;
  CCSize m_scrollLayerSize;
  CCSize m_buttonMenuSize;
  FilterAndSortPopup *parentPopup;

  virtual void onActionMenuButton(CCObject *) = 0;
  void onClose(CCObject *) override;

  bool init(float, float, FilterAndSortPopup *);

  void preCustomSetup();
  virtual void customSetup() = 0;
  void postCustomSetup();

};
