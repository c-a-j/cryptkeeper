# AGENTS.md

## Project Context

Crypt Keeper is a password manager intended to reach feature parity with `gopass`, while using an opaque storage model:

- Secret payloads are stored as individual files in a shared directory.
- Secret filenames are UUIDs (no semantic metadata in filenames/paths).
- Metadata and lookup structure are abstracted into an encrypted index file.

Agents should preserve this architectural direction in proposals and implementations.

## Project Structure

- `src/` contains implementation code:
  - `src/main.cpp` is the CLI entrypoint.
  - `src/cli/` builds and wires CLI commands/options.
  - `src/cmd/` contains command handlers (`init`, `get`, `config`, etc).
  - `src/util/` contains shared utilities (currently logging).
- `include/` mirrors the source layout for public/internal headers:
  - `include/cli/`, `include/cmd/`, `include/utils/`, etc.
- Root project files:
  - `CMakeLists.txt` defines the build.
  - `justfile` contains convenience task shortcuts.
  - `README.md` documents project goals and roadmap.
  - `AGENTS.md` defines agent behavior and constraints.
- `build/` is a generated build output directory (CMake artifacts and fetched
  deps like CLI11); agents should ignore this directory unless explicitly
  instructed otherwise

## Rules

### Editing
1. Do not edit any project file unless the user explicitly requests edits.
2. Suggestions must prioritize readability, modularity, and maintainability.
3. Suggestions and code changes must match the style and patterns already present in this repository.

### Libraries and Frameworks
- Avoid introducing new external dependencies unless absolutely necessary.
- If a new dependency is required, please state the reason.

### Licensing
- The project is licensed under the terms of the MIT license and we can only add compatible licenses. See .license-lint.yml for a list of compatible licenses.

## Default Agent Behavior

- Work in read-only mode by default: inspect, reason, and propose.
- If a requested task is ambiguous about edits, ask for confirmation before modifying files.
- When edits are requested, keep changes minimal and scoped to the task.
- Do not introduce broad refactors unless explicitly requested.

## Style Alignment Expectations

- Follow existing C++ conventions in this repo:
  - Header includes grouped cleanly.
  - Prefer small focused functions.
  - Favor straightforward control flow and early returns.
  - Keep logging and error handling consistent with existing command modules.
- Preserve naming and file organization patterns (`src/cmd`, `include/cmd`, `utils`, etc.).
- Avoid adding new frameworks, abstractions, or dependencies unless clearly justified and requested.
- Refer to https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines for best practice guidance

## Guidance for Architectural Suggestions

When proposing features or changes, bias toward:

- Clear module boundaries (index, crypto, storage, CLI, migration).
- Testable interfaces for index read/write, UUID mapping, and secret retrieval.
- Migration-safe design for future `pass`/`gopass` interoperability.
- Explicit threat-model-aware decisions (metadata leakage, key handling, integrity checks).

## Recommended Response Pattern

When asked for help without explicit edit permission:

1. Summarize understanding of the request.
2. Provide a concrete proposal (and optional patch sketch/diff).
3. Ask whether to apply the changes.