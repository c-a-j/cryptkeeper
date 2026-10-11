#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <filesystem>

namespace ck::mount {
  namespace fs = std::filesystem;
  template<typename Owner, typename Member>
  using MemberPtr = Member Owner::*;

  template<typename T>
  struct Field {
    std::string_view key;
    T member;
  };
  
  struct Mount {
    std::string path;
    std::string hash;

    using StrMember = MemberPtr<Mount, std::string>;
    static constexpr std::array<Field<StrMember>, 2> k_fields {{
      {.key = "path", .member = &Mount::path},
      {.key = "hash", .member = &Mount::hash},
    }};

    static constexpr const std::array<Field<StrMember>, 2>& fields() { return k_fields; }
    static constexpr const std::string_view k_name = "mount";
  };

  struct State {
    Mount root;
    std::unordered_map<std::string, Mount> mounts;
  };

  struct ResolvedPath {
    std::string alias;
    fs::path crypt_path;
    std::string relative_path;
  };

  class Mounts {
    public:
      explicit Mounts() = default;
      explicit Mounts(State& s);

      Mount& root();
      const Mount& root() const;

      std::unordered_map<std::string, Mount>& mounts();
      const std::unordered_map<std::string, Mount>& mounts() const;

      void mount(const std::string& path, const std::string& alias);
      void mount(const std::string& alias);
      void umount(const std::string& alias);
      void chroot(const std::string& path);
      ResolvedPath resolve(const std::string& path);
      void print();
      bool empty() const noexcept;
      static bool file_exists();

      void load();
      void save();

    private:
      State state_;
  };

  extern Mounts mnt;
}
