#include <Geode/Geode.hpp>
#include <algorithm>

#include <constants.hpp>
#include <iconkit.hpp>
#include <popups/DisplayOptionsPopup.hpp>
#include <popups/FilterAndSortPopup.hpp>
#include <utils/LinkedCCMenu.hpp>
#include <utils/CCMenuItemTogglerSpoof.hpp>


using namespace geode::prelude;

void DisplayOptionsPopup::onClose(CCObject *sender) {
  Popup<FilterAndSortPopup *>::onClose(sender);
  if (parentPopup) parentPopup->refreshMenuState();
};

enum DisplayOption { ShowDenied, SeparateAcceptedFromDenied, DontRecolorNavigationButtons, NoGapBetweenAcceptedAndDenied, HideNavigationMenuOnSinglePage };

void DisplayOptionsPopup::toggleOption(CCObject *sender) {
  CCMenuItemToggler *toggler = static_cast<CCMenuItemToggler *>(sender);
  int tag = toggler->getTag();
  switch (tag) {
    case DisplayOption::ShowDenied:
      iconKitState.pendingSettings.showDenied = !toggler->isToggled(); break;
    case DisplayOption::SeparateAcceptedFromDenied:
      iconKitState.pendingSettings.separateAcceptedFromDenied = !toggler->isToggled(); break;
    case DisplayOption::DontRecolorNavigationButtons:
      iconKitState.pendingSettings.dontRecolorNavigationButtons = !toggler->isToggled(); break;
    case DisplayOption::NoGapBetweenAcceptedAndDenied:
      iconKitState.pendingSettings.noGapBetweenAcceptedAndDenied = !toggler->isToggled(); break;
    case DisplayOption::HideNavigationMenuOnSinglePage:
      iconKitState.pendingSettings.hideNavigationMenuOnSinglePage = !toggler->isToggled(); break;
  }
}

void DisplayOptionsPopup::addOption(const char* name, int option) {

  CCMenuItemToggler *toggler = createTogglerWithStandardSpritesSpoofOn(this, menu_selector(DisplayOptionsPopup::toggleOption));
  toggler->setTag(option);
  toggler->setScale(0.8f);
  toggler->setLayoutOptions(AxisLayoutOptions::create()->setAutoScale(false));

  CCLabelBMFont* label = CCLabelBMFont::create(name, "bigFont.fnt");
  float maxScale = 0.375f;
  if (label->getContentSize().width * maxScale > (m_size.width - 2*HORIZONTAL_BORDER_SIZE - 40 - toggler->getContentSize().width/0.8f)) {
    maxScale = (m_size.width - 2*HORIZONTAL_BORDER_SIZE - 40 - toggler->getContentSize().width/0.8f) / label->getContentSize().width;
  }
  label->setLayoutOptions(AxisLayoutOptions::create()->setBreakLine(true)->setSameLine(true)->setScaleLimits({}, maxScale));
  
  switch (option) {
    case DisplayOption::ShowDenied:
      toggler->toggle(iconKitState.pendingSettings.showDenied); break;
    case DisplayOption::SeparateAcceptedFromDenied:
      toggler->toggle(iconKitState.pendingSettings.separateAcceptedFromDenied); break;
    case DisplayOption::DontRecolorNavigationButtons:
      toggler->toggle(iconKitState.pendingSettings.dontRecolorNavigationButtons); break;
    case DisplayOption::NoGapBetweenAcceptedAndDenied:
      toggler->toggle(iconKitState.pendingSettings.noGapBetweenAcceptedAndDenied); break;
    case DisplayOption::HideNavigationMenuOnSinglePage:
      toggler->toggle(iconKitState.pendingSettings.hideNavigationMenuOnSinglePage); break;
  }

  m_buttonMenu->addChild(toggler);
  m_buttonMenu->addChild(label);
}

void DisplayOptionsPopup::addOptionInfo(int tag, const char* description) {
  InfoAlertButton *infoButton = InfoAlertButton::create("Info", description, 0.5f);
  CCMenuItemToggler *button = static_cast<CCMenuItemToggler *>(m_buttonMenu->getChildByTag(tag));
  auto [x, y] = button->getScaledContentSize() / 2;
  auto [bx, by] = infoButton->getContentSize() / 2;
  infoButton->setPosition(button->getPosition() - CCPoint{x, -y} - CCPoint{bx, -by});
  m_buttonMenu->addChild(infoButton);
}

