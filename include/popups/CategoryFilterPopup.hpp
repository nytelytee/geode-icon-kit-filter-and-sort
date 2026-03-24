#pragma once

#include <Geode/Geode.hpp>
#include <popups/FilterPopup.hpp>

class CategoryFilterPopup : public FilterPopup {
protected:

	void toggleCategory(cocos2d::CCObject *sender);
	void addCategory(int, std::string);

	void onActionMenuButton(cocos2d::CCObject *) override;
	void customSetup() override;

public:
	static CategoryFilterPopup* create(FilterAndSortPopup *);
};
