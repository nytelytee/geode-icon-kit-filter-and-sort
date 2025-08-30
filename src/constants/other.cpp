#include<constants.hpp>

std::unordered_map<UnlockType, int> VANILLA_MAX_ICONS;

std::unordered_map<IconType, UnlockType> ICON_TO_UNLOCK = {
  {IconType::Cube,    UnlockType::Cube},
  {IconType::Ship,    UnlockType::Ship},
  {IconType::Ball,    UnlockType::Ball},
  {IconType::Ufo,     UnlockType::Bird},
  {IconType::Wave,    UnlockType::Dart},
  {IconType::Robot,   UnlockType::Robot},
  {IconType::Spider,  UnlockType::Spider},
  {IconType::Swing,   UnlockType::Swing},
  {IconType::Jetpack, UnlockType::Jetpack},
  
  {IconType::DeathEffect, UnlockType::Death},
  {IconType::Special, UnlockType::Streak},
  {IconType::Item, UnlockType::GJItem},
  {IconType::ShipFire, UnlockType::ShipFire},
};

std::unordered_map<UnlockType, IconType> UNLOCK_TO_ICON = {
  {UnlockType::Cube,    IconType::Cube},
  {UnlockType::Ship,    IconType::Ship},
  {UnlockType::Ball,    IconType::Ball},
  {UnlockType::Bird,    IconType::Ufo},
  {UnlockType::Dart,    IconType::Wave},
  {UnlockType::Robot,   IconType::Robot},
  {UnlockType::Spider,  IconType::Spider},
  {UnlockType::Swing,   IconType::Swing},
  {UnlockType::Jetpack, IconType::Jetpack},
  
  {UnlockType::Death, IconType::DeathEffect},
  {UnlockType::Streak, IconType::Special},
  {UnlockType::GJItem, IconType::Item},
  {UnlockType::ShipFire, IconType::ShipFire},
};

std::vector<IconType> ICON_TYPES_TO_CHANGE = {
    IconType::Cube, IconType::Ship, IconType::Ball,
    IconType::Ufo, IconType::Wave, IconType::Robot,
    IconType::Spider, IconType::Swing, IconType::Jetpack
};

std::vector<UnlockType> UNLOCK_TYPES_TO_CHANGE = {
    UnlockType::Cube, UnlockType::Ship, UnlockType::Ball,
    UnlockType::Bird, UnlockType::Dart, UnlockType::Robot,
    UnlockType::Spider, UnlockType::Swing, UnlockType::Jetpack
};

std::unordered_map<UnlockType, std::string> ICON_NAMES = {
  {UnlockType::Cube,    "Cube"},
  {UnlockType::Ship,    "Ship"},
  {UnlockType::Ball,    "Ball"},
  {UnlockType::Bird,    "Bird"},
  {UnlockType::Dart,    "Dart"},
  {UnlockType::Robot,   "Robot"},
  {UnlockType::Spider,  "Spider"},
  {UnlockType::Swing,   "Swing"},
  {UnlockType::Jetpack, "Jetpack"},
  {UnlockType::Streak,  "Streak"},
  {UnlockType::Death,   "Death"},
  {UnlockType::GJItem,  "GJItem"},
  {UnlockType::ShipFire,"ShipFire"},
  {UnlockType::Col1,    "Color 1"},
  {UnlockType::Col2,    "Color 2"},
  {static_cast<UnlockType>(0), "NULL"},
};

std::unordered_map<SortType, std::string> SORT_TYPE_NAMES = {
  {SortType::Number,     "Number"},
  {SortType::LockStatus, "Lock Status"},
  {SortType::Category,   "Category"},
  {SortType::Author,     "Author"},
};
