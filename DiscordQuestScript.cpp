#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;

// --- GAME SIMULATION MODE ---
void runSpoofMode(const std::string& title) {
    SetConsoleTitleA(title.c_str());

    std::cout << "==================================================\n";
    std::cout << " [SPOOFING ACTIVE] Window Title: " << title << "\n";
    std::cout << " Status: Simulating active game loop...\n";
    std::cout << " DO NOT CLOSE THIS WINDOW MANUALLY.\n";
    std::cout << "==================================================\n";

    // Keep alive for 25 minutes
    std::this_thread::sleep_for(std::chrono::minutes(25));
}

// --- MASTER CONTROLLER ---
int main(int argc, char* argv[]) {
    // Jump straight to spoof execution if the internal flag is passed
    if (argc > 2 && std::string(argv[1]) == "--spoof") {
        std::string title = "";
        for (int i = 2; i < argc; ++i) {
            title += argv[i];
            if (i < argc - 1) title += " ";
        }
        runSpoofMode(title);
        return 0;
    }

    int platformChoice = 0;
    std::string gameTitle, relativePath, exeName;
    fs::path basePath;
    int failCount = 0; // Tracks the number of consecutive 'N' selections

    std::cout << "=== MULTI-PLATFORM DISCORD QUEST AUTOMATOR ===\n\n";
    std::cout << " Select Game Platform:\n";
    std::cout << "  [1] Steam (C:\\Program Files (x86)\\Steam\\steamapps\\common\\)\n";
    std::cout << "  [2] Epic Games (C:\\Program Files\\Epic Games\\)\n";
    std::cout << "Choice (1 or 2): ";
    std::cin >> platformChoice;
    std::cin.ignore(); // Clear the newline character from buffer

    // Assign Base Directories
    if (platformChoice == 1) {
        basePath = "C:\\Program Files (x86)\\Steam\\steamapps\\common";
    }
    else if (platformChoice == 2) {
        basePath = "C:\\Program Files\\Epic Games";
    }
    else {
        std::cerr << "[-] Invalid selection. Exiting.\n";
        return 1;
    }

    std::cout << "\n[?] Enter Game Title (Exactly as shown on Discord): ";
    std::getline(std::cin, gameTitle);

    std::cout << "[?] Enter Relative Subfolders (e.g., Zenless Zone Zero\\games\\ZenlessZoneZero Game): ";
    std::getline(std::cin, relativePath);

    std::cout << "[?] Enter Executable Name (e.g., ZenlessZoneZero.exe): ";
    std::getline(std::cin, exeName);

    if (exeName.size() < 4 || exeName.substr(exeName.size() - 4) != ".exe") {
        exeName += ".exe";
    }

    // Combine paths dynamically
    fs::path targetDirectory = basePath / relativePath;
    fs::path targetExePath = targetDirectory / exeName;

    // Build the directories recursively and copy the executable
    try {
        if (!fs::exists(targetDirectory)) {
            fs::create_directories(targetDirectory);
            std::cout << "\n[+] Built nested folder tree: " << targetDirectory.string() << "\n";
        }

        char currentBinaryPath[MAX_PATH];
        GetModuleFileNameA(NULL, currentBinaryPath, MAX_PATH);

        fs::copy_file(currentBinaryPath, targetExePath, fs::copy_options::overwrite_existing);
        std::cout << "[+] Cloned automated dummy launcher to target directory.\n\n";

    }
    catch (const std::exception& e) {
        std::cerr << "[-] File System Error (Try running as Admin): " << e.what() << "\n";
        return 1;
    }

    // --- DETECTION RETRY LOOP ---
    while (true) {
        std::cout << "[*] Launching dummy process to hit Discord tracking hooks...\n";

        std::string fullPathStr = "\"" + targetExePath.string() + "\"";
        std::string cmdArgs = " --spoof \"" + gameTitle + "\"";
        std::string combinedCmd = fullPathStr + cmdArgs;

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        char* writablePath = new char[combinedCmd.length() + 1];
        strcpy_s(writablePath, combinedCmd.length() + 1, combinedCmd.c_str());

        // Spawn the fake game inside its own dedicated visible console window
        if (!CreateProcessA(NULL, writablePath, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cerr << "[-] Failed to execute spoof layer. Error: " << GetLastError() << "\n";
            delete[] writablePath;
            return 1;
        }
        delete[] writablePath;

        // Discord scan buffer countdown
        std::cout << "[*] Starting 20-second connection cooldown...\n";
        for (int i = 20; i > 0; --i) {
            std::cout << "\rTime remaining for scan window: " << i << "s   " << std::flush;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "\n\n";

        // Verification Interface
        std::cout << "========================================================\n";
        std::cout << " STATUS CHECK:\n";
        std::cout << " Is Discord currently displaying \"Playing " << gameTitle << "\"?\n";
        std::cout << "   [Y] -> Yes, lock it in (Runs quietly for 25 mins)\n";
        std::cout << "   [N] -> No, kill process and re-cycle to trigger re-scan\n";
        std::cout << "========================================================\n";
        std::cout << "Choice: ";

        char choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 'Y' || choice == 'y') {
            std::cout << "\n[+] Connection verified. Lock mode initialized.\n";
            std::cout << "[*] This controller will auto-close when the reward timer finishes.\n";

            WaitForSingleObject(pi.hProcess, INFINITE);

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            break;
        }
        else {
            std::cout << "\n[-] Discord missed it. Killing instance and cycling launcher...\n\n";
            TerminateProcess(pi.hProcess, 0);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }
    // --- FUNCTIONALITY 1: Automated Directory Annihilation ---
    std::cout << "\n[*] Initiating post-execution directory deletion sequence...\n";
    try {
        if (fs::exists(targetDirectory)) {
            // Force deletes everything inside the created tree path safely
            fs::remove_all(targetDirectory);
            std::cout << "[+] Successfully deleted created directory: " << targetDirectory.string() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[-] Clean-up Warning: Could not delete folders automatically. Details: " << e.what() << "\n";
        std::cerr << "[-] You may need to delete the folder manually if Windows keeps it locked.\n";
    }

    std::cout << "[+] Quest loop finalized.";
    return 0;
}