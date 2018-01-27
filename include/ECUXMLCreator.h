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
        void Add_source_code(std::string source_file_name_full_path);
        int ScanSourceCodeFiles(void);
        int ScanAddressMapFiles(void);
        void CreateXMLFile(void);
        void PlotResults2Terminal(void);
    protected:
    private:
        std::vector<std::string> files_to_scan;
        std::vector<ECU_variable> CCP_Variables;
        std::string result_file_name;
        std::string map_file_name;
        tinyxml2::XMLError eResult;
};

#endif // ECUXMLCREATOR_H
