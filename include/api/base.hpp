#pragma once

#define ICON_KIT_FILTER_AND_SORT_ID "nytelyte.icon_kit_filter_and_sort"

namespace nytelyte {
  
  namespace detail {
    template<size_t N>
    struct StringLiteral {
      constexpr StringLiteral(const char (&str)[N]) {
          std::copy_n(str, N, value);
      }
      char value[N];
    };
  }
  
  // a dispatch event wrapper that only requires the id to be defined once, it can then be used on both the
  // posting side and listening side of things
  template<detail::StringLiteral EventID, class... Args>
  struct SimpleDispatchEvent {
    using Filter = geode::DispatchFilter<Args...>;
    using Event = Filter::Event;
    using Listener = geode::EventListener<Filter>;
    static constexpr const char* ID = EventID.value;
    
    static geode::ListenerResult post(Args... args) {
      return Event(SimpleDispatchEvent::ID, args...).post();
    }

    template <typename F> requires std::invocable<F, Args...>
    static Listener listen(F&& function) {
      return Listener(std::forward<F>(function), Filter(SimpleDispatchEvent::ID));
    }
    
    template<typename C>
    static Listener listen(C* cls, typename geode::to_member<C, geode::ListenerResult(Args...)>::value function) {
      return Listener(cls, function, Filter(SimpleDispatchEvent::ID));
    }
    
    template <typename F> requires std::invocable<F, Args...>
    static Listener* listenGlobal(F&& function) {
      return new Listener(SimpleDispatchEvent::listen(function));
    }
    
    template<typename C>
    static Listener* listenGlobal(C* cls, typename geode::to_member<C, geode::ListenerResult(Args...)>::value function) {
      return new Listener(SimpleDispatchEvent::listen(cls, function));
    }
    
  };
  
  // same thing as above, but listeners always return LitenerResult::Stop
  // and the return value is captured via a pointer and returned by the post method
  template<detail::StringLiteral EventID, class ResultT, class... Args>
  requires std::default_initializable<ResultT> &&
           std::assignable_from<ResultT&, ResultT> &&
           std::copy_constructible<ResultT> &&
           (!std::is_reference_v<ResultT>) &&
           (!std::is_abstract_v<ResultT>)
  struct SimpleDispatchEventWithResult {
    using Filter = geode::DispatchFilter<ResultT*, Args...>;
    using Event = Filter::Event;
    using Listener = geode::EventListener<Filter>;
    using Result = ResultT;
    static constexpr const char* ID = EventID.value;
    
    static ResultT post(Args... args) {
      ResultT into;
      Event(SimpleDispatchEventWithResult::ID, &into, args...).post();
      return into;
    }

    template <typename F>
    requires std::invocable<F, Args...> &&
             std::assignable_from<ResultT&, std::invoke_result_t<F, Args...>>
    static Listener listen(F&& function) {
        return Listener([func = std::forward<F>(function)](ResultT* into, Args... args) {
            *into = func(args...);
            return geode::ListenerResult::Stop;
        }, Filter(SimpleDispatchEventWithResult::ID));
    }

    template<typename C>
    requires std::assignable_from<ResultT&, std::invoke_result_t<std::invoke_result_t<typename geode::to_member<C, ResultT(Args...)>::value, C*, Args...>>>
    static Listener listen(C* cls, typename geode::to_member<C, ResultT(Args...)>::value function) {
      return Listener([cls, function](ResultT* into, Args... args) {
        *into = (cls->*function)(args...);
        return geode::ListenerResult::Stop;
      }, Filter(SimpleDispatchEventWithResult::ID));
    }
    
    template <typename F> requires std::invocable<F, Args...>
    static Listener* listenGlobal(F&& function) {
      return new Listener(SimpleDispatchEventWithResult::listen(function));
    }
    
    template<typename C>
    static Listener* listenGlobal(C* cls, typename geode::to_member<C, geode::ListenerResult(Args...)>::value function) {
      return new Listener(SimpleDispatchEventWithResult::listen(cls, function));
    }

  };
  
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
