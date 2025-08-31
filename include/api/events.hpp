#pragma once

/*
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include <Geode/loader/Dispatch.hpp>
#include <Geode/loader/Mod.hpp>
#ifdef __clang__
#pragma clang diagnostic pop
#endif
*/

#include <api/base.hpp>

namespace nytelyte {
  namespace icon_kit_filter_and_sort {
    namespace events {
      using CompareVanillaIconsByNumber = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-number">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>
        ::type;

      using CompareVanillaIconsByLockStatus = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-lock-status">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>
        ::type;

      using CompareVanillaIconsByCategory = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-category">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>
        ::type;

      using CompareVanillaIconsByAuthor = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-author">
        ::takes<int, int, UnlockType>
        ::returns<std::optional<bool>>
        ::type;

      using CompareVanillaIcons = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons">
        ::takes<int, int, UnlockType>
        ::returns<bool>
        ::type;

      using RecalculateIconOrderAndTrackIcon = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-track-icon">
        ::takes<GJGarageLayer*, UnlockType, int>
        ::returns<bool>
        ::type;

      using VanillaIconDisplayToPosition = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/vanilla-icon-display-to-position">
        ::takes<UnlockType, int>
        ::returns<int>
        ::type;

      using VanillaIconPositionToDisplay = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/vanilla-icon-position-to-display">
        ::takes<UnlockType, int>
        ::returns<int>
        ::type;

      using GetActiveIconPage = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/get-active-icon-page">
        ::takes<UnlockType>
        ::returns<int>
        ::type;

      using GetVanillaIconsInOrder = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/get-vanilla-icons-in-order">
        ::takes<UnlockType>
        ::returns<std::vector<int>>
        ::type;

      using GetMoreIconsIconsInOrder = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/get-more-icons-icons-in-order">
        ::takes<UnlockType>
        ::returns<std::vector<std::string>>
        ::type;

      using RecalculateIconOrder = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order">
        ::type;

      using RecalculateIconOrderAndRemainOnSamePages = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-remain-on-same-pages">
        ::takes<GJGarageLayer*>
        ::type;

      using RecalculateIconOrderAndRemainOnSameSectionPages = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-remain-on-same-section-pages">
        ::takes<GJGarageLayer*>
        ::type;

      using RecalculateIconOrderAndGoToActiveIconPages = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-go-to-active-icon-pages">
        ::takes<GJGarageLayer*>
        ::type;

      using GiveIconAttention = nytelyte::event::GoodDispatchEvent
        ::ID<"nytelyte.icon_kit_filter_and_sort/give-icon-attention">
        ::takes<GJGarageLayer*, UnlockType, int>
        ::type;
    }
  }
}
