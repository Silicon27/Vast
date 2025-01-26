//
// Created by David Yang on 2025-01-24.
//

#pragma once

inline std::fstream output;

inline void initializeOutputFile(const std::string& filename) {
    if (output.is_open()) {
        output.close();  // Close any previously opened file
    }
    output.open(filename, std::ios::out | std::ios::app);  // Open in append mode
    if (!output.is_open()) {
        throw std::runtime_error("Failed to open the file: " + filename);
    }
}

namespace toasm {
    namespace directives {
        // Directives
    }

    namespace dmov {
        // Data movement instructions

        // mov
        // add to the output file without the use of a function
        inline void _pmov(const std::string& from, const std::string& to) {
            output << "mov " << from << ", " << to << std::endl;
        }


    }

    namespace arith {
        // Arithmetic instructions
    }

    namespace logic {
        // Logical instructions
    }

    namespace comp {
        // Comparison instructions
    }

    namespace cflow {
        // Control flow and Branching instructions
    }

    namespace ldpMul {
        // Load and store multiple, instructions
    }

    namespace dproc {
        // Data processing instructions
    }

    namespace simd {
        // SIMD instructions
    }

    namespace fp {
        // Floating point instructions
    }

    namespace system {
        // System instructions
    }
}