#pragma once

#include <Geode/Geode.hpp>

// this can only be used for really really simple menus
// i did not bother to actually make it retain any of the
// links either, so if something that's linked gets freed
// and the loops in ccTouchWhatever use it, you will
// be acessing invalid memory.
// this doesn't concern my use case because, in places i
// use this, it gets freed together with the children

class LinkedCCMenu : public cocos2d::CCMenu {
protected:
	std::list<cocos2d::CCMenu *> m_links;
	cocos2d::CCMenu *m_touching = nullptr;

	bool ccTouchBegan(cocos2d::CCTouch *, cocos2d::CCEvent *) override;
	void ccTouchEnded(cocos2d::CCTouch *, cocos2d::CCEvent *) override;
	void ccTouchMoved(cocos2d::CCTouch *, cocos2d::CCEvent *) override;
	void ccTouchCancelled(cocos2d::CCTouch *, cocos2d::CCEvent *) override;

public:

	size_t size() { return m_links.size(); }

	void sortAndUpdatePriority();

	bool link(cocos2d::CCMenu *);
	bool unlink(cocos2d::CCMenu *);
	bool linkChildren(cocos2d::CCNode *, int = 1);
	bool unlinkChildren(cocos2d::CCNode *, int = 1);

	static LinkedCCMenu * create();
	static LinkedCCMenu * createLinked(cocos2d::CCNode *, int = 1);

};
