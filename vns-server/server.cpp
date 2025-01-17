#include "httplib.h"
#include "extern/vns-cpp/compiler.hpp"
#include "extern/vns-cpp/schema.hpp"
#include "extern/vns-cpp/naming.hpp"
#include "extern/vns-cpp/validator.hpp"
#include "extern/vns-cpp/version.hpp"

// HTTP
int main(const int argc, char **argv)
{
    // process arguments
    const std::unordered_set<std::string> arguments_with_input = {"-p", "-a"};
    const std::unordered_set<std::string> arguments_without_input = {"-d"};
    std::unordered_map<std::string, std::string> arguments_map;
    for (size_t i = 1; i < argc; ++i)
    {
        if (std::string current_arg = argv[i]; arguments_with_input.contains(current_arg))
        {
            arguments_map[current_arg] = argv[++i];
        } else if (arguments_without_input.contains(current_arg))
        {
            arguments_map[current_arg].clear();
        } else
        {
            std::cout << "Unknown argument " << current_arg << ", ignored." << std::endl;
        }
    }

    // address
    const std::string ADDRESS = arguments_map.contains("-a") ? arguments_map.at("-a") : "0.0.0.0";

    // port
    const int PORT = arguments_map.contains("-p") ? stoi(arguments_map.at("-p")) : 8181;

    // if show debug info
    const bool DEBUG = arguments_map.contains("-d");

    // Create a server object
    httplib::Server svr;

    //Handling compile request
    svr.Post("/post/compile", [&DEBUG](const httplib::Request &req, httplib::Response &res) {
        // Output the request body
        if (DEBUG)
            std::cout << "Received POST request with body: " << req.body << std::endl;

        // parse body to json
        const nlohmann::json req_j = nlohmann::json::parse(req.body);

        // update naming database
        if (req_j.contains("namings"))
            Naming::update_database(req_j["namings"].get<std::unordered_map<std::string, std::vector<std::string>>>());

        // compile raw vns data
        const std::string compiled = req_j.contains("data") ? Compiler::load_as_string(req_j["data"].get<std::string>())
                                                            : std::string();

        if (DEBUG)
            std::cout << "result: " << compiled << std::endl;

        // Respond with compile json data
        res.set_content(compiled, "application/json");
    });

    // Handling vns format json validation request
    svr.Post("/post/validate", [](const httplib::Request &req, httplib::Response &res) {
        // Respond with result
        const nlohmann::json response = {
                {"result", !req.body.empty() && Validator::validate(nlohmann::json::parse(req.body))}};

        // return result
        res.set_content(response.dump(), "application/json");
    });

    // Get schema request
    svr.Get("/get/schema", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(VNS_SCHEMA, "application/json");
    });

    // Get compiler version
    svr.Get("/get/version", [](const httplib::Request &, httplib::Response &res) {
        const nlohmann::json response = {{"version",   VERSION},
                                         {"reversion", REVISION},
                                         {"patch",     PATCH}};
        res.set_content(response.dump(), "application/json");
    });

    // Start the server on given address (at localhost:8181 by default)
    std::cout << "Starting server at http://" << ADDRESS << ':' << PORT << std::endl;

    // listen the port
    svr.listen(ADDRESS, PORT);

    return 0;
}