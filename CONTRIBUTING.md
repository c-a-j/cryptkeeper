# Contributing

Crypt Keeper uses GitHub to manage reviews of pull requests.

* If you are a new contributor see: [Steps to Contribute](#steps-to-contribute)

* If you have a trivial fix or improvement, go ahead and create a pull request.

* If you plan to do something more involved, first raise an issue to discuss
  your idea. This will avoid unnecessary work.

* Relevant coding style guidelines are  the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)

## Steps to Contribute

Should you wish to work on an issue, please claim it first by commenting on the GitHub issue you want to work on it.
This will prevent duplicated efforts from contributors.

Please check the [`help-wanted`](https://github.com/c-a-j/cryptkeeper/issues?q=state%3Aopen%20label%3A%22help%20wanted%22) label to find issues that need help.
If you have questions about one of the issues please comment on them and one of
the maintainers will try to clarify it.

## Pull Request Checklist

* Branch from master and, if needed, rebase to the current master branch before submitting your pull request.
  If it doesn't merge cleanly with master you will be asked to rebase your changes.

* Commits should be as small as possible, while ensuring that each commit is correct independently.

* Add tests relevant to the fixed bug or new feature.

* Commit messages must contain [Developer Certificate of Origin](https://developercertificate.org/) / `Signed-off-by` line, for example:

      One line description of commit

      More detailed description of commit, if needed.

      Signed-off-by: Your Name <your@email.com>

* The first line of the commit message, the subject line, should be prefix with a tag indicating the type of the change. These tags will be extracted and used to populate the changelog.
  Valid `[TAG]`s are `[BREAKING]`, `[BUGFIX]`, `[CLEANUP]`, `[DEPRECATION]`,
  `[DOCUMENTATION]`, `[ENHANCEMENT]`, `[FEATURE]`, `[TESTING]`, and `[UX]`.

## Building & Testing

* Build via `just build` to create the binary file `./buils/src/ck`.
* Run tests with: `just test`
* Run clang-tidy with: `make lint`

## Releasing

See [docs/releases.md](docs/releases.md).

* **Note**: Most of this document was borrowed from `gopass`.