set shell := ["bash", "-cu"]

build_dir := "build"
build_type := "Debug"
binary := build_dir + "/src/ck"
vault_dir := "~/.local/share/crypt-keeper"
config_dir := "~/.config/crypt-keeper"
asan_env := "ASAN_SYMBOLIZER_PATH=" + `which llvm-symbolizer` + " ASAN_OPTIONS=symbolize=1"

configure sanitizers="OFF" tests="OFF":
  cmake -S . -B {{ build_dir }} -DCMAKE_CXX_COMPILER=clang++ -DCK_ENABLE_SANITIZERS={{ sanitizers }} -DCMAKE_BUILD_TYPE={{ build_type }} -DBUILD_TESTING={{ tests }}

build tests="OFF": (configure "ON" (tests))
  cmake --build {{ build_dir }}

release: (configure ("OFF"))
  cmake --build {{ build_dir }}

run *args: build
 {{ asan_env }} ./{{binary}} {{ args }}

test filter="": (build ("ON"))
  cd {{ build_dir }} && ctest --output-on-failure {{ if filter != "" { "-R " + filter } else { "" }}} 

destroy:
  rm -rf {{ vault_dir }}
  rm -rf {{ config_dir }}
