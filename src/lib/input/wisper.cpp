#include <string>
#include <iostream>
#include <csignal>

#include "lib/types.hpp"
#include "util/logger.hpp"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace ck::lib {
  using namespace ck::types;
  using namespace ck::util::logger;

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

  void wisper(Secret& secret) {
    logger.info("Enter secret:");

    std::string value;
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
        if (!value.empty()) {
          value.pop_back();
          std::cout << "\b \b";
          std::cout.flush();
        }
        continue;
      }

      if (ch >= 32 && ch <= 126) {
        value.push_back(static_cast<char>(ch));
        std::cout << '*';
        std::cout.flush();
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
          if (!value.empty()) {
            value.pop_back();
            std::cout << "\b \b";
            std::cout.flush();
          }
          continue;
        }

        if (static_cast<unsigned char>(ch) < 32) {
          continue;
        }

        value.push_back(static_cast<char>(ch));
        std::cout << '*';
        std::cout.flush();
      }
    }
    std::cout << "\n";
#endif

    if (interrupted) {
      std::raise(SIGINT);
    }

    secret.value = value;
  }
}
