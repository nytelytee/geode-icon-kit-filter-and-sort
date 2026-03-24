#pragma once

#include <Geode/Geode.hpp>

class BoundCCMenu : public cocos2d::CCMenu {
protected:
	cocos2d::CCRect m_bound;

	bool initWithRect(cocos2d::CCRect worldSpaceBound) { m_bound = worldSpaceBound; return cocos2d::CCMenu::init(); }

	bool ccTouchBegan(cocos2d::CCTouch *, cocos2d::CCEvent *) override;
	void ccTouchEnded(cocos2d::CCTouch *, cocos2d::CCEvent *) override;
	void ccTouchMoved(cocos2d::CCTouch *, cocos2d::CCEvent *) override;

public:
	cocos2d::CCRect getBound() const { return m_bound; }
	void setBound(cocos2d::CCRect newBound) { m_bound = newBound; }

	static BoundCCMenu *create(cocos2d::CCRect worldSpaceBound) {
		auto a = new BoundCCMenu();
		if (a && a->initWithRect(worldSpaceBound)){ a->autorelease(); return a; }
		delete a; return nullptr;
	}
};
