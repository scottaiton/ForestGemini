# ForestGemini
ForestClaw wrapper for Gemini

## Setting up external libraries for gemini

```
git clone https://github.com/gemini3d/external
cmake -B build --install-prefix=/Users/zettergm/libs -Damr=true
cmake --build build -j
```

## Setting up libgemini and libgemini_c

```
git clone https://github.com/gemini3d/gemini3d
cmake -B build -DCMAKE_PREFIX_PATH=~/libs -Dcpp=true --install-prefix=/Users/zettergm/libs
cmake --build build -j
cmake --install build
```

## Compiling forestgemini

```
https://github.com/ECLAIRWaveS/ForestGemini.git
cmake -B build -DCMAKE_PREFIX_PATH=~/libs
cmake --build build -j
```