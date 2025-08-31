#pragma once

#include <Geode/loader/Dispatch.hpp>
#include <type_traits>
#include <functional>

#define ICON_KIT_FILTER_AND_SORT_ID "nytelyte.icon_kit_filter_and_sort"

namespace nytelyte::event {
  
  namespace internal {

    template<size_t N>
    struct StringLiteral {
      constexpr StringLiteral(const char (&str)[N]) {
          std::copy_n(str, N, value);
      }
      char value[N];
    };

    template<typename T>
    struct is_unset : std::false_type {};

    struct unset_t {};
    template<>
    struct is_unset<unset_t> : std::true_type {};

    template <typename Class, typename ReturnType, typename... EventParameters>
    using MemberFunction = typename geode::to_member<Class, ReturnType(EventParameters...)>::value;
    
    template<typename Function, typename... EventParameters>
    concept FunctionHasValidArguments = std::invocable<Function, EventParameters...>;
    
    template<typename Function, typename ReturnType, typename... EventParameters>
    concept FunctionHasValidReturnType = std::assignable_from<ReturnType&, std::invoke_result_t<Function, EventParameters...>>;

    template <typename Function, typename ReturnType, typename... EventParameters>
    concept IsValidFunction = FunctionHasValidArguments<Function, EventParameters...> && FunctionHasValidReturnType<Function, ReturnType, EventParameters...>;

    template<typename Class, typename ReturnType, typename... EventParameters>
    concept MemberFunctionHasValidReturnType = std::assignable_from<ReturnType&, std::invoke_result_t<MemberFunction<Class, ReturnType>, Class*, EventParameters...>>;
    
    template <typename Class, typename ReturnType, typename... EventParameters>
    concept IsValidMemberFunction = MemberFunctionHasValidReturnType<Class, ReturnType, EventParameters...>;

  }

  template<typename EventID, typename EventParameters, typename EventReturn>
  struct EventImpl;

  template<typename EventID = internal::unset_t, typename EventParameters = internal::unset_t, typename EventReturn = internal::unset_t>
  struct EventBuilder {
      template<internal::StringLiteral Id>
      using ID = std::enable_if_t<
        std::is_same_v<EventID, internal::unset_t>,
        EventBuilder<std::integral_constant<const char*, Id.value>, EventParameters, EventReturn>
      >;
      
      template<typename... Parameters>
      using takes = std::enable_if_t<
        std::is_same_v<EventParameters, internal::unset_t>,
        EventBuilder<EventID, std::tuple<Parameters...>, EventReturn>
      >;
      
      template<typename Return>
      using returns = std::enable_if_t<
        std::is_same_v<EventReturn, internal::unset_t>,
        EventBuilder<EventID, EventParameters, Return>
      >;
      
      using type = EventImpl<
        std::conditional_t<internal::is_unset<EventID>::value, void, EventID>,
        std::conditional_t<internal::is_unset<EventParameters>::value, std::tuple<>, EventParameters>,
        std::conditional_t<internal::is_unset<EventReturn>::value, void, EventReturn>
      >;
  };


  template<typename EventID, typename EventParameters, typename EventReturn>
  struct EventImpl {};

  template<typename EventID, typename... EventParameters, typename EventReturn>
  struct EventImpl<EventID, std::tuple<EventParameters...>, EventReturn> {
  private:

  public:
    using Filter = std::conditional_t<
        std::is_void_v<EventReturn>,
        geode::DispatchFilter<EventParameters...>,
        geode::DispatchFilter<EventReturn*, EventParameters...>
    >;
    using Event = Filter::Event;
    using Listener = geode::EventListener<Filter>;
    static constexpr const char* ID = std::is_void_v<EventID> ? nullptr : EventID::value;
    
    // posting, the return value is geode::ListenerResult
    // use if you either care about the listener result, or just want to fire and forget
    //   MyEvent::postWithID(const char* id, EventParameters... args)
    //     posts the event with the given id and returns the listener result
    //   MyEvent::post(EventParameters... args)
    //     only if the id has been provided at compile time
    //     acts the same as MyEvent::postWithID(MyEvent::ID, args...);
    //   MyEvent::post(const char* id, EventParameters... args)
    //     only if the id has not been provided at compile time
    //     acts the same as MyEvent::postWithID(id, args...)

    // receiving, the return value is the return type specified
    // use if you don't care about the listener result, but do care about the return value
    //   MyEvent::receiveWithID(const char* id, EventParameters... args)
    //     receives the value from an event with the provided id and returns it
    //   MyEvent::receive(EventParameters... args)
    //   MyEvent::receive(const char* id, EventParameters... args)
    //     act the same as their conterparts in posting
    
