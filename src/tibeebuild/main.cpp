/* Copyright (c) 2014 Philippe Proulx <eepp.ca>
 *
 * This file is part of tigerbeetle.
 *
 * tigerbeetle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tigerbeetle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tigerbeetle.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "BuilderBeetle.hpp"
#include "Arguments.hpp"

namespace bfs = boost::filesystem;

namespace
{

/**
 * Parses the command line arguments passed to the program.
 *
 * @param argc Number of arguments in \p argv
 * @param argv Command line arguments
 * @param args Arguments values to fill
 *
 * @returns    0 to continue, 1 if there's a command line error
 */
int parseOptions(int argc, char* argv[], tibee::Arguments& args)
{
    namespace bpo = boost::program_options;

    bpo::options_description desc;

    desc.add_options()
        ("help,h", "help")
        ("traces,T", bpo::value<std::vector<std::string>>())
        ("verbose,v", bpo::bool_switch()->default_value(false))
        ("stateprov,s", bpo::value<std::vector<std::string>>()->multitoken())
        ("bind-progress,b", bpo::value<std::string>())
        ("cache-dir,d", bpo::value<std::string>())
    ;

    bpo::positional_options_description pos;

    pos.add("traces", -1);

    bpo::variables_map vm;

    try {
        auto cliParser = bpo::command_line_parser(argc, argv);
        auto parsedOptions = cliParser.options(desc).positional(pos).run();

        bpo::store(parsedOptions, vm);
    } catch (const std::exception& ex) {
        std::cerr << "Command line error: " << ex.what() << std::endl;
        return 1;
    }

    if (!vm["help"].empty()) {
        std::cout <<
            "usage: tibeebuild [options] <trace path>..." << std::endl <<
            std::endl <<
            "options:" << std::endl <<
            std::endl <<
            "  -h, --help           print this help message" << std::endl <<
            "  -b, --bind-progress  bind address for build progress (default: none)" << std::endl <<
            "  -d, --cache-dir      write caches to this directory (default: CWD)" << std::endl <<
            "  -s                   state provider file path (at least one)" << std::endl <<
            "  -v, --verbose        verbose" << std::endl;

        return -1;
    }

    try {
        vm.notify();
    } catch (const std::exception& ex) {
        std::cerr << "Command line error: " << ex.what() << std::endl;
        return 1;
    }

    // traces
    if (vm["traces"].empty()) {
        std::cerr << "Command line error: need at least one trace file to work with" << std::endl;
        return 1;
    }

    auto traces = vm["traces"].as<std::vector<std::string>>();

    for (const auto& trace : traces) {
        bfs::path p {trace};

        if (!bfs::exists(p)) {
            std::cerr << "Trace path " << p << " does not exist" << std::endl;
            return 1;
        }

        args.traces.push_back(p);
    }

    // cache directory
    bfs::path cacheDirPath = bfs::current_path();

    if (!vm["cache-dir"].empty()) {
        cacheDirPath = vm["cache-dir"].as<std::string>();
    }

    if (bfs::exists(cacheDirPath) && bfs::is_directory(cacheDirPath)) {
        args.cacheDir = cacheDirPath;
    } else {
        std::cerr << "Cache output directory " << cacheDirPath << " is not a directory" << std::endl;
        return 1;
    }

    // state providers
    if (vm["stateprov"].empty()) {
        std::cerr << "Command line error: need at least one state provider to work with" << std::endl;
        return 1;
    }

    auto stateProviders = vm["stateprov"].as<std::vector<std::string>>();

    for (const auto& stateProvider : stateProviders) {
        bfs::path p {stateProvider};

        if (bfs::exists(p) && !bfs::is_directory(p)) {
            args.stateProviders.push_back(p);
        } else {
            std::cerr << "State provider " << p << " is not a file" << std::endl;
            return 1;
        }
    }

    // bind progress
    if (!vm["bind-progress"].empty()) {
        args.bindProgress = vm["bind-progress"].as<std::string>();
    }

    // verbose
    args.verbose = vm["verbose"].as<bool>();

    return 0;
}

}

int main(int argc, char* argv[])
{
    tibee::Arguments args;

    int ret = parseOptions(argc, argv, args);

    if (ret < 0) {
        return 0;
    } else if (ret > 0) {
        return ret;
    }

    // create the builder beetle and run it
    std::unique_ptr<tibee::BuilderBeetle> builderBeetle {new tibee::BuilderBeetle {args}};

    builderBeetle->run();

    return 0;
}
