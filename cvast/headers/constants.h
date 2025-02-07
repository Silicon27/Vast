//
// Created by David Yang on 2025-01-22.
//

#pragma once

namespace cvast {
    namespace constants {
        // Constants for the project
        const std::string VER = "1.0.0";
        const std::string LICENSE = "MIT";

        class Null {
        private:
            Null()= default;
        public:
            static int null;
        };

        int Null::null = 0;
    }

    namespace syntax {
        const std::string KEYWORDS[] = {
            "create", "expand", "export", "declare", "if", "elif"
        };
    }
}