    // receiving both, the return value is a pair of the listener result and the return type specified
    // use if you care about both of these things for whatever reason
    //   MyEvent::receiveBothWithID(const char* id, EventParameters... args)
    //     receives the value from an event with the provided id and returns the listener result of the posting of the event and the return value
    //   MyEvent::receiveBoth(EventParameters... args)
    //   MyEvent::receiveBoth(const char* id, EventParameters... args)
    //     act the same as their conterparts in posting

    static geode::ListenerResult postWithID(const char* id, EventParameters... args) {
      if constexpr (std::is_void_v<EventReturn>) {
        return Event(id, args...).post();
      } else {
        // this needs to be default constructible ofc
        EventReturn into;
        return Event(id, &into, args...).post();
      }
    }

    static geode::ListenerResult post(const char* id, EventParameters... args) requires std::is_void_v<EventID> { return postWithID(id, args...); }
    static geode::ListenerResult post(EventParameters... args) requires !std::is_void_v<EventID> { return postWithID(EventImpl::ID, args...); }

    static EventReturn receiveWithID(const char* id, EventParameters... args) requires !std::is_void_v<EventReturn> {
      EventReturn into;
      Event(id, &into, args...).post();
      return into;
    }
    static EventReturn receive(const char* id, EventParameters... args) requires std::is_void_v<EventID> { return receiveWithID(id, args...); }
    static EventReturn receive(EventParameters... args) requires !std::is_void_v<EventID> { return receiveWithID(EventImpl::ID, args...); }

    static std::pair<geode::ListenerResult, EventReturn> receiveBothWithID(const char* id, EventParameters... args) requires !std::is_void_v<EventReturn> {
      EventReturn into;
      geode::ListenerResult result = Event(id, &into, args...).post();
      return std::pair{result, into};
    }
    static std::pair<geode::ListenerResult, EventReturn> receiveBoth(const char* id, EventParameters... args) requires std::is_void_v<EventID> { return receiveBothWithID(id, args...); }
    static std::pair<geode::ListenerResult, EventReturn> receiveBoth(EventParameters... args) requires !std::is_void_v<EventID> { return receiveBothWithID(EventImpl::ID, args...); }
    
    
    // listening and sending, will always return the internal geode dispatch event listener that gets created
    //
    // MyEvent::listenWithID(const char* id, geode::ListenerResult(EventParameters...) function)
    //   creates an event listener with the given id and the function, and returns it
    //   note that if the defined event has a return value it will be default constructed when posting/receiving/receiving both
    //   you just don't have to care about it with this method
    //
    // MyEvent::sendWithID(const char* id, geode::ListenerResult result, EventReturn(EventParameters...) function)
    //   only if the event has a return type
    //   creates an event listener with the given id and the function, and returns it
    //   you are expected to return the object which the receive/receiveBoth methods will receive
    //   the actual event then returns result
    //   useful if you know you will always pick one result type, and just care about the thing you are returning
    //
    // MyEvent::sendBothWithID(const char* id, std::pair<geode::ListenerResult, EventReturn>(EventParameters...) function)
    //   only if the event has a return type
    //   creates an event listener with the given id and the function, and returns it
    //   you are expected to the listener result and the return value that recieve/receiveBoth methods will receive, in a pair
    //   the actual event then returns the first value of the pair, and the second one gets passed onto receive
    //   useful if you want to return values, but also want to conditionally stop the propagation of the event
    //
    // MyEvent::listenWithID<C>(const char* id, C* cls, geode::ListenerResult (C::*function)(EventParameters...))
    //   creates an event listener with the given id and the member function with the member, and returns it
    //   otherwise identical to the version that just takes a function
    //   all of the other methods specified above have this overload as well
    //
    // MyEvent::listenWithIDGlobal(const char* id, geode::ListenerResult(EventParameters...) function)
    //   same as listenWithID, but returns a pointer to the event listener, which was allocated on the heap
    //   useful for global listeners
    //   all of the methods specified above also have this version
    //
    // additionally, you may drop the "WithID" from all of these, and they will automatically prefill the id (or not) in the same way
    // that is done in post/receive/receiveBoth
    //
    // so, the final list of all available method names is:
    //
    // listenWithID, sendWithID, sendBothWithID
    // listenWithIDGlobal, sendWithIDGlobal, sendBothWithIDGlobal,
    // listen, send, sendBoth,
    // listenGlobal, sendGlobal, sendBothGlobal
    
    

