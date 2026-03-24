#pragma once

#include <Geode/Geode.hpp>
#include <popups/FilterPopup.hpp>

class AuthorFilterPopup : public FilterPopup {
protected:

	void toggleAuthor(cocos2d::CCObject *);
	void goToAuthorProfile(cocos2d::CCObject *);
	void addAuthor(int, int);

	void onActionMenuButton(cocos2d::CCObject *) override;
	void customSetup() override;

public:
	static AuthorFilterPopup* create(FilterAndSortPopup *parent);
};
