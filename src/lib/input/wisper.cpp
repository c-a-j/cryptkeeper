#include <iostream>
#include <csignal>
#include <vector>

#include "lib/crypto/secure_bytes.hpp"
#include "util/logger/logger.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace ck::input {
  using ck::util::logger::logger;

#ifndef _WIN32
  struct TerminalInputGuard {
    int fd;
    termios oldt {};
    bool active {false};

    explicit TerminalInputGuard(int fd_) : fd(fd_) {
      if (tcgetattr(fd, &oldt) != 0) {
        return;
      }

      termios newt = oldt;
      newt.c_lflag &= ~static_cast<tcflag_t>(ECHO);
      newt.c_lflag &= ~static_cast<tcflag_t>(ICANON);
      newt.c_lflag &= ~static_cast<tcflag_t>(ISIG);
      newt.c_cc[VMIN] = 1;
      newt.c_cc[VTIME] = 0;

      if (tcsetattr(fd, TCSANOW, &newt) == 0) {
        active = true;
      }
    }

    ~TerminalInputGuard() {
      if (active) {
        tcsetattr(fd, TCSANOW, &oldt);
      }
    }
  };
#endif

  namespace {
    void erase_last_masked_char(std::vector<char>& value) {
      if (value.empty()) {
        return;
      }

      value.pop_back();
      std::cout << "\b \b";
      std::cout.flush();
    }

    void append_masked_char(std::vector<char>& value, const char ch) {
      value.push_back(ch);
      std::cout << '*';
      std::cout.flush();
    }
  }

  ck::crypto::SecureBytes wisper(std::string_view prompt) {
    logger.info(prompt);
    
    std::vector<char> value;
    bool interrupted = false;

#ifdef _WIN32
    for (;;) {
      int ch = _getch();

      if (ch == 3) {
        interrupted = true;
        break;
      }

      if (ch == 0 || ch == 224) {
        (void)_getch();
        continue;
      }

      if (ch == '\r') {
        break;
      }

      if (ch == '\b') {
        erase_last_masked_char(value);
        continue;
      }

      if (ch >= 32 && ch <= 126) {
        append_masked_char(value, static_cast<char>(ch));
      }
    }
    std::cout << "\n";
#else
    {
      TerminalInputGuard guard(STDIN_FILENO);

      for (;;) {
        char ch {};
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        if (n <= 0) {
          break;
        }

        if (ch == 3) {
          interrupted = true;
          break;
        }

        if (ch == '\n' || ch == '\r') {
          break;
        }

        if (ch == 127 || ch == '\b') {
          erase_last_masked_char(value);
          continue;
        }

        if (static_cast<unsigned char>(ch) < 32) {
          continue;
        }

        append_masked_char(value, ch);
      }
    }
    std::cout << "\n";
#endif

    if (interrupted) {
      ck::crypto::secure_wipe(value.data(), value.size());
      std::raise(SIGINT);
    }

    ck::crypto::SecureBytes result;
    result.assign(value.data(), value.size());
    ck::crypto::secure_wipe(value.data(), value.size());
    return result;
  }
}
