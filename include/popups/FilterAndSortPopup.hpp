#pragma once

#include <Geode/Geode.hpp>

class FilterPopup;
class SortPopup;
class DisplayOptionsPopup;

class FilterAndSortPopup : public geode::Popup {
protected:

	friend FilterPopup;
	friend SortPopup;
	friend DisplayOptionsPopup;

	cocos2d::CCMenu *m_buttonMenu;
	cocos2d::CCMenu *m_topMenu;
	cocos2d::CCMenu *m_actionMenu;

	bool init() override;

	void onToggle(cocos2d::CCObject *);
	void onMenuButton(cocos2d::CCObject *);
	void onActionButton(cocos2d::CCObject *);
	void refreshMenuState();

	void onClose(cocos2d::CCObject*) override;

public:
	static FilterAndSortPopup* create();

};
