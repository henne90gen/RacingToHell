#include <filesystem>
#include <fstream>
#include <iomanip>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    if ((argc - 1) % 3 != 0) {
        spdlog::error("Wrong number of arguments");
        return 1;
    }

    auto of = std::ofstream("resources.cpp", std::ios::binary);
    if (!of.is_open()) {
        spdlog::error("Failed to open output file");
        return 1;
    }

    of << "#include \"Resources.h\"\n";
    of << "#include <string>\n";
    of << "#include <unordered_map>\n";
    of << "\n";

    for (int i = 1; i < argc; i += 3) {
        std::string resource_name = argv[i];
        std::string identifier_name = argv[i + 1];
        std::string input_file_name = argv[i + 2];

        auto last_write_time = std::filesystem::last_write_time(input_file_name);
        auto timestamp =
            std::chrono::time_point_cast<std::chrono::milliseconds>(last_write_time).time_since_epoch().count();
        of << "Resource " << identifier_name << "_resource(";
        of << "\"" << resource_name << "\", " << timestamp << ");\n";
    }

    of << "\n";
    of << "extern std::unordered_map<std::string, Resource *> resources;\n";
    of << "\n";

    of << "void init_resources() {\n";
    for (int i = 1; i < argc; i += 3) {
        std::string identifier_name = argv[i + 1];
        std::string resource_variable_name = identifier_name + "_resource";
        of << "    resources[" << resource_variable_name << ".resource_name] = &" << resource_variable_name << ";\n";
    }
    of << "}\n";

    of.close();

    return 0;
}
