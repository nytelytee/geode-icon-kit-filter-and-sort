#pragma once

#include "easy-event.hpp"
#include "base.hpp"

namespace nytelyte {
  namespace icon_kit_filter_and_sort {
    namespace events {
      using EasyEvent = nytelyte::easy_event::EasyEvent;

      using CompareVanillaIconsByNumber = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-number">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>;

      using CompareVanillaIconsByLockStatus = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-lock-status">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>;

      using CompareVanillaIconsByCategory = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-category">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>;

      using CompareVanillaIconsByAuthor = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-author">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>;

      using CompareVanillaIcons = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons">
        ::takes<int, int, UnlockType>
        ::returns<bool>;

      using RecalculateIconOrderAndTrackIcon = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-track-icon">
        ::takes<GJGarageLayer*, UnlockType, int>
        ::returns<bool>;

      using VanillaIconDisplayToPosition = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/vanilla-icon-display-to-position">
        ::takes<UnlockType, int>
        ::returns<int>;

      using VanillaIconPositionToDisplay = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/vanilla-icon-position-to-display">
        ::takes<UnlockType, int>
        ::returns<int>;

      using GetActiveIconPage = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/get-active-icon-page">
        ::takes<UnlockType>
        ::returns<int>;

      using GetVanillaIconsInOrder = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/get-vanilla-icons-in-order">
        ::takes<UnlockType>
        ::returns<std::vector<int>>;

      using GetMoreIconsIconsInOrder = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/get-more-icons-icons-in-order">
        ::takes<UnlockType>
        ::returns<std::vector<std::string>>;

      using RecalculateIconOrder = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order">;

      using RecalculateIconOrderAndRemainOnSamePages = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-remain-on-same-pages">
        ::takes<GJGarageLayer*>;

      using RecalculateIconOrderAndRemainOnSameSectionPages = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-remain-on-same-section-pages">
        ::takes<GJGarageLayer*>;

      using RecalculateIconOrderAndGoToActiveIconPages = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-go-to-active-icon-pages">
        ::takes<GJGarageLayer*>;

      using GiveIconAttention = EasyEvent
        ::withID<"nytelyte.icon_kit_filter_and_sort/give-icon-attention">
        ::takes<GJGarageLayer*, UnlockType, int>;
    }
  }
}
