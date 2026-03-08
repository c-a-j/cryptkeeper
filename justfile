set shell := ["bash", "-cu"]

build_dir := "build"
build_type := "Debug"
binary := build_dir + "/src/ck"
vault_dir := "~/.local/share/crypt-keeper"
config_dir := "~/.config/crypt-keeper"

configure tests="OFF":
  cmake -S . -B {{ build_dir }} -DCK_ENABLE_SANITIZERS=OFF -DCMAKE_BUILD_TYPE={{ build_type }} -DBUILD_TESTING={{ tests }}

build tests="OFF": (configure (tests))
  cmake --build {{ build_dir }}

run *args: build
  ./{{binary}} {{ args }}

test filter="": (build ("ON"))
  cd {{ build_dir }} && ctest --output-on-failure {{ if filter != "" { "-R " + filter } else { "" }}} 

destroy:
  rm -rf {{ vault_dir }}
  rm -rf {{ config_dir }}
