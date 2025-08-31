#pragma once

#define ICON_KIT_FILTER_AND_SORT_ID "nytelyte.icon_kit_filter_and_sort"

namespace nytelyte {
  
  namespace icon_kit_filter_and_sort {
    // an icon, which may be either a vanilla icon, or a more icons icon
    class Icon {
      private:
        std::variant<int, std::string> m_icon;
      public:
        Icon(int icon) : m_icon(icon) {}
        Icon(std::string icon) : m_icon(icon) {}

        bool isVanillaIcon() { return std::holds_alternative<int>(m_icon); }
        bool isMoreIconsIcon() { return std::holds_alternative<std::string>(m_icon); }

        std::optional<int> getVanillaIcon() {
          if (isVanillaIcon()) return std::get<int>(m_icon);
          return std::nullopt;
        }

        std::optional<std::string> getMoreIconsIcon() {
          if (isMoreIconsIcon()) return std::get<std::string>(m_icon);
          return std::nullopt;
        }
    };
  }

}
