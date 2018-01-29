/*
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with CCP driver.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: Sep 9, 2017
 *      Author: Matthias Baumann
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include "ECU_variable.h"
#include "scan_source_files.h"
#include "tinyxml2.h"
#include "ECUXMLCreator.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> files_to_scan;
    std::vector<ECU_variable> CCP_Variables;
    std::string result_file_name;
    std::string map_file_name;
    std::string endianess;

    ECUXMLCreator xml_creator;

    if(argc < 2 )
    {

        // The wrong number of arguments had been handed over with the function call
        std::cerr << "No Argument found." << std::endl;
        return 1;
    }

    if(argc == 2 )
    {
        std::string arg = argv[1];
        std::string str_target = "-v";
        std::string git_version;
        std::string git_checksum;


        if (arg.compare(str_target) == 0)
        {
            git_version  = "v1.4";
            git_checksum = "b2ee096fbc0b98a58bc24f0b86a7fc8d7f2b4a93";
        }
        // Die falsche Anzahl von Argumenten wurde der Funktion mit übergeben.
        std::cout << "------------Program: Stockente-------------------------" << std::endl
                  << "Version      : " << git_version << std::endl
                  << "Git-Checksum : " << git_checksum <<std::endl
                  << "-------------------------------------------------------" << std::endl;
        return 0;
    }

    if(argc == 3)
    {
        // Check if the first argument is path to a file which can be opened.

        std::ifstream testf(argv[1]);
        if( testf.good() == true)
            testf.close();
        else
        {
            std::cerr << "The first argument was not a file which could be opened.";
            return 4;
        }

        // Try to open the CPP file handed over with the first argument
        files_to_scan.push_back(argv[1]);
        // File to save the result in.
        result_file_name = argv[2];
    }
    else
    {
        unsigned int argtype = 0;
        unsigned int cnt_target = 0,cnt_map = 0, cnt_src;
        for( int idx_k = 1; idx_k < argc; idx_k++ )
        {

            std::string arg = argv[idx_k];
            std::string str_target = "-t";
            std::string str_mapfile = "-x";
            std::string str_source = "-s";
            std::string str_endianess = "-e";
            if (arg.compare(str_target) == 0)       {argtype = 1;}
            if (arg.compare(str_mapfile) == 0)      {argtype = 2;}
            if (arg.compare(str_source) == 0)       {argtype = 3;}
            if (arg.compare(str_endianess) == 0)    {argtype = 4;}
            // Try to open the CPP file handed over with the first argument
            if(     arg.compare(str_target) != 0
                &&  arg.compare(str_mapfile) != 0
                &&  arg.compare(str_source) != 0
                &&  arg.compare(str_endianess) != 0)

            {
                switch (argtype)
                {
                    case 0:
                        std::cerr << "A unexpected argument had been entered in the function call" << std::endl;
                        return 6;
                    case 1:
                        xml_creator.Set_result_file_name(arg);
                        cnt_target++;
                        if(cnt_target > 1)
                        {
                            std::cerr << "To many targetfiles had been defined inside the arguements" << std::endl;
                        }
                        break;
                    case 2:
                        xml_creator.Set_map_file_name(arg);
                        cnt_map++;
                        if(cnt_map > 1)
                        {
                            std::cerr << "The function calll is only possible with one map file, but more than one had been defined inside the argument list." << std::endl;
                            return 7;
                        }
                        {
                            std::ifstream testf(arg);
                            if( testf.good() == true)
                                testf.close();
                            else
                            {
                                std::cerr << "Mapfile could not be opened: " << arg << std::endl;
                                return 8;
                            }
                        }
                        break;
                    case 3:
                        cnt_src++;
                        {
                            std::ifstream testf(arg);
                            if( testf.good() == true)
                            {
                                testf.close();
                                files_to_scan.push_back(arg);
                                xml_creator.Add_source_code(arg);
                            }
                            else
                            {
                                std::cerr << "Source file could not be opened: " << arg << std::endl;
                            }
                        }
                        break;
                    case 4:
                        {
                            xml_creator.Set_endianness(arg);
                            break;
                        }
                        break;
                    default:
                        std::cerr << "Stockente, Bug detected." << std::endl;
                        return 9;
                        break;

                }
            }

        }
    }

    xml_creator.ScanSourceCodeFiles();
    xml_creator.ScanAddressMapFiles();
    xml_creator.CreateXMLFile();
    xml_creator.PlotResults2Terminal();

    return 0;
    }
