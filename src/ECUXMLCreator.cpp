#include "ECUXMLCreator.h"

ECUXMLCreator::ECUXMLCreator()
{
    //Default values
    station_address = 0;
    endianness = "little";
    git_version  = "v1.4";
    git_checksum = "b2ee096fbc0b98a58bc24f0b86a7fc8d7f2b4a93";
}

ECUXMLCreator::~ECUXMLCreator()
{
    //dtor
}


void ECUXMLCreator::Add_source_code(std::string source_file_name_full_path)
{
    files_to_scan.push_back(source_file_name_full_path);
}

/*******************************************************************************************
* Function: Scan all source code file and try to find the variables
******************************************************************************************/
int ECUXMLCreator::ScanSourceCodeFiles(void)
{

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
            ScanForStationAddress(one_line_from_xml_file);
            source_code_line++;
            std::string::size_type n;
            n = one_line_from_xml_file.find("$CCP$");
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
    return 0;
}

/*******************************************************************************************
* Function: Scan if the station address is available
******************************************************************************************/
int ECUXMLCreator::ScanForStationAddress(std::string& one_line_from_xml_file)
{

    std::string::size_type n;
    n = one_line_from_xml_file.find("$CCP_StationAddress$");
    if (n == std::string::npos) {
        //std::cout << "not found\n";
        return 0;
    } else
    {
        std::string::size_type pos_station_address_tag = one_line_from_xml_file.find("CCP_STATION_ADDRESS");
        std::string::size_type pos_start_value = one_line_from_xml_file.find_first_of("1234567890.",pos_station_address_tag);
        std::string::size_type pos_end_value = one_line_from_xml_file.find_first_not_of("1234567890.",pos_start_value);

        if( pos_start_value == std::string::npos || pos_end_value == std::string::npos)
        {
            std::cerr << "Error[-1102]:The start or the end of the string, describing the station address, could"
                      << "not be found." << std::endl;
            return -1102;
        }

        std::string str_value = one_line_from_xml_file.substr(pos_start_value,pos_end_value - pos_start_value);

        try
        {
            std::stoll(str_value);
        }
        catch(...)
        {
            std::cerr << "Error[-1104]:The string which should be the station address value could not be "
                      << "converted into a number." << std::endl;
            return -1104;
        }
        if(std::stoll(str_value) < 0 || std::stoll(str_value) > 65535)
        {
            std::cerr << "Error[-1105]:The station address is to big and will be changed to 0 "
                      << std::endl;
            return -1105;
        }
        station_address =std::stoll(str_value);
    }
    return 0;
}
/*******************************************************************************************
* Function: Scan the MAP file and try to find all the addresses
******************************************************************************************/
int ECUXMLCreator::ScanAddressMapFiles(void)
{
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
    return 0;
}

/*******************************************************************************************
* Function: Create the XML File
******************************************************************************************/
void ECUXMLCreator::CreateXMLFile(void)
{
using namespace tinyxml2;

    XMLDocument xmlDoc;
    XMLNode * pRoot = xmlDoc.NewElement("measurement_file");
    xmlDoc.InsertFirstChild(pRoot);
    /* Save the ECU build Information */
    XMLElement * p_build_information = xmlDoc.NewElement("build_information");
        pRoot->InsertEndChild(p_build_information);
    XMLElement * p_created_by = xmlDoc.NewElement("created_by");
        p_build_information->InsertEndChild(p_created_by);
        std::string tmp ="Stockente - ";
        tmp.append(git_version);
        tmp.append(" - Git-checksum:");
        tmp.append(git_checksum);
        p_created_by->SetText(tmp.c_str());
    /* Define the ECU Properties */
    XMLElement * p_ecu_properties = xmlDoc.NewElement("ecu_properties");
        pRoot->InsertEndChild(p_ecu_properties);
    XMLElement * p_endianness = xmlDoc.NewElement("endianness");
        p_ecu_properties->InsertEndChild(p_endianness);
        p_endianness->SetText(endianness.c_str());
    XMLElement * p_station_address = xmlDoc.NewElement("station_address");
        p_ecu_properties->InsertEndChild(p_station_address);
        std::stringstream ss_StationAddress;
        ss_StationAddress << station_address;
        std::string str_StationAddress = ss_StationAddress.str();
        p_station_address->SetText(str_StationAddress.c_str());
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

    eResult = xmlDoc.SaveFile(result_file_name.c_str());
}


/*******************************************************************************************
* Function: Create the XML File
******************************************************************************************/
void ECUXMLCreator::PlotResults2Terminal(void)
{
/* Start to print the results of the program execution */
    std::cout << std::endl << std::endl;
    std::cout << "------------Results------------------------------------" << std::endl;
    std::cout << "Number of CCP Variables: " << CCP_Variables.size() << std::endl;
    std::cout << "CCP_Station Address    : " << station_address << std::endl;
    std::cout << "ECU Endianness         : " << endianness << std::endl;
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
}

/*******************************************************************************************
* Function: Define the endianness
******************************************************************************************/
void ECUXMLCreator::Set_endianness(std::string str_endianness)
{
    if(str_endianness.compare("big") == 0)
    {
        endianness = "big";
        return;
    }
    if(str_endianness.compare("little") == 0)
    {
        endianness = "little";
        return;
    }
    std::cerr << "Error[-123]: Endianness flag found but the follwoing argument is not valid."
              << "Endianness will be set to little instead" << std::endl;
}

