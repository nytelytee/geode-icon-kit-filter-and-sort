/*
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org>
 */

#pragma once

#include <Geode/loader/Dispatch.hpp>

namespace nytelyte::easy_event {

  namespace internal {

    template <size_t N>
    struct StringLiteral {
      constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }
      char value[N];
    };

    template <typename Function, typename... EventParameters>
    concept FunctionHasValidArguments = std::invocable<Function, EventParameters...>;

    template <typename Function, typename ReturnType, typename... EventParameters>
    concept FunctionHasValidReturnType = (
      (std::is_void_v<ReturnType> && std::is_void_v<std::invoke_result_t<Function, EventParameters...>>) ||
      std::assignable_from<ReturnType&, std::invoke_result_t<Function, EventParameters...>>
    );

    template <typename Function, typename ReturnType, typename... EventParameters>
    concept IsValidFunction = (
      FunctionHasValidArguments<Function, EventParameters...> &&
      FunctionHasValidReturnType<Function, ReturnType, EventParameters...>
    );

    template <typename Class, typename ReturnType, typename... EventParameters>
    using MemberFunction = typename geode::to_member<Class, ReturnType(EventParameters...)>::value;

    template <typename Class, typename ReturnType, typename... EventParameters>
    concept MemberFunctionHasValidReturnType = (
      (std::is_void_v<ReturnType> && std::is_void_v<std::invoke_result_t<MemberFunction<Class, ReturnType, EventParameters...>, Class*, EventParameters...>>) ||
      std::assignable_from<ReturnType&, std::invoke_result_t<MemberFunction<Class, ReturnType, EventParameters...>, Class*, EventParameters...>>
    );

    template <typename Class, typename ReturnType, typename... EventParameters>
    concept IsValidMemberFunction = MemberFunctionHasValidReturnType<Class, ReturnType, EventParameters...>;

    template <typename EventID, typename EventParameters, typename EventReturn>
    struct EventImpl;

    template <typename EventID, typename... EventParameters, typename EventReturn>
    struct EventImpl<EventID, std::tuple<EventParameters...>, EventReturn> {
    private:
      using EventReturnPointer = std::conditional_t<std::is_void_v<EventReturn>, std::nullptr_t, EventReturn*>;
    public:
      // Actual internal dispatch event values, in case you need them
      using Filter = std::conditional_t<
          std::is_void_v<EventReturn>,
          geode::DispatchFilter<EventParameters...>,
          geode::DispatchFilter<EventReturn*, EventParameters...>
      >;
      using Event = Filter::Event;
      using Listener = geode::EventListener<Filter>;

      // The parameters the EasyEvent is using right now, in case you need them
      static constexpr const char* ID = []{ if constexpr (std::is_void_v<EventID>) return nullptr; else return EventID::value; }();
      using Returns = EventReturn;
      using Takes = std::tuple<EventParameters...>;

      // Normal post
      inline static geode::ListenerResult postWithID(const char* id, EventParameters... args) {
        if constexpr (std::is_void_v<EventReturn>) return Event(id, args...).post();
        else return Event(id, nullptr, args...).post();
      }
      
      // Raw post
      inline static geode::ListenerResult rawPostWithID(const char* id, EventReturnPointer into, EventParameters... args) {
        if constexpr (std::is_void_v<EventReturn>) return Event(id, args...).post();
        else return Event(id, into, args...).post();
      }

      // Receiving post
      inline static EventReturn receiveWithID(const char* id, EventParameters... args)
      requires (!std::is_void_v<EventReturn>)  {
        EventReturn into;
        Event(id, &into, args...).post();
        return into;
      }

      // Receiving both post
      inline static std::pair<geode::ListenerResult, EventReturn> receiveBothWithID(const char* id, EventParameters... args)
      requires (!std::is_void_v<EventReturn>) {
        EventReturn into;
        geode::ListenerResult result = Event(id, &into, args...).post();
        return std::pair{result, into};
      }

      // If the easy event has an embedded ID, these post/receive/receiveBoth/rawPost overloads are defined, allowing you to post the event without an ID
      // This part is a lot more complicated for listeners given how many options they have to support, but luckily, at least this is simple enough to write
      // out manually.
      inline static geode::ListenerResult post(EventParameters... args)
      requires (!std::is_void_v<EventID>)
      { return postWithID(EventImpl::ID, args...); }
      
