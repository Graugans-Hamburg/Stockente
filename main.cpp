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

#include "testscenarios.h"
#include "ECU_variable.h"
#include "tinyxml2.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> files_to_scan;
    std::vector<ECU_variable> CCP_Variables;
    std::string result_file_name;
    std::string map_file_name;

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
            git_version  = "Version  only known for a release";
            git_checksum = "Checksum only known for a release";
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
            if (arg.compare(str_target) == 0)       {argtype = 1;}
            if (arg.compare(str_mapfile) == 0)      {argtype = 2;}
            if (arg.compare(str_source) == 0)       {argtype = 3;}
            // Try to open the CPP file handed over with the first argument
            if(     arg.compare(str_target) != 0
                &&  arg.compare(str_mapfile) != 0
                &&  arg.compare(str_source) != 0)
            {
                switch (argtype)
                {
                    case 0:
                        std::cerr << "A unexpected argument had been entered in the function call" << std::endl;
                        return 6;
                    case 1:
                        result_file_name = arg;
                        cnt_target++;
                        if(cnt_target > 1)
                        {
                            std::cerr << "To many targetfiles had been defined inside the arguements" << std::endl;
                        }
                        break;
                    case 2:
                        map_file_name = arg;
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
                            }
                            else
                            {
                                std::cerr << "Source file could not be opened: " << arg << std::endl;
                            }
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

    for(unsigned int idx_j = 0; idx_j < files_to_scan.size(); idx_j++)
    {
        std::cout << "Start to analyse: " << files_to_scan.at(idx_j) << std::endl;
        std::ifstream src_code_file(files_to_scan.at(idx_j));
        if ( ! src_code_file)
        {
            std::cerr << "Source code file could not be opened" << std::endl;
            return 2;
        }





        std::string one_line_from_xml_file;
        std::string instruction;
        std::string str_comment;
        int source_code_line = 0;
        while(std::getline(src_code_file, one_line_from_xml_file))
        {
            source_code_line++;

            std::string::size_type n;
            n = one_line_from_xml_file.find("$CCP");
            if (n == std::string::npos) {
                //std::cout << "not found\n";
            } else
            {
            // CCP Tag found start to analyse the file.
            int result = 0;
            ECU_variable* tmp_variable = new(ECU_variable);


            result = check_T01_comment(one_line_from_xml_file);
            if(result >= 0)
            result = check_T02_one_comment(one_line_from_xml_file);
            if(result >= 0)
            result = check_T03_semicolon(one_line_from_xml_file);
            if(result >= 0)
            result = check_T04_extract_instruction(one_line_from_xml_file, instruction);
            if(result >= 0)
            result = check_T05_get_var_properties(instruction,*tmp_variable);
            if(result >= 0)
            result = check_T07_extract_comment(one_line_from_xml_file, str_comment);
            // Check the comment for the thresholds and the description and the unit
            if(result >= 0)
            check_T06_extract_thres(str_comment,*tmp_variable,min_value);
            if(result >= 0)
            check_T06_extract_thres(str_comment,*tmp_variable,max_value);
            if(result >= 0)
            check_T08_extract_str_element(str_comment,*tmp_variable,unit_value);
            if(result >= 0)
            check_T08_extract_str_element(str_comment,*tmp_variable,des_value);
            if(result < 0)
            {
                std::cout << "   Error detected in file: " << files_to_scan.at(idx_j) << std::endl;
                std::cout << "   Error detected in line: " << source_code_line << std::endl << std::endl;
            }
            if(result !=0)
                delete(tmp_variable);
            else
                CCP_Variables.push_back(*tmp_variable);
            }

        }

        if(src_code_file.is_open())
        {
            src_code_file.close();
        }

    }
    /*******************************************************************************************
     *
     * Extract the information from the map file
     *
     ******************************************************************************************/
    if(!map_file_name.empty())
    {
        std::cout << "Start to analyse the map file: " << map_file_name << std::endl;
        std::ifstream map_file(map_file_name);
        if ( ! map_file)
        {
            std::cerr << "Mapfile could not be opened" << std::endl;
            return 2;
        }

        std::string one_line_from_map_file;

        for(unsigned int idx_m = 0; idx_m < CCP_Variables.size(); idx_m++)
        {
            // Reset the file to the beginning- TODO could be better
            map_file.close();
            std::ifstream map_file(map_file_name);
            //
            ECU_variable* tmp_ECU_variable;
            tmp_ECU_variable = &CCP_Variables.at(idx_m);
            while(std::getline(map_file, one_line_from_map_file))
            {

                std::string::size_type pos_name_start;
                std::string::size_type pos_name_end;
                std::string::size_type pos_start_address;
                std::string::size_type pos_end_address;
                std::string ECUVariableName = tmp_ECU_variable->GetName();
                pos_name_start = one_line_from_map_file.find(tmp_ECU_variable->GetName());

                if (pos_name_start == std::string::npos)
                {
                    //std::cout << "not found\n";
                }
                else
                {
                    // CCP Tag found start to analyse the file.
                    pos_name_end = pos_name_start + ECUVariableName.length() -1 ;
                    pos_start_address = one_line_from_map_file.find("0x");
                    pos_end_address = one_line_from_map_file.find(" ", pos_start_address);
                    if(   pos_start_address != std::string::npos
                       && pos_start_address < pos_name_start
                       && check_if_a_letter_is_right_after(one_line_from_map_file,pos_name_end) == false
                       && check_if_a_letter_is_right_before(one_line_from_map_file,pos_name_start) == false)
                    {
                        // Address found
                        std::string str_address = one_line_from_map_file.substr(pos_start_address,pos_end_address);
                        unsigned long address = std::stoul(str_address,nullptr,0);
                        tmp_ECU_variable->SetAddress((uint32_t)(address) );

                    }
                }
            }

            if(map_file.is_open())
            {
                map_file.close();
            }
        }

    }
    else
    {
        std::cout << "No map file handed over as an argument. XML-File will be created without the"
                  << " addresses (all adresses zero)" << std::endl;
    }
    /*******************************************************************************************
     *
     * Print the written into a xmlfile
     *
     ******************************************************************************************/

    using namespace tinyxml2;

    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("measurement_file");
    xmlDoc.InsertFirstChild(pRoot);
    /* Save the ECU build Information */
    XMLElement * p_build_information = xmlDoc.NewElement("build_information");
        pRoot->InsertEndChild(p_build_information);
    XMLElement * p_created_by = xmlDoc.NewElement("created_by");
        p_build_information->InsertEndChild(p_created_by);
        p_created_by->SetText("Stockente - Unreleased Version had been used.");
    /* Define the ECU Properties */
    XMLElement * p_ecu_properties = xmlDoc.NewElement("ecu_properties");
        pRoot->InsertEndChild(p_ecu_properties);
    XMLElement * p_endianness = xmlDoc.NewElement("endianness");
        p_ecu_properties->InsertEndChild(p_endianness);
        p_endianness->SetText("little");
    XMLElement * p_station_address = xmlDoc.NewElement("station_address");
        p_ecu_properties->InsertEndChild(p_station_address);
        p_station_address->SetText("0");
    /* Define the ECU variable list */
    XMLElement * p_varible_list = xmlDoc.NewElement("variable_list");
        pRoot->InsertEndChild(p_varible_list);
    for(unsigned int idx_i = 0; idx_i < CCP_Variables.size(); idx_i++)
    {
        ECU_variable* tmp_ECU_variable;
        tmp_ECU_variable = &CCP_Variables.at(idx_i);

        XMLElement * pVariable = xmlDoc.NewElement("variable");
        p_varible_list->InsertEndChild(pVariable);
        XMLElement * pName = xmlDoc.NewElement("name");
            pVariable->InsertEndChild(pName);
            std::string tmp_str = tmp_ECU_variable->GetName();
            pName->SetText(tmp_str.c_str());
        XMLElement * pAddress = xmlDoc.NewElement("address");
            pVariable->InsertEndChild(pAddress);
            std::stringstream ss_Address;
            ss_Address << "0x"<<std::hex <<tmp_ECU_variable->GetAddress();
            std::string str_address = ss_Address.str();
            pAddress->SetText(str_address.c_str());
        XMLElement * pDataType = xmlDoc.NewElement("datatype");
            pVariable->InsertEndChild(pDataType);
            if(tmp_ECU_variable->GetDataType() == type_u8) pDataType->SetText("uint8");
            if(tmp_ECU_variable->GetDataType() == type_i8) pDataType->SetText("int8");
            if(tmp_ECU_variable->GetDataType() == type_u16) pDataType->SetText("uint16");
            if(tmp_ECU_variable->GetDataType() == type_i16) pDataType->SetText("int16");
            if(tmp_ECU_variable->GetDataType() == type_u32) pDataType->SetText("uint32");
            if(tmp_ECU_variable->GetDataType() == type_i32) pDataType->SetText("int32");
            if(tmp_ECU_variable->GetDataType() == type_f32) pDataType->SetText("float");
        if(!tmp_ECU_variable->str_MinValue.empty())
        {
        XMLElement * pMin = xmlDoc.NewElement("min");
            pVariable->InsertEndChild(pMin);
            std::stringstream ss_Min;
            ss_Min << tmp_ECU_variable->GetMinValue_F();
            std::string str_Min = ss_Min.str();
            pMin->SetText(str_Min.c_str());
        }
        if(!tmp_ECU_variable->str_MaxValue.empty())
        {
        XMLElement * pMax = xmlDoc.NewElement("max");
            pVariable->InsertEndChild(pMax);
            std::stringstream ss_Max;
            ss_Max << tmp_ECU_variable->GetMaxValue_F();
            std::string str_Max = ss_Max.str();
            pMax->SetText(str_Max.c_str());
        }
        if(!tmp_ECU_variable->str_Description.empty())
        {
        XMLElement * pDes = xmlDoc.NewElement("description");
            pVariable->InsertEndChild(pDes);
            std::string tmp_str = tmp_ECU_variable->GetDescription();
            pDes->SetText(tmp_str.c_str());
        }
        if(!tmp_ECU_variable->str_Unit.empty())
        {
        XMLElement * pUnit = xmlDoc.NewElement("unit");
            pVariable->InsertEndChild(pUnit);
            std::string tmp_str = tmp_ECU_variable->GetUnit();
            pUnit->SetText(tmp_str.c_str());
        }
    }

    XMLError eResult = xmlDoc.SaveFile(result_file_name.c_str());

    /* Start to print the results of the program execution */
    std::cout << std::endl << std::endl;
    std::cout << "------------Results------------------------------------" << std::endl;
    std::cout << "Number of CCP Variables: " << CCP_Variables.size() << std::endl;
    if(eResult == 0)
    {
        std::cout << "The VarFile creation was successful" << std::endl;
        std::cout << "New Varfile: "  << result_file_name << std::endl;
    }
    else{
        std::cout << "The VarFile creating was not successful" << std::endl;
        std::cerr << "Error[-001]:TinyXML could not save to the target file. "
                  << "TinyXML returns the follwing ErrorCode: "<< eResult
                  << " The file that should have been created is: " << result_file_name<< std::endl;
    }
    std::cout << "------------END----------------------------------------" << std::endl;
    return 0;
}
