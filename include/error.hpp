#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace ck::error {
class AppError : public std::runtime_error {
  private:
    struct Messages {
        std::string first;
        std::string second;
    };

    std::shared_ptr<const Messages> messages_;

  public:
    AppError(std::string m1, std::string m2 = {})
        : std::runtime_error(m1 + m2),
          messages_(
              std::make_shared<const Messages>(Messages{
                  std::move(m1), std::move(m2)})
          ) {
    }

    const std::string &msg1() const noexcept {
        return messages_->first;
    }

    const std::string &msg2() const noexcept {
        return messages_->second;
    }
};

template <typename T> struct Error : AppError {
    T code;

    Error(T c, std::string m2 = {})
        : AppError(std::string(label(c)), std::move(m2)), code(c) {
    }

  private:
    static std::string_view label(T c);
};
} // namespace ck::error
