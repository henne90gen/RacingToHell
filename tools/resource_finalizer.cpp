#include <fstream>
#include <spdlog/spdlog.h>

int main(int argc, char **argv) {
    if (argc <= 1) {
        spdlog::error("Too few arguments");
        return 1;
    }

    std::string output_file_name = "resources.cpp";
    auto of = std::ofstream(output_file_name);
    if (!of.is_open()) {
        spdlog::error("Failed to open {}", output_file_name);
        return 1;
    }

    of << "#include \"Resources.h\"\n\n";
    of << "#include <string>\n";
    of << "#include <unordered_map>\n";
    of << "\n";
    for (int i = 1; i < argc; i++) {
        std::string file_name = argv[i];
        file_name = file_name.substr(0, file_name.size() - 4);
        of << "extern Resource " << file_name << "_resource;\n";
    }
    of << "\n";
    of << "extern std::unordered_map<std::string, Resource *> resources;\n";
    of << "\n";
    of << "void init_resources() {\n";
    for (int i = 1; i < argc; i++) {
        std::string file_name = argv[i];
        file_name = file_name.substr(0, file_name.size() - 4);
        auto resource_name = file_name + "_resource";
        of << "    resources[" << resource_name << ".resource_name] = &" << resource_name << ";\n";
    }
    of << "}\n";
    of.close();

    return 0;
}
