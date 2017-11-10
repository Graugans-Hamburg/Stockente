#!/bin/bash

# Enter the path root directory. This folder and all folder below will be searched
project_loc=/home/Mustermann/home/crazyprojekt/examplefolder
# Enter the path where the program Stockente has been saved
stockente_loc=/home/Mustermann/home/hellyeah/examplefolder
# Enter the path and the name of the map file containing the address information
result_loc=/home/Mustermann/home/crazyprojekt/bin/release/example.map
# Enter the path where to save the result and the name of the file (xml-file with variable information)
result_loc=/home/Mustermann/home/crazyprojekt/bin/release/CCP_DataBase.xml


find ${project_loc} -name '*.c' -o -name '*.cpp' -o -name '*.h' | xargs ${stockente_loc} -t ${result_loc} -x ${map_loc} -s


