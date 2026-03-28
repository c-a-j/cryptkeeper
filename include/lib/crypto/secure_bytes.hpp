#pragma once
#include <string>
#include <vector>
#include <cstddef>
#include <span>

#include "util/error.hpp"
#include "./secure_wipe.hpp"

namespace ck::crypto {
  using ck::util::error::Error;
  using ck::util::error::CryptoErrc;
  using enum ck::util::error::CryptoErrc;
  
  class SecureBytes {
    public:
      SecureBytes() = default;
      explicit SecureBytes(std::size_t size) : data_(size) {}
      
      SecureBytes(const SecureBytes&) = delete;
      SecureBytes& operator=(const SecureBytes&) = delete;
      
      SecureBytes(SecureBytes&& other) noexcept = default;
      SecureBytes& operator=(SecureBytes&& other) noexcept = default;
      
      ~SecureBytes() {
        secure_wipe(data_.data(), data_.size());
      }
      
      std::byte* data() { return data_.data(); }
      const std::byte* data() const { return data_.data(); }
      std::size_t size() const { return data_.size(); }
      
      char* char_data() { return reinterpret_cast<char*>(data_.data()); }
      const char* char_data() const { return reinterpret_cast<const char*>(data_.data()); }

      std::span<std::byte> bytes() { 
        return std::span<std::byte>{data_.data(), data_.size()}; 
      }
      std::span<const std::byte> bytes() const { 
        return std::span< const std::byte>{data_.data(), data_.size()}; 
      }
      
      void assign(const char* src, std::size_t len) {
        if (src == nullptr && len != 0) {
          throw Error<CryptoErrc>{InvalidSecureBytesArgs};
        }
        data_.assign(
          reinterpret_cast<const std::byte*>(src),
          reinterpret_cast<const std::byte*>(src) + len
        );
      }
    
    private:
      std::vector<std::byte> data_;
  };
}
