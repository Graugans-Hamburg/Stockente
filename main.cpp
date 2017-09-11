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

        // Die falsche Anzahl von Argumenten wurde der Funktion mit übergeben.
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
        std::cout << "------------Program: Stockente----------------" << std::endl
                  << "Version      : " << git_version << std::endl
                  << "Git-Checksum : " << git_checksum <<std::endl
                  << "----------------------------------------------" << std::endl;
        return 0;
    }

    if(argc == 3)
    {
        // Überprüfe ob das erst Argument eine Datei beschreibt welche auch geäffnet werden kann.

        std::ifstream testf(argv[1]);
        if( testf.good() == true)
            testf.close();
        else
        {
            std::cerr << "Das erste Argument war leider keine Datei welche geöffnet werden konnte.";
            return 4;
        }

        // Try to open the CPP file handed over with the first argument
        files_to_scan.push_back(argv[1]);
        // Datei in welcher die Ergebnisse gespeichert werden sollen
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
                        std::cerr << "Ein unerwartetes Argument wurde übergeben" << std::endl;
                        return 6;
                    case 1:
                        result_file_name = arg;
                        cnt_target++;
                        if(cnt_target > 1)
                        {
                            std::cerr << "Zu viele Zieldateien wurden angegeben" << std::endl;
                        }
                        break;
                    case 2:
                        map_file_name = arg;
                        cnt_map++;
                        if(cnt_map > 1)
                        {
                            std::cerr << "Zu viele Mapdateien wurden angegeben" << std::endl;
                            return 7;
                        }
                        {
                            std::ifstream testf(arg);
                            if( testf.good() == true)
                                testf.close();
                            else
                            {
                                std::cerr << "Mapdatei konnte nicht geöffnet werden: " << arg << std::endl;
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
                                std::cerr << "Quelltextdatei konnte nicht geöffnet werden: " << arg << std::endl;
                            }
                        }
                        break;
                    default:
                        std::cerr << "Programmierfehler" << std::endl;
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
        while(std::getline(src_code_file, one_line_from_xml_file))
        {

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
            // Überprüfe den Kommentar
            check_T06_extract_thres(str_comment,*tmp_variable,min_value);
            check_T06_extract_thres(str_comment,*tmp_variable,max_value);
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

            std::string::size_type pos_name;
            std::string::size_type pos_start_address;
            std::string::size_type pos_end_address;
            pos_name = one_line_from_map_file.find(tmp_ECU_variable->GetName());
            if (pos_name == std::string::npos)
            {
                //std::cout << "not found\n";
            }
            else
            {
                // CCP Tag found start to analyse the file.
                pos_start_address = one_line_from_map_file.find("0x");
                pos_end_address = one_line_from_map_file.find(" ", pos_start_address);

                if(pos_start_address != std::string::npos
                   && pos_start_address < pos_name)
                {
                    // Addresse wurde gefunden
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

    /*******************************************************************************************
     *
     * Print the written into a xmlfile
     *
     ******************************************************************************************/

    using namespace tinyxml2;

    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("measurement_file");
    xmlDoc.InsertFirstChild(pRoot);

    for(unsigned int idx_i = 0; idx_i < CCP_Variables.size(); idx_i++)
    {
        ECU_variable* tmp_ECU_variable;
        tmp_ECU_variable = &CCP_Variables.at(idx_i);

        XMLElement * pVariable = xmlDoc.NewElement("variable");
        pRoot->InsertEndChild(pVariable);
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
        }
        if(!tmp_ECU_variable->str_Unit.empty())
        {
        XMLElement * pUnit = xmlDoc.NewElement("unit");
            pVariable->InsertEndChild(pUnit);
        }
    }

    XMLError eResult = xmlDoc.SaveFile(result_file_name.c_str());

    return 0;
}
