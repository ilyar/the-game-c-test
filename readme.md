# The Game in pure C using raylib

Starter template for Step 1 of building The Game in pure C using [raylib](https://github.com/raysan5/raylib)

## Requirements

1. C compiler with C23 support (`GCC 13+` or `Clang 18+`)
2. [CMake](https://cmake.org/) 3.25 or newer

## How to Run

1. Clone the repository and navigate to the project directory:
```bash
git clone <REPOSITORY_URL>
cd pure-game
cmake -B cmake-build-debug
```

2. Build and run Desktop
```bash
cmake --build cmake-build-debug --target run
```

3. Build for Web
```bash
cmake --build cmake-build-debug --target web
```

4. Build and run Web locally
```bash
cmake --build cmake-build-debug --target web-run
```

## Assignment

1. Open `src/main.c`
2. Complete the instructions for **Step 1**
3. Verify your changes by running the game
4. Send a link to your repository with the solution to [@ilyarsoftware](https://t.me/ilyarsoftware) to receive the next step