    template <typename Function> requires internal::IsValidFunction<Function, geode::ListenerResult, EventParameters...>
    static Listener listenWithID(const char* id, Function&& function) {
      if constexpr (std::is_void_v<EventReturn>)
        return Listener(std::forward<Function>(function), Filter(id));
      else
        // the underlying event will receive the pointer, but will do nothing with it
        return Listener(
          [func = std::forward<Function>(function)](EventReturn*, EventParameters... args) {
            return func(args...);
          },
          Filter(id)
        );
    }
    
    template<typename C, typename R, typename... Ps>
    using MemberFunction = internal::MemberFunction<C, R, Ps...>;

    template<typename Class> requires internal::IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
    static Listener listenWithID(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult> function) {
      if constexpr (std::is_void_v<EventReturn>)
        return Listener(instance, function, Filter(id));
      else
        // the underlying event will receive the pointer, but will do nothing with it
        return Listener(
          [instance, function](EventReturn*, EventParameters... args) {
            return (instance->*function)(args...);
          },
          Filter(id)
        );
    }
    
    template <typename Function> requires internal::IsValidFunction<Function, geode::ListenerResult, EventParameters...>
    static Listener* listenWithIDGlobal(const char* id, Function&& function)
    { return new Listener(listenWithID(id, std::forward<Function>(function))); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
    static Listener* listenWithIDGlobal(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult> function)
    { return new Listener(listenWithID(id, instance, function)); }

    template <typename Function> requires !std::is_void_v<EventReturn> && internal::IsValidFunction<Function, EventReturn, EventParameters...>
    static Listener sendWithID(const char* id, geode::ListenerResult result, Function&& function) {
      return Listener(
        [result, func = std::forward<Function>(function)](EventReturn* into, EventParameters... args) {
          *into = func(args...);
          return result; 
        },
        Filter(id)
      );
    }
    
    template<typename Class> requires !std::is_void_v<EventReturn> && internal::IsValidMemberFunction<Class, EventReturn, EventParameters...>
    static Listener sendWithID(const char* id, geode::ListenerResult result, Class* instance, MemberFunction<Class, EventReturn> function) {
      return Listener(
        [instance, function, result](EventReturn* into, EventParameters... args) {
          *into = (instance->*function)(args...);
          return result;
        },
        Filter(id)
      );
    }
    
    template <typename Function> requires internal::IsValidFunction<Function, EventReturn, EventParameters...>
    static Listener* sendWithIDGlobal(const char* id, geode::ListenerResult result, Function&& function)
    { return new Listener(sendWithID(id, result, std::forward<Function>(function))); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, EventReturn, EventParameters...>
    static Listener* sendWithIDGlobal(const char* id, geode::ListenerResult result, Class* instance, MemberFunction<Class, EventReturn> function)
    { return new Listener(sendWithID(id, result, instance, function)); }
    
    template <typename Function>
    requires !std::is_void_v<EventReturn> && internal::IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener sendBothWithID(const char* id, Function&& function) {
      return Listener(
        [func = std::forward<Function>(function)](EventReturn* into, EventParameters... args) {
          geode::ListenerResult result;
          std::tie(result, *into) = func(args...);
          return result;
        },
        Filter(id)
      );
    }
    
    template<typename Class>
    requires !std::is_void_v<EventReturn> && internal::IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener sendBothWithID(const char* id, Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>> function) {
      return Listener(
        [instance, function](EventReturn* into, EventParameters... args) {
          geode::ListenerResult result;
          std::tie(result, *into) = (instance->*function)(args...);
          return result;
        },
        Filter(id)
      );
    }

    template <typename Function> requires internal::IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener* sendBothWithIDGlobal(const char* id, Function&& function)
    { return new Listener(sendBothWithID(id, std::forward<Function>(function))); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener* sendBothWithIDGlobal(const char* id, Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>> function)
    { return new Listener(sendBothWithID(id, instance, function)); }

    // regular listen implementations
    template <typename Function> requires internal::IsValidFunction<Function, geode::ListenerResult, EventParameters...>
    static Listener listen(const char* id, Function&& function) requires std::is_void_v<EventID>
    { return listenWithID(id, std::forward<Function>(function)); }
    
    template <typename Function> requires internal::IsValidFunction<Function, geode::ListenerResult, EventParameters...>
    static Listener listen(Function&& function) requires !std::is_void_v<EventID>
    { return listenWithID(EventImpl::ID, std::forward<Function>(function)); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
    static Listener listen(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult> function) requires std::is_void_v<EventID>
    { return listenWithID(id, instance, function); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
    static Listener listen(Class* instance, MemberFunction<Class, geode::ListenerResult> function) requires !std::is_void_v<EventID>
    { return listenWithID(EventImpl::ID, instance, function); }
    
    template <typename Function> requires internal::IsValidFunction<Function, geode::ListenerResult, EventParameters...>
    static Listener* listenGlobal(const char* id, Function&& function) requires std::is_void_v<EventID>
    { return listenWithIDGlobal(id, std::forward<Function>(function)); }
    
    template <typename Function> requires internal::IsValidFunction<Function, geode::ListenerResult, EventParameters...>
    static Listener* listenGlobal(Function&& function) requires !std::is_void_v<EventID>
    { return listenWithIDGlobal(EventImpl::ID, std::forward<Function>(function)); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
    static Listener* listenGlobal(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult> function) requires std::is_void_v<EventID>
    { return listenWithIDGlobal(id, instance, function); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
    static Listener* listenGlobal(Class* instance, MemberFunction<Class, geode::ListenerResult> function) requires !std::is_void_v<EventID>
    { return listenWithIDGlobal(EventImpl::ID, instance, function); }

    // regular send implementations

    template <typename Function> requires internal::IsValidFunction<Function, EventReturn, EventParameters...>
    static Listener send(const char* id, geode::ListenerResult result, Function&& function) requires std::is_void_v<EventID>
    { return sendWithID(id, result, std::forward<Function>(function)); }
    
    template <typename Function> requires internal::IsValidFunction<Function, EventReturn, EventParameters...>
    static Listener send(geode::ListenerResult result, Function&& function) requires !std::is_void_v<EventID>
    { return sendWithID(EventImpl::ID, result, std::forward<Function>(function)); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, EventReturn, EventParameters...>
    static Listener send(const char* id, geode::ListenerResult result, Class* instance, MemberFunction<Class, EventReturn> function) requires std::is_void_v<EventID>
    { return sendWithID(id, result, instance, function); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, EventReturn, EventParameters...>
    static Listener send(Class* instance, geode::ListenerResult result, MemberFunction<Class, EventReturn> function) requires !std::is_void_v<EventID>
    { return sendWithID(EventImpl::ID, result, instance, function); }
    
    template <typename Function> requires internal::IsValidFunction<Function, EventReturn, EventParameters...>
    static Listener* sendGlobal(const char* id, geode::ListenerResult result, Function&& function) requires std::is_void_v<EventID>
    { return sendWithIDGlobal(id, result, std::forward<Function>(function)); }
    
    template <typename Function> requires internal::IsValidFunction<Function, EventReturn, EventParameters...>
    static Listener* sendGlobal(geode::ListenerResult result, Function&& function) requires !std::is_void_v<EventID>
    { return sendWithIDGlobal(EventImpl::ID, result, std::forward<Function>(function)); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, EventReturn, EventParameters...>
    static Listener* sendGlobal(const char* id, geode::ListenerResult result, Class* instance, MemberFunction<Class, EventReturn> function) requires std::is_void_v<EventID>
    { return sendWithIDGlobal(id, result, instance, function); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, EventReturn, EventParameters...>
    static Listener* sendGlobal(Class* instance, geode::ListenerResult result, MemberFunction<Class, EventReturn> function) requires !std::is_void_v<EventID>
    { return sendWithIDGlobal(EventImpl::ID, result, instance, function); }
    
    // regular send both implementations
    template <typename Function> requires internal::IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener sendBoth(const char* id, Function&& function) requires std::is_void_v<EventID>
    { return sendBothWithID(id, std::forward<Function>(function)); }
    
    template <typename Function> requires internal::IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener sendBoth(Function&& function) requires !std::is_void_v<EventID>
    { return sendBothWithID(EventImpl::ID, std::forward<Function>(function)); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener sendBoth(const char* id, Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>> function) requires std::is_void_v<EventID>
    { return sendBothWithID(id, instance, function); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener sendBoth(Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>> function) requires !std::is_void_v<EventID>
    { return sendBothWithID(EventImpl::ID, instance, function); }
    
    template <typename Function> requires internal::IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener* sendBothGlobal(const char* id, Function&& function) requires std::is_void_v<EventID>
    { return sendBothWithIDGlobal(id, std::forward<Function>(function)); }
    
    template <typename Function> requires internal::IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener* sendBothGlobal(Function&& function) requires !std::is_void_v<EventID>
    { return sendBothWithIDGlobal(EventImpl::ID, std::forward<Function>(function)); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener* sendBothGlobal(const char* id, Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>> function) requires std::is_void_v<EventID>
    { return sendBothWithIDGlobal(id, instance, function); }
    
    template <typename Class> requires internal::IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...>
    static Listener* sendBothGlobal(Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>> function) requires !std::is_void_v<EventID>
    { return sendBothWithIDGlobal(EventImpl::ID, instance, function); }

  };
  
  using GoodDispatchEvent = EventBuilder<>;
  
}
