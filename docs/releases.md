## Releases

### Versioning

Crypt Keeper uses semantic versioning. Bump the version according to the
nature of the changes:

- **patch**: bug fixes, typos, minor corrections
- **minor**: new features, new commands, non-breaking changes
- **major**: breaking changes to CLI behavior, vault format, or config schema

### Cutting a release

1. Ensure all tests pass:
   ```bash
   just test
   ```

2. Run a clean lint check:
   ```bash
   just lint
   ```

3. Update the version in `CMakeLists.txt`:
   ```cmake
   project(crypt-keeper VERSION x.y.z LANGUAGES CXX)
   ```

4. Commit the version bump and tag the release:
   ```bash
   git commit -am "release vx.y.z"
   git tag vx.y.z
   ```

5. Push the tag:
   ```bash
   git push origin master --tags
   ```

### Build artifacts

Release builds are created with:
```bash
just release
```

This produces an optimized binary with sanitizers, debug logs, and clang-tidy
disabled.
