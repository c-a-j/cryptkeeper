#pragma once

#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <optional>
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
    inline static constexpr std::array<Field<StrMember>, 2> k_fields {{
      {"path", &Mount::path},
      {"hash", &Mount::hash},
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
    fs::path vault_path;
    std::string relative_path;
  };

  class Mounts {
    public:
      explicit Mounts();
      explicit Mounts(State&);

      Mount& root();
      const Mount& root() const;

      std::unordered_map<std::string, Mount>& mounts();
      const std::unordered_map<std::string, Mount>& mounts() const;

      void mount(const std::string&, const std::string&);
      void mount(const std::string&);
      void umount(const std::string&);
      void chroot(const std::string&);
      ResolvedPath resolve(const std::string&);
      void print();
      bool empty() const noexcept;
      bool file_exists() const noexcept;

      void load();
      void save();

    private:
      State state_;
      fs::path file_;
  };

  extern Mounts mnt;
}
