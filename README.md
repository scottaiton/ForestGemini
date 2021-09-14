# ForestGemini
ForestClaw wrapper for Gemini

In the current state of this project, gemini is a git submodule, and ForestClaw is exptected to be installed.

Update the submodule:

	 git submodule init
	 git submodule update

Configure with CMake in build directory

	cmake -DFORESTCLAW_ROOT=/path/to/forestclaw/install /path/to/src