#ifndef ECUXMLCREATOR_H
#define ECUXMLCREATOR_H

#include <iostream>
#include <vector>
#include "../ECU_variable.h"
#include "../scan_source_files.h"
#include "../tinyxml2.h"


class ECUXMLCreator
{
    public:
        ECUXMLCreator();
        virtual ~ECUXMLCreator();
        void Set_result_file_name(std::string tmp){result_file_name = tmp;};
        void Set_map_file_name(std::string tmp){map_file_name = tmp;};
        void Set_endianness(std::string str_endianess);
        void Add_source_code(std::string source_file_name_full_path);
        int ScanSourceCodeFiles(void);
        int ScanAddressMapFiles(void);
        int ScanForTwinsAndRemove(void);
        int CheckForVariablesWithoutAddress(void);
        int ScanForStationAddress(std::string& one_line_from_xml_file);
        void CreateXMLFile(void);
        void PlotResults2Terminal(void);
        std::string git_version;
        std::string git_checksum;
    protected:
    private:
        std::vector<std::string> files_to_scan;
        std::vector<ECU_variable> CCP_Variables;
        std::string result_file_name;
        std::string map_file_name;
        std::string endianness;
        int station_address;
        tinyxml2::XMLError eResult;

        //Log warning
        int number_of_twins;
        int number_of_homeless;

};

#endif // ECUXMLCREATOR_H