      inline static geode::ListenerResult rawPost(EventReturnPointer into, EventParameters... args)
      requires (!std::is_void_v<EventID>)
      { return rawPostWithID(EventImpl::ID, into, args...); }

      inline static EventReturn receive(EventParameters... args)
      requires (!std::is_void_v<EventID> && !std::is_void_v<EventReturn>)
      { return receiveWithID(EventImpl::ID, args...); }

      inline static std::pair<geode::ListenerResult, EventReturn> receiveBoth(EventParameters... args)
      requires (!std::is_void_v<EventID> && !std::is_void_v<EventReturn>)
      { return receiveBothWithID(EventImpl::ID, args...); }

      // Base functions

      // Preset result listening
      template <geode::ListenerResult result, typename Function>
      requires IsValidFunction<Function, void, EventParameters...>
      inline static Listener listenWithID(const char* id, Function&& function) {
        if constexpr (std::is_void_v<EventReturn>)
          return Listener(
            [func = std::forward<Function>(function)](EventParameters... args) {
              func(args...);
              return result;
            },
            Filter(id)
          );
        else
          // the underlying event will receive the return value pointer, but will do nothing with it
          return Listener(
            [func = std::forward<Function>(function)](EventReturn*, EventParameters... args) {
              func(args...);
              return result;
            },
            Filter(id)
          );
      }

      // Unpreset result listening
      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventParameters...>
      inline static Listener listenWithID(const char* id, Function&& function) {
        if constexpr (std::is_void_v<EventReturn>)
          return Listener(std::forward<Function>(function), Filter(id));
        else
          // the underlying event will receive the return value pointer, but will do nothing with it
          return Listener(
            [func = std::forward<Function>(function)](EventReturn*, EventParameters... args) {
              return func(args...);
            },
            Filter(id)
          );
      }

      // Preset result raw listening
      template <geode::ListenerResult result, typename Function>
      requires IsValidFunction<Function, void, EventReturnPointer, EventParameters...>
      inline static Listener rawListenWithID(const char* id, Function&& function) {
        if constexpr(std::is_void_v<EventReturn>)
          return Listener(
            [func = std::forward<Function>(function)](EventParameters... args) {
              func(nullptr, args...);
              return result;
            },
            Filter(id)
          );
        else
          return Listener(
            [func = std::forward<Function>(function)](EventReturnPointer into, EventParameters... args) {
              func(into, args...);
              return result;
            },
            Filter(id)
          );
      }

      // Unpreset result raw listening
      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventReturnPointer, EventParameters...>
      inline static Listener rawListenWithID(const char* id, Function&& function) {
        if constexpr(std::is_void_v<EventReturn>)
          return Listener(
            [func = std::forward<Function>(function)](EventParameters... args) {
              return func(nullptr, args...);
            },
            Filter(id)
          );
        else
          return Listener(
            [func = std::forward<Function>(function)](EventReturnPointer into, EventParameters... args) {
              return func(into, args...);
            },
            Filter(id)
          );
      }


      // Preset result sending
      template <geode::ListenerResult result, typename Function>
      requires IsValidFunction<Function, EventReturn, EventParameters...> && (!std::is_void_v<EventReturn>)
      inline static Listener sendWithID(const char* id, Function&& function) {
        return Listener(
          [func = std::forward<Function>(function)](EventReturn* into, EventParameters... args) {
            if (into) *into = func(args...);
            else func(args...);
            return result;
          },
          Filter(id)
        );
      }

      // Unpreset result sending
      template <typename Function>
      requires IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventReturn>)
      inline static Listener sendWithID(const char* id, Function&& function) {
        return Listener(
          [func = std::forward<Function>(function)](EventReturn* into, EventParameters... args) {
            geode::ListenerResult result;
            if (into) std::tie(result, *into) = func(args...);
            else std::tie(result, std::ignore) = func(args...);
            return result;
          },
          Filter(id)
        );
      }

      // Base member functions, suffixed with On

