# Build posgi yourself

## Building from Source (Linux / macOS)

### Prerequisites

#### Build dependencies

* [git](https://git-scm.com/)
* make, cmake
* clang++
* clang-format (code format only)
* cpplint (lint only)
* clang-tidy (lint only)
* curl (fuzz tests coverage only)
* lcov (fuzz tests coverage only)
* lldb (debugging only)

#### Test dependencies

* TODO(jhi): fill

### Ubuntu / Debian

```bash
sudo apt install git make cmake clang clang-format cpplint clang-tidy curl lcov lldb 
```

### macOS

```bash
brew install git make cmake clang clang-format cpplint clang-tidy curl lcov lldb
```

Finally, add the following to your `~/.zshrc` or `~/.bashrc` to use the correct version of
LLVM:

```bash
export PATH=$(brew --prefix)/opt/llvm/bin:$PATH
export LDFLAGS="-L$(brew --prefix)/opt/llvm/lib"
export CPPFLAGS="-I$(brew --prefix)/opt/llvm/include"
```

## Steps

To build **posgi** from source you have to execute the following steps:

```bash
git clone https://github.com/JochenHiller/posgi.git
cd posgi
# will check for tools and install all neded third_party libraries and tools
# when tools are missing check prerequisites first
./scripts/setup.sh clean check install
./build.sh clean all
# run all tests
./build.sh test
# run linters
./build.sh lint
# run fuzz tests, will run infinite, stop manually
./build.sh fuzztest-run
# generate HTML coverage report from fuzz tests
./build.sh fuzztest-report
```

### Windows

Not yet tested.
