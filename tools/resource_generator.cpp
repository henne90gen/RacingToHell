#include <fstream>
#include <iomanip>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        spdlog::error("Wrong number of arguments");
        return 1;
    }

    std::string input_file_name = argv[1];
    std::string symbol_name = argv[2];

    auto f = std::ifstream(input_file_name, std::ios::binary | std::ios::ate);
    if (!f.is_open()) {
        spdlog::error("Failed to open input file");
        return 1;
    }

    auto file_size = f.tellg();
    f.seekg(0);

    auto of = std::ofstream(symbol_name + ".cpp", std::ios::binary);
    if (!of.is_open()) {
        spdlog::error("Failed to open output file");
        return 1;
    }
    of << "#include \"Resources.h\"\n";
    of << "const unsigned char " << symbol_name << "[] = {\n";
    auto flags = of.flags();
    of << std::setfill('0');
    of << std::hex;

    char buf[256];
    size_t size = 0;
    size_t bytes_read = 0;
    size_t line_count = 0;
    bool file_too_large = file_size > 1 * 1024 * 1024;
    do {
        if (file_too_large) {
            of << "0xff";
            break;
        }

        f.read(buf, 256);
        bytes_read = f.gcount();
        size += bytes_read;
        for (int i = 0; i < bytes_read; i++) {
            of << "0x" << std::setw(2) << (0xFF & (unsigned int)buf[i]) << ", ";
            line_count++;
            if (line_count > 10) {
                of << "\n";
                line_count = 0;
            }
        }
    } while (f);

    of.flags(flags);
    of << "\n};\n";
    of << "const size_t " << symbol_name << "_len = " << size << ";\n";
    of << "Resource " << symbol_name << "_resource(" << symbol_name << ", " << symbol_name << "_len, ";
    if (file_too_large) {
        of << "true";
    } else {
        of << "false";
    }
    of << ");\n";

    return 0;
}
