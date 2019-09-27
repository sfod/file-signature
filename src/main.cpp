#include <iostream>

#include <boost/program_options.hpp>

#include "reader.hpp"
#include "processor.hpp"
#include "writer.hpp"
#include "hasher.hpp"


struct CMDOptions {
    std::string filename;
    std::string signature_filename;
    uint32_t block_size;
    uint64_t memory;
};

bool init(int argc, char **argv, CMDOptions &cmd_options)
{
    boost::program_options::options_description desc("Options");

    try {
        desc.add_options()
                ("help,h", "Show help message")
                ("file,f", boost::program_options::value<std::string>()->required(), "Input file")
                ("signature-file,s", boost::program_options::value<std::string>()->required(), "Output file to store signature")
                ("block,b", boost::program_options::value<uint32_t>()->default_value(1u<<20u), "The size of a block, in bytes")
                ("memory,m", boost::program_options::value<uint64_t>()->default_value(1u<<31u), "Memory limit for internal queue, in bytes")
        ;

        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help") > 0) {
            std::cout << "Usage: " << argv[0] << " <options>\n";
            std::cout << desc << std::endl;
            return false;
        }

        boost::program_options::notify(vm);

        cmd_options.filename = vm["file"].as<std::string>();
        cmd_options.signature_filename = vm["signature-file"].as<std::string>();
        cmd_options.block_size = vm["block"].as<uint32_t>();
        cmd_options.memory = vm["memory"].as<uint64_t >();
    }
    catch (std::exception &e) {
        std::cout << "Error on parsing cmd options: " << e.what() << "\n";
        std::cout << desc << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    CMDOptions cmd_options;

    if (!init(argc, argv, cmd_options)) {
        return EXIT_FAILURE;
    }

    try {
        signature::Writer writer(cmd_options.signature_filename);
        signature::Hasher hasher;
        signature::Processor processor(std::move(writer), std::move(hasher), cmd_options.memory);
        signature::Reader file_reader(cmd_options.filename, cmd_options.block_size);

        while (file_reader.has_next()) {
            processor.process(file_reader.next());
        }

        std::cout << "Finished reading\n";
    }
    // TODO Clean.
    catch (std::exception &e) {
        std::cerr << "Error occurred: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
