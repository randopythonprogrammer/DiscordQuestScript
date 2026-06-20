# DiscordQuestScript
A lightweight, automated C++ utility designed to clear Discord "Play to Earn" quests. 

Discord's game detection system actively scans the absolute file path and window titles of running processes to verify if you are playing a specific game. This tool bypasses those checks by automatically building the required nested directory structures (mimicking Steam or Epic Games), cloning itself as the target executable, and keeping an active console window open for the required quest duration.

## Features
* **Multi-Platform Support:** Quickly anchors to standard Steam (`common`) or Epic Games directories.
* **Dynamic Path Generation:** Automatically builds deep folder chains (e.g., `GOALS\games\Goals`).
* **Self-Cloning Process:** Copies itself into the faked directory under the official game executable name.
* **Smart Detection Loop:** Cycles the process every 20 seconds until Discord successfully detects it.
* **2-Strike Failsafe:** Automatically aborts if the user reports detection failure twice.
* **Auto-Cleanup:** Recursively deletes all generated fake directories and executables once the quest timer completes or the script fails.

---

## Disclaimer
**Use at your own risk.** This tool is for educational purposes to demonstrate how local process tracking and filesystem permissions work. Discord frequently updates its detection hooks, and bypassing their quest system may violate their Terms of Service.
**You may also run this script with admin privileges**, as it sometimes struggles to create folders and chained folders.

---

## Prerequisites
* **OS:** Windows 10/11
* **Compiler:** Any C++ compiler supporting **C++17** or higher (Visual Studio, MinGW, etc.) due to the `<filesystem>` library requirement.
* **Permissions:** Administrator access (required to write folders inside `C:\Program Files`).

---

## How to Compile
If you are using **GCC/MinGW** via command line, compile with the C++17 flag:
```bash
g++ -std=c++17 main.cpp -o DiscordQuestScript.exe
