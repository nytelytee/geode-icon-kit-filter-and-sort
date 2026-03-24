#pragma once

#include <Geode/Geode.hpp>

class FilterAndSortPopup;

class SortPopup : public geode::Popup {
protected:

	cocos2d::CCNode *m_sortingList;
	FilterAndSortPopup *parentPopup;

	static const GLubyte EVEN_BACKGROUND_OPACITY = 32;
	static const GLubyte ODD_BACKGROUND_OPACITY = 16;

	bool init(FilterAndSortPopup *);

	void onArrow(cocos2d::CCObject *);
	void onReverseSortOrder(cocos2d::CCObject *);
	void onToggle(cocos2d::CCObject *);

	void onClose(cocos2d::CCObject *) override;

	void recalculateListItemBackgrounds();

public:
	static SortPopup* create(FilterAndSortPopup *);

};