bool DisplayOptionsPopup::setup(FilterAndSortPopup *parent) {
  parentPopup = parent;

  this->setTitle("Icon Kit Display Options");
  
  float titleMargin = m_bgSprite->boundingBox().getMaxY() - m_title->boundingBox().getMaxY() - VERTICAL_BORDER_SIZE;
  float titleHeight = m_title->getScale()*m_title->getContentSize().height;
  float realTitleHeight = 2*titleMargin + titleHeight;
  float separatorHeight = 1;

  CCLayerColor* separator = CCLayerColor::create({ 0, 0, 0, 50 }, m_size.width - 2*HORIZONTAL_BORDER_SIZE, separatorHeight);
  separator->setPosition(HORIZONTAL_BORDER_SIZE, m_title->boundingBox().getMinY() - titleMargin);

  m_buttonMenu = CCMenu::create();
  m_buttonMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setGrowCrossAxis(true)->setAutoScale(true)->setGap(10.f));
  m_buttonMenu->setContentSize({m_size.width - 2*HORIZONTAL_BORDER_SIZE - 40, 0});

  m_buttonMenu->setPosition(m_size/2 - CCPoint{0, realTitleHeight/2});

  addOption("Show Denied Icons", DisplayOption::ShowDenied);
  addOption("Separate Accepted\nFrom Denied", DisplayOption::SeparateAcceptedFromDenied);
  addOption("Don't Recolor\nNavigation Buttons", DisplayOption::DontRecolorNavigationButtons);
  addOption("No Gap Between Accepted\nand Denied Navigation Buttons", DisplayOption::NoGapBetweenAcceptedAndDenied);
  addOption("Hide Navigation Menu\non Single Page", DisplayOption::HideNavigationMenuOnSinglePage);

  m_buttonMenu->updateLayout();
  m_buttonMenu->setLayout(nullptr);

  float topTogglerMax = m_buttonMenu->getChildByIndex<InfoAlertButton>(0)->boundingBox().getMaxY();
  float topInfoButtonMax;

  addOptionInfo(
    DisplayOption::ShowDenied,
    "Whether <cr>denied</c> icons will be shown after <cg>accepted</c> icons. If off, <cr>denied</c> icons are completely hidden."
  );

  topInfoButtonMax = m_buttonMenu->getChildByIndex<InfoAlertButton>(-1)->boundingBox().getMaxY();

  addOptionInfo(
    DisplayOption::SeparateAcceptedFromDenied,
    "Whether the <cr>denied</c> icons will start showing up on a separate page."
  );

  addOptionInfo(
    DisplayOption::DontRecolorNavigationButtons,
    "Whether the navigation button (page) colors will be changed to\nshow which icons the current page contains.\n"
    "<cj>Blue</c> - only <cg>accepted</c> icons\n<cr>Red</c> - only <cr>denied</c> icons\n<cp>Magenta</c> - both <cg>accepted</c> and <cr>denied</c> icons\n"
    "If off, the current page color is always <cj>blue</c>."
  );

  addOptionInfo(
    DisplayOption::NoGapBetweenAcceptedAndDenied,
    "If <cb>Separate Accepted From Denied</c> is active, there is a gap between <cg>accepted</c> and <cr>denied</c> pages. This removes the gap."
  );
  
  addOptionInfo(
    DisplayOption::HideNavigationMenuOnSinglePage,
    "In vanilla, if there is only one page, the navigation menu is not shown.\n"
    "Since this mod adds extra information to it, it makes sense to show it anyway.\n"
    "Toggle to revert to vanilla behavior.\n"
    "Note: This does not affect the special and death effect pages, the menu is always hidden on one page there." 
  );
  
  m_buttonMenu->setPositionY(m_buttonMenu->getPositionY() - (topInfoButtonMax - topTogglerMax)/2);
   
  m_mainLayer->addChild(m_buttonMenu);
  m_mainLayer->addChild(separator);

  m_mainLayer->addChild(LinkedCCMenu::createLinked(m_mainLayer));

  return true;
}

DisplayOptionsPopup* DisplayOptionsPopup::create(FilterAndSortPopup *parent) {
    auto ret = new DisplayOptionsPopup();
    if (ret && ret->initAnchored(260.f, 240.f, parent)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
