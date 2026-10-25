set shell := ["bash", "-cu"]

build_dir := "build"
preset := "debug"
binary_name := "ck"
crypt_dir := "~/.local/share/crypt-keeper"
config_dir := "~/.config/crypt-keeper"
asan_env := "ASAN_SYMBOLIZER_PATH=" + `which llvm-symbolizer` + " ASAN_OPTIONS=symbolize=1"
bindir := "~/.local/bin"

configure preset=preset tests="OFF":
  cmake --preset "{{ preset }}" -DBUILD_TESTING="{{ tests }}"

build preset=preset tests="OFF": (configure (preset) (tests))
  cmake --build --preset "{{ preset }}"

test preset=preset regex="": (build (preset) ("ON"))
  ctest --preset "{{ preset }}" -R "{{ regex }}"

lint: (configure ("tidy") ("ON"))
  cmake --build --preset tidy --clean-first

format:
  @find src include tests -type f \
    \( -name '*.c' -o -name '*.h' -o -name '*.cpp' -o -name '*.hpp' \
    -o -name '*.cc' -o -name '*.hh' -o -name '*.cxx' -o -name '*.hxx' \) \
    -exec clang-format -i {} +

[unix]
install dir=bindir: (build ("release"))
  /usr/bin/install -d {{ dir }}
  /usr/bin/install -m 0755 "./{{ build_dir }}/release/src/{{ binary_name }}" {{ dir }}/{{ binary_name }}

[windows]
install dir=bindir:
  {{ error("Windows install not supported. Use WSL or install the binary manually.") }}

run preset=preset *args: (build (preset))
 {{ asan_env }} "./{{ build_dir }}/{{ preset }}/src/{{ binary_name }}" {{ args }}

destroy:
  rm -rf {{ crypt_dir }}
  rm -rf {{ config_dir }}
