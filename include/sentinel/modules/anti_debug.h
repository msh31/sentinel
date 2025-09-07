//
// Created by Marco on 07/09/2025.
//

#ifndef ANTI_DEBUG_H
#define ANTI_DEBUG_H

#include <sentinel/common.h>

class anti_debug
{
    public:

    private:
        struct DebuggerSignatures {
            std::vector<std::string> x64dbg = {"x64dbg.exe", "x32dbg.exe", "x64dbg.dll", "x32dbg.dll"};
            std::vector<std::string> OllyDbg = {"ollydbg.exe"};
            std::vector<std::string> WinDbg = {"windbg.exe", "cdb.exe", "ntsd.exe"};
            std::vector<std::string> IDA = {"ida.exe", "ida64.exe", "win32_dbg.dll"};
            std::vector<std::string> Immunity = {"ImmunityDebugger.exe"};
            std::vector<std::string> CheatEngine = {"cheatengine.exe"};
            std::vector<std::string> Scylla = {"scylla.exe", "scylla_hide.dll"};
            std::vector<std::string> ProcessTools = {"ProcessHacker.exe", "procexp.exe", "procmon.exe"};
            std::vector<std::string> Ghidra = {"ghidra.exe"};
        };
};

#endif //ANTI_DEBUG_H
