#include <fstream>
#include <iomanip>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        spdlog::error("Wrong number of arguments");
        return 1;
    }

    std::string input_file_name = argv[1];
    std::string file_name = argv[2];
    std::string resource_name = argv[3];
    std::string identifier_name = argv[4];

    auto f = std::ifstream(input_file_name, std::ios::binary | std::ios::ate);
    if (!f.is_open()) {
        spdlog::error("Failed to open input file");
        return 1;
    }

    auto file_size = f.tellg();
    f.seekg(0);

    auto of = std::ofstream(file_name, std::ios::binary);
    if (!of.is_open()) {
        spdlog::error("Failed to open output file");
        return 1;
    }
    of << "#include <string>\n";
    of << "#include <string_view>\n";
    of << "#include \"Resources.h\"\n";
    of << "const std::string " << identifier_name << "_resource_name = \"" << resource_name << "\";";
    of << "const unsigned char " << identifier_name << "_content[] = {\n";
    auto flags = of.flags();
    of << std::setfill('0');
    of << std::hex;

    char buf[256];
    size_t size = 0;
    size_t line_count = 0;
    bool file_too_large = file_size > 1 * 1024 * 1024;
    do {
        if (file_too_large) {
            of << "0xff";
            break;
        }

        f.read(buf, 256);
        auto bytes_read = f.gcount();
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

    if (!file_too_large && size != file_size) {
        spdlog::error("Failed to read entire file");
        return 1;
    }

    of.flags(flags);
    of << "\n};\n";
    of << "const size_t " << identifier_name << "_length = " << size << ";\n";
    of << "Resource " << identifier_name << "_resource(";
    of << identifier_name << "_resource_name, ";
    of << "std::string_view((const char *)" << identifier_name << "_content, " << identifier_name << "_length), ";
    if (file_too_large) {
        of << "true";
    } else {
        of << "false";
    }
    of << ");\n";

    return 0;
}
