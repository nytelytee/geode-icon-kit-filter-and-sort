#pragma once

#include <Geode/loader/Dispatch.hpp>
#include <Geode/loader/Mod.hpp>
#include <api/base.hpp>

namespace nytelyte {
  namespace icon_kit_filter_and_sort {
    namespace events {

      using CompareVanillaIconsByNumber = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-number",
        std::optional<bool>,
        int, int, UnlockType
      >;
      using CompareVanillaIconsByLockStatus = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-lock-status",
        std::optional<bool>,
        int, int, UnlockType
      >;
      using CompareVanillaIconsByCategory = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-category",
        std::optional<bool>,
        int, int, UnlockType
      >;
      using CompareVanillaIconsByAuthor = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-author",
        std::optional<bool>,
        int, int, UnlockType
      >;
      using CompareVanillaIcons = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/compare-vanilla-icons-by-author",
        bool,
        int, int, UnlockType
      >;

      using RecalculateIconOrder = SimpleDispatchEvent<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order">;
      using RecalculateIconOrderAndRemainOnSamePages = SimpleDispatchEvent<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-remain-on-same-pages", GJGarageLayer*>;
      using RecalculateIconOrderAndRemainOnSameSectionPages = SimpleDispatchEvent<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-remain-on-same-section-pages", GJGarageLayer*>;
      using RecalculateIconOrderAndGoToActiveIconPages = SimpleDispatchEvent<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-go-to-active-icon-pages", GJGarageLayer*>;

      using RecalculateIconOrderAndTrackIcon = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/recalculate-icon-order-and-track-icon",
        bool,
        GJGarageLayer*, UnlockType, int
      >;
      using GiveIconAttention = SimpleDispatchEvent<"nytelyte.icon_kit_filter_and_sort/give-icon-attention", GJGarageLayer*, UnlockType, int>;
      
      using VanillaIconDisplayToPosition = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/vanilla-icon-display-to-position",
        int,
        UnlockType, int
      >;
      using VanillaIconPositionToDisplay = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/vanilla-icon-position-to-display",
        int,
        UnlockType, int
      >;
      
      using GetActiveIconPage = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/get-active-icon-page",
        int,
        UnlockType
      >;

      using GetVanillaIconsInOrder = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/get-vanilla-icons-in-order",
        std::vector<int>,
        UnlockType
      >;
      using GetMoreIconsIconsInOrder = SimpleDispatchEventWithResult<"nytelyte.icon_kit_filter_and_sort/get-more-icons-icons-in-order",
        std::vector<std::string>,
        UnlockType
      >;
    }
  }
}
