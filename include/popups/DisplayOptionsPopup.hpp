#pragma once

#include <Geode/Geode.hpp>
using namespace geode::prelude;
class FilterAndSortPopup;


class DisplayOptionsPopup : public Popup<FilterAndSortPopup*> {
protected:

  CCMenu *m_buttonMenu;
  FilterAndSortPopup *parentPopup = nullptr;
  
  void toggleOption(CCObject *sender);

  void addOption(const char*, int);
  void addOptionInfo(int, const char*);
  bool setup(FilterAndSortPopup *) override;

  void onClose(CCObject *) override;

public:
  static DisplayOptionsPopup* create(FilterAndSortPopup *);
};