      // Preset result listening
      template <geode::ListenerResult result, typename Class>
      requires IsValidMemberFunction<Class, void, EventParameters...>
      inline static Listener listenWithIDOn(const char* id, Class* instance, MemberFunction<Class, void, EventParameters...> function) {
        if constexpr (std::is_void_v<EventReturn>)
          return Listener(
            [instance, function](EventParameters... args) {
              (instance->*function)(args...);
              return result;
            },
            Filter(id)
          );
        else
          // the underlying event will receive the return value pointer, but will do nothing with it
          return Listener(
            [instance, function](EventReturn*, EventParameters... args) {
              (instance->*function)(args...);
              return result;
            },
            Filter(id)
          );
      }

      // Unpreset result listening
      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
      inline static Listener listenWithIDOn(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult, EventParameters...> function) {
        if constexpr (std::is_void_v<EventReturn>)
          return Listener(instance, function, Filter(id));
        else
          // the underlying event will receive the return value pointer, but will do nothing with it
          return Listener(
            [instance, function](EventReturn*, EventParameters... args) {
              return (instance->*function)(args...);
            },
            Filter(id)
          );
      }

      // Preset result raw listening
      template <geode::ListenerResult result, typename Class>
      requires IsValidMemberFunction<Class, void, EventReturnPointer, EventParameters...>
      inline static Listener rawListenWithIDOn(const char* id, Class* instance, MemberFunction<Class, void, EventReturnPointer, EventParameters...> function) {
        if constexpr (std::is_void_v<EventReturn>)
          return Listener(
            [instance, function](EventParameters... args) {
              (instance->*function)(nullptr, args...);
              return result;
            },
            Filter(id)
          );
        else
          return Listener(
            [instance, function](EventReturnPointer into, EventParameters... args) {
              (instance->*function)(into, args...);
              return result;
            },
            Filter(id)
          );
      }

      // Unpreset result raw listening
      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...>
      inline static Listener rawListenWithIDOn(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...> function) {
        if constexpr(std::is_void_v<EventReturn>)
          return Listener(
            [instance, function](EventParameters... args) {
              return (instance->*function)(nullptr, args...);
            },
            Filter(id)
          );
        else
          return Listener(
            [instance, function](EventReturnPointer into, EventParameters... args) {
              return (instance->*function)(into, args...);
            },
            Filter(id)
          );
      }

      // Preset result sending
      template <geode::ListenerResult result, typename Class>
      requires IsValidMemberFunction<Class, EventReturn, EventParameters...> && (!std::is_void_v<EventReturn>)
      inline static Listener sendWithIDOn(const char* id, Class* instance, MemberFunction<Class, EventReturn, EventParameters...> function) {
        return Listener(
          [instance, function](EventReturn* into, EventParameters... args) {
            if (into) *into = (instance->*function)(args...);
            else (instance->*function)(args...);
            return result;
          },
          Filter(id)
        );
      }

      // Unpreset result sending
      template <typename Class>
      requires IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventReturn>)
      inline static Listener sendWithIDOn(const char* id, Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> function) {
        return Listener(
          [instance, function](EventReturn* into, EventParameters... args) {
            geode::ListenerResult result;
            if (into) std::tie(result, *into) = (instance->*function)(args...);
            else std::tie(result, std::ignore) = (instance->*function)(args...);
            return result;
          },
          Filter(id)
        );
      }

      /**********************************************************************************************************/
      /******************************************THIS PART IS AUTOGENERATED**************************************/
      /**********************************************************************************************************/

      /*************************/
      /* MAIN GLOBAL LISTENERS */
      /*************************/

      // Global versions of listenWithID[On]

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, void, EventParameters...>
      static Listener* globalListenWithID(const char* id, Function&& function)
      { return new Listener (listenWithID<result, Function>(id, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventParameters...>
      static Listener* globalListenWithID(const char* id, Function&& function)
      { return new Listener (listenWithID<Function>(id, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, void, EventParameters...>
      static Listener* globalListenWithIDOn(const char* id, Class* instance, MemberFunction<Class, void, EventParameters...> function)
      { return new Listener (listenWithIDOn<result, Class>(id, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...>
      static Listener* globalListenWithIDOn(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult, EventParameters...> function)
      { return new Listener (listenWithIDOn<Class>(id, instance, function)); }

      // Global versions of rawListenWithID[On]

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, void, EventReturnPointer, EventParameters...>
      static Listener* globalRawListenWithID(const char* id, Function&& function)
      { return new Listener (rawListenWithID<result, Function>(id, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventReturnPointer, EventParameters...>
      static Listener* globalRawListenWithID(const char* id, Function&& function)
      { return new Listener (rawListenWithID<Function>(id, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, void, EventReturnPointer, EventParameters...>
      static Listener* globalRawListenWithIDOn(const char* id, Class* instance, MemberFunction<Class, void, EventReturnPointer, EventParameters...> function)
      { return new Listener (rawListenWithIDOn<result, Class>(id, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...>
      static Listener* globalRawListenWithIDOn(const char* id, Class* instance, MemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...> function)
      { return new Listener (rawListenWithIDOn<Class>(id, instance, function)); }

      // Global versions of sendWithID[On]

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, EventReturn, EventParameters...> && (!std::is_void_v<EventReturn>)
      static Listener* globalSendWithID(const char* id, Function&& function)
      { return new Listener (sendWithID<result, Function>(id, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventReturn>)
      static Listener* globalSendWithID(const char* id, Function&& function)
      { return new Listener (sendWithID<Function>(id, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, EventReturn, EventParameters...> && (!std::is_void_v<EventReturn>)
      static Listener* globalSendWithIDOn(const char* id, Class* instance, MemberFunction<Class, EventReturn, EventParameters...> function)
      { return new Listener (sendWithIDOn<result, Class>(id, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventReturn>)
      static Listener* globalSendWithIDOn(const char* id, Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> function)
      { return new Listener (sendWithIDOn<Class>(id, instance, function)); }

      /*****************************/
      /* PRESET EVENT ID LISTENERS */
      /*****************************/

      // Preset versions of listenWithID[On]/globalListenWithID[On]

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, void, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener listen(Function&& function)
      { return (listenWithID<result, Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener listen(Function&& function)
      { return (listenWithID<Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, void, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalListen(Function&& function)
      { return (globalListenWithID<result, Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalListen(Function&& function)
      { return (globalListenWithID<Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, void, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener listenOn(Class* instance, MemberFunction<Class, void, EventParameters...> function)
      { return (listenWithIDOn<result, Class>(EventImpl::ID, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener listenOn(Class* instance, MemberFunction<Class, geode::ListenerResult, EventParameters...> function)
      { return (listenWithIDOn<Class>(EventImpl::ID, instance, function)); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, void, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalListenOn(Class* instance, MemberFunction<Class, void, EventParameters...> function)
      { return (globalListenWithIDOn<result, Class>(EventImpl::ID, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalListenOn(Class* instance, MemberFunction<Class, geode::ListenerResult, EventParameters...> function)
      { return (globalListenWithIDOn<Class>(EventImpl::ID, instance, function)); }

      // Preset versions of rawListenWithID[On]/globalRawListenWithID[On]

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, void, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener rawListen(Function&& function)
      { return (rawListenWithID<result, Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener rawListen(Function&& function)
      { return (rawListenWithID<Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, void, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalRawListen(Function&& function)
      { return (globalRawListenWithID<result, Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, geode::ListenerResult, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalRawListen(Function&& function)
      { return (globalRawListenWithID<Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, void, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener rawListenOn(Class* instance, MemberFunction<Class, void, EventReturnPointer, EventParameters...> function)
      { return (rawListenWithIDOn<result, Class>(EventImpl::ID, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener rawListenOn(Class* instance, MemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...> function)
      { return (rawListenWithIDOn<Class>(EventImpl::ID, instance, function)); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, void, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalRawListenOn(Class* instance, MemberFunction<Class, void, EventReturnPointer, EventParameters...> function)
      { return (globalRawListenWithIDOn<result, Class>(EventImpl::ID, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...> && (!std::is_void_v<EventID>)
      static Listener* globalRawListenOn(Class* instance, MemberFunction<Class, geode::ListenerResult, EventReturnPointer, EventParameters...> function)
      { return (globalRawListenWithIDOn<Class>(EventImpl::ID, instance, function)); }

      // Preset versions of sendWithID[On]/globalSendWithID[On]

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, EventReturn, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener send(Function&& function)
      { return (sendWithID<result, Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener send(Function&& function)
      { return (sendWithID<Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Function>
      requires IsValidFunction<Function, EventReturn, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener* globalSend(Function&& function)
      { return (globalSendWithID<result, Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <typename Function>
      requires IsValidFunction<Function, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener* globalSend(Function&& function)
      { return (globalSendWithID<Function>(EventImpl::ID, std::forward<Function>(function))); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, EventReturn, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener sendOn(Class* instance, MemberFunction<Class, EventReturn, EventParameters...> function)
      { return (sendWithIDOn<result, Class>(EventImpl::ID, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener sendOn(Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> function)
      { return (sendWithIDOn<Class>(EventImpl::ID, instance, function)); }

      template <geode::ListenerResult result ,typename Class>
      requires IsValidMemberFunction<Class, EventReturn, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener* globalSendOn(Class* instance, MemberFunction<Class, EventReturn, EventParameters...> function)
      { return (globalSendWithIDOn<result, Class>(EventImpl::ID, instance, function)); }

      template <typename Class>
      requires IsValidMemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> && (!std::is_void_v<EventID>) && (!std::is_void_v<EventReturn>)
      static Listener* globalSendOn(Class* instance, MemberFunction<Class, std::pair<geode::ListenerResult, EventReturn>, EventParameters...> function)
      { return (globalSendWithIDOn<Class>(EventImpl::ID, instance, function)); }

    };

    template <typename T>
    struct is_tuple : std::false_type {};

    template <typename... Args>
    struct is_tuple<std::tuple<Args...>> : std::true_type {};

    template <typename EventID = void, typename EventParameters = std::tuple<>, typename EventReturn = void>
    struct EventBuilder : public EventImpl<EventID, EventParameters, EventReturn> {
    private:
      template <StringLiteral... Id>
      static constexpr auto makeIdType() {
        if constexpr (sizeof...(Id) == 0)
          return static_cast<EventBuilder<void, EventParameters, EventReturn>*>(nullptr);
        else if constexpr (sizeof...(Id) == 1)
          return static_cast<EventBuilder<std::integral_constant<const char*, Id.value>..., EventParameters, EventReturn>*>(nullptr);
        else
          static_assert(sizeof...(Id) <= 1, "id<> accepts at most one string literal");
      }

      template <typename T>
      struct takes_tuple_helper {
          static_assert(is_tuple<T>::value, "takes_tuple requires a std::tuple<Args...> type");
          using type = EventBuilder<EventID, T, EventReturn>;
      };

    public:
      EventBuilder() = delete;
      EventBuilder(const EventBuilder&) = delete;
      EventBuilder(EventBuilder&&) = delete;
      EventBuilder& operator=(const EventBuilder&) = delete;
      EventBuilder& operator=(EventBuilder&&) = delete;
      ~EventBuilder() = delete;

      // Allows you to set the dispatch event ID as part of the type itself, so you only have to write it once; now you can't misspell it.
      // You can remove the id via ::id<>, in case you want to do that for some reason.
      template <StringLiteral... Id>
      using id = std::remove_pointer_t<decltype(makeIdType<Id...>())>;

      // You pass in the types of the parameters that the event takes, in order, the same way you would on a geode::DispatchEvent.
      template <typename... Parameters>
      using takes = EventBuilder<EventID, std::tuple<Parameters...>, EventReturn>;

      // Added in case you want to, say, copy a different EasyEvent's Takes values, you could do EasyEvent::takes_tuple<MyEvent::Takes>.
      // I could just pass ParameterTuple directly to EventBuilder, but the error messages suck, so we're doing it like this
      // since i can actually add a static assert into takes_tuple_helper.
      template <typename ParameterTuple = std::tuple<>>
      using takes_tuple = typename takes_tuple_helper<ParameterTuple>::type;

      // An EasyEvent may have a return value, passed via pointer, the underlying dispatch event receives that pointer as the first argument.
      // You can remove the return value (which is the same as setting it to void), by passing ::returns<> (or ::returns<void>).
      template <typename Return = void>
      using returns = EventBuilder<EventID, EventParameters, Return>;
    };

  }
  using EasyEvent = internal::EventBuilder<>;

}
