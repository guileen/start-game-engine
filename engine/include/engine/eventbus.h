// https://gist.github.com/darkf/8882611

// EventBus 是一个单例，用于管理全局事件，相当于一种异步编程的模式。
// 注意事项：一个按钮的事件监听，不应该使用EventBus，但一个按钮点击之后，可能会emit一个EventBus事件。
// 但是所有一类按钮的事件监听，可以由EventBus监听，相当于 On Any Button Clicked，而各个Button对应的Handler应该在其他地方已经有管理，不属于EventBus的管理范畴，而是这个Any Button Click Listener需要处理的事情。

// 键盘、鼠标事件直接由 input manager进行管理是更明智的做法。

#pragma once

#include <map>
#include <functional>
#include <typeinfo>
// Has the limitation that on<T> will not catch subtypes of T, only T.
// That may or may not be a problem for your usecase.
//
// It also doesn't (yet) let you use the subtype as an argument.
struct Event{};

typedef std::multimap<const std::type_info*,
					  const std::function<void(const Event&)>> EventMap;

namespace EventBus {
    namespace {
        // anonymouse namespace as private
        EventMap eventMap;
    }

    // template<typename EventWanted>
    // void on(const std::function<void(const EventWanted&)>& fn) {
    //     event::eventMap.emplace(&typeid(EventWanted), fn);
    // }
    template<typename EventWanted>
    void on(const std::function<void(const Event&)>& fn) {
        printf("register event listener %d\n", typeid(EventWanted).name());
        eventMap.emplace(&typeid(EventWanted), fn);
    }

    // template<typename EventWanted, typename Func>
    // void on(Func fn) {
    //     eventMap.emplace(&typeid(EventWanted), fn);
    // }

    void emit(const Event& event) {
        printf("Emit event: %s\n", typeid(event).name());
        auto range = eventMap.equal_range(&typeid(event));
        for(auto it = range.first; it != range.second; ++it)
            it->second(event);
    }
};