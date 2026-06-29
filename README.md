# hackathon-GBemulator

Émulateur Game Boy écrit en C++20, rendu graphique via [raylib](https://github.com/raysan5/raylib).

## Prérequis

- **CMake** ≥ 3.16
- Un compilateur **C++20** (GCC, Clang, AppleClang ou MSVC)
- **raylib** (facultatif) : s'il n'est pas installé, CMake le télécharge et le
  compile automatiquement.

## Build & lancement (Makefile)

```bash
make            # configure + compile -> build/gbemu
make run        # compile puis lance avec roms/Tetris.gb
make run ROM=chemin/vers.gb   # lance avec une autre ROM
make clean      # supprime build/
```

## Build manuel (CMake)

```bash
cmake -S . -B build
cmake --build build -j
./build/gbemu roms/Tetris.gb
```

Une fenêtre à l'échelle de l'écran Game Boy (160×144, ×4) s'ouvre.
Fermer la fenêtre ou appuyer sur **Échap** pour quitter.
