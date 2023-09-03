#pragma once

#include <csvpp/Conversions.h>
#include <csvpp/LineProcessor.h>
#include <csvpp/Read.h>

#include <functional>

namespace csvpp {
template <typename... Ts> class TypedProcessor : public LineProcessor {
public:
  template <typename... Fields>
  TypedProcessor(Fields &&...columns)
      : LineProcessor{std::forward<Fields>(columns)...} {
    static_assert(sizeof...(Fields) == sizeof...(Ts),
                  "number of fields should be equal to number of types");
  }

  template <typename ProcessAction, typename... Fields>
  static void process(const Reader &reader, const std::string &fileName,
                      ProcessAction &&action, Fields &&...columns) {
    TypedProcessorFctor proc{std::forward<ProcessAction>(action),
                             std::forward<Fields>(columns)...};
    reader.read(fileName, proc);
  }

protected:
  void process(const Line &row) final {
    impl_.process(row, [this](Ts &&...args) {
      this->process_(std::forward<Ts>(args)...);
    });
  }

  virtual void process_(Ts... converted) = 0;

private:
  template <std::size_t Index, typename T, typename... To>
  class ColumnRecursion : public ColumnRecursion<Index + 1, To...> {
  public:
    template <typename Action, typename... PackedArgs>
    void process(const Line &line, const Action &action, PackedArgs &&...args) {
      this->ColumnRecursion<Index + 1, To...>::process(
          line, action, std::forward<PackedArgs>(args)...,
          Converter<T>::convert(line[Index]));
    }
  };

  template <std::size_t Index, typename T> class ColumnRecursion<Index, T> {
  public:
    template <typename Action, typename... PackedArgs>
    void process(const Line &line, const Action &action, PackedArgs &&...args) {
      action(std::forward<PackedArgs>(args)...,
             Converter<T>::convert(line[Index]));
    }
  };

  ColumnRecursion<0, Ts...> impl_;

  class TypedProcessorFctor;
};

template <typename... Ts>
class TypedProcessor<Ts...>::TypedProcessorFctor final
    : public TypedProcessor<Ts...> {
public:
  using ProcessActionT = std::function<void(Ts...)>;

  template <typename ProcessAction, typename... Fields>
  TypedProcessorFctor(ProcessAction &&action, Fields &&...columns)
      : TypedProcessor<Ts...>{std::forward<Fields>(columns)...},
        action_{std::forward<ProcessAction>(action)} {}

protected:
  void process_(Ts... converted) final {
    action_(std::forward<Ts>(converted)...);
  }

private:
  ProcessActionT action_;
};

} // namespace csvpp
