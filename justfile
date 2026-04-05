set shell := ["bash", "-cu"]

build_dir := "build"
build_type := "Debug"
binary_name := "ck"
binary := build_dir + "/src/" + binary_name
vault_dir := "~/.local/share/crypt-keeper"
config_dir := "~/.config/crypt-keeper"
asan_env := "ASAN_SYMBOLIZER_PATH=" + `which llvm-symbolizer` + " ASAN_OPTIONS=symbolize=1"
bindir := "~/.local/bin"

configure build_type="Debug" sanitizers="OFF" tests="OFF" debug_logs="OFF" clang_tidy="OFF":
  cmake -S . -B {{ build_dir }} \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCK_ENABLE_SANITIZERS={{ sanitizers }} \
    -DCMAKE_BUILD_TYPE={{ build_type }} \
    -DCK_ENABLE_DEBUG_LOGS={{ debug_logs }} \
    -DBUILD_TESTING={{ tests }} \
    -DCK_ENABLE_CLANG_TIDY={{ clang_tidy }}

build tests="OFF": (configure "Debug" "ON" (tests) "ON" "ON")
  cmake --build {{ build_dir }} -- -j$(nproc)

release: (configure "Release" "OFF" "OFF" "OFF" "OFF")
  cmake --build {{ build_dir }} -- -j$(nproc)

run *args: build
 {{ asan_env }} ./{{ binary }} {{ args }}

test filter="": (build ("ON"))
  cd {{ build_dir }} && ctest --output-on-failure {{ if filter != "" { "-R " + filter } else { "" }}} 
  
lint: (configure "Debug" "OFF" "OFF" "ON" "ON")
  cmake --build {{ build_dir }} --clean-first -- -j$(nproc)

[unix]
install dir=bindir: release
  /usr/bin/install -d {{ dir }}
  /usr/bin/install -m 0755 {{ binary }} {{ dir }}/{{ binary_name }}

[windows]
install dir=bindir:
  {{ error("Windows install not supported. Use WSL or install the binary manually.") }}

destroy:
  rm -rf {{ vault_dir }}
  rm -rf {{ config_dir }}
