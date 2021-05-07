#include <iostream>
#include <string>
#include <fstream>
#include <vector>

std::string get_include_filename(std::string line)
{
    int i = 8;
    std::string file_name = "";

    while (line[i] == ' ')
    {
        ++i;
    }
    
    if (line[i] == '"')
    {
        ++i;
        while (line[i] != '"')
        {
            file_name += line[i];
            ++i;
        }
    }

    return file_name;
}

int main(int argc, char** argv)
{
    std::string file_name, extension;
    int idx = 0;
    bool before_period = true;
    while (argv[1][idx] != '\0')
    {
        if (argv[1][idx] == '.')
        {
            before_period = false;
        }

        if (before_period)
        {
            file_name += argv[1][idx];
        }
        else
        {
            extension += argv[1][idx];
        }
        
        ++idx;
    }

    std::ifstream input(file_name + extension);

    if (!input.is_open())
    {
        std::cerr << "could not open " << file_name << extension << std::endl;
        return -1;
    }

    std::ofstream connected_file(file_name + "_connected" + extension);

    if (!connected_file.is_open())
    {
        std::cerr << "could not open " << file_name << "_connected" << extension << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(input, line))
    {
        if (line.substr(0, 8) == "#include")
        {
            std::string include_file = get_include_filename(line);
            if (include_file != "")
            {
                std::ifstream part(include_file);
                if (!part.is_open())
                {
                    std::cerr << "could not open " << include_file << std::endl;
                    return -1;
                }

                // copy include file
                std::string buf;
                while (std::getline(part, buf))
                {
                    connected_file << buf << std::endl;
                }

                part.close();

                std::cout << "include : " << include_file << std::endl;
                continue;
            }
        }
        
        connected_file << line << std::endl;
    }

    input.close();
    connected_file.close();
    std::cout << "Competed connecting!" << std::endl;
}