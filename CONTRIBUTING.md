# Contributing to the Vernier Library

Thanks for taking the time to help. Bug reports, questions, fixes and new pattern types are all welcome.

By taking part in this project you agree to follow its [code of conduct](CODE_OF_CONDUCT.md).

## Reporting a bug or asking a question

Open an issue on [GitHub](https://github.com/vernierlib/vernier/issues). For a bug, please give the platform, compiler, CMake and OpenCV versions, the steps that reproduce it and what you expected to happen. If the problem shows up on a particular image or pattern file, attach it or a smaller one that still fails.

Questions about using the library can go in an issue too. For anything you would rather not post publicly, such as a commercial licence, write to [vernier@femto-st.fr](mailto:vernier@femto-st.fr).

## Suggesting a feature

Open an issue describing what you need and why before writing a large change, so we can agree on the approach first. Small fixes can go straight to a pull request.

## Making a change

1. Fork the repository and create a branch from `main`.
2. Build the library and the tests as described in the [README](README.md), then check that they pass:

```Shell
	> cmake -B build -DCMAKE_BUILD_TYPE=Release
	> cmake --build build --config Release --parallel
	> ctest --test-dir build -C Release --output-on-failure
```

3. Add a test in `test/` when you fix a bug or add a feature. Every `test/*.cpp` file is picked up by CMake and run by `ctest`, and the existing ones show how to use `UnitTest.hpp`.
4. Open a pull request against `main`. The CI builds and tests it on Ubuntu (x64 and arm64), Windows and macOS, and it needs to pass on all of them before it can be merged.

Keep a pull request to one topic, it makes review much quicker.

## Code style

Follow the style of the file you are editing: four space indentation, `camelCase` for methods and variables, `PascalCase` for classes, and everything inside the `vernier` namespace. New source files start with the same licence header as the existing ones. Public classes and methods are documented with Doxygen comments, since the [documentation](https://vernierlib.github.io/vernier) is generated from them.

## Licence

The library is distributed under the [GNU General Public License v3](LICENSE.txt). By submitting a contribution you agree that it is released under the same licence.
