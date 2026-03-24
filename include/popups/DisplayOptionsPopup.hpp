#pragma once

#include <Geode/Geode.hpp>
class FilterAndSortPopup;

class DisplayOptionsPopup : public geode::Popup {
protected:

	cocos2d::CCMenu *m_buttonMenu;
	FilterAndSortPopup *parentPopup = nullptr;

	void toggleOption(cocos2d::CCObject *sender);

	void addOption(const char*, int);
	void addOptionInfo(int, const char*);
	bool init(FilterAndSortPopup *);

	void onClose(cocos2d::CCObject *) override;

public:
	static DisplayOptionsPopup* create(FilterAndSortPopup *);
};

