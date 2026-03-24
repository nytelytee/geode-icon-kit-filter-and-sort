#pragma once

#include <Geode/Geode.hpp>
#include <utils/BoundCCMenu.hpp>

class FilterAndSortPopup;

class FilterPopup : public geode::Popup {
protected:

	cocos2d::CCMenu *m_actionButtonMenu;
	BoundCCMenu *m_buttonMenu;
	geode::ScrollLayer *m_scrollLayer;
	cocos2d::CCSize m_scrollLayerSize;
	cocos2d::CCSize m_buttonMenuSize;
	FilterAndSortPopup *parentPopup;

	virtual void onActionMenuButton(cocos2d::CCObject *) = 0;
	void onClose(cocos2d::CCObject *) override;

	bool init(float, float, FilterAndSortPopup *);

	void preCustomSetup();
	virtual void customSetup() = 0;
	void postCustomSetup();

};
