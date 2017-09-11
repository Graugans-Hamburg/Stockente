#include "testscenarios.h"

/*******************************************************************************************
 *
 * Function 01
 *
 ******************************************************************************************/
int check_T01_comment (std::string& one_line_from_xml_file)
{

       /* Es beginnt die Überprüfung ob es in dieser Zeile einen folständigen Kommentar gibt.*/
        std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
        std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");

        if(    pos_start_comment == std::string::npos
            || pos_end_comment   == std::string::npos)
        {
            // Es entweder keine Zeichenfolge gefunden werden welche auf den Start oder das Ende
            // Ende eines Kommentars hinweist.
            return -101;
        }
        // Suche nach einem weiteren Kommentar in der Zeile.
        std::string::size_type pos_second_start_comment = one_line_from_xml_file.find("/*", pos_start_comment +1);
        std::string::size_type pos_second_end_comment = one_line_from_xml_file.find("*/", pos_end_comment +1);
        if(    pos_second_start_comment != std::string::npos
            || pos_second_end_comment   != std::string::npos)
        {
            // Es mehr als eine Zeichenfolge welche den Start und/oder den Stop für einen
            // eines Kommentars gefunden ("/*" "*/")
            return -102;
        }

        return 0; // Test erfolgreich

}

/*******************************************************************************************
 *
 * Function 02
 *
 ******************************************************************************************/


int check_T02_one_comment(std::string& one_line_from_xml_file)
{
 /* Es soll kontrolliert werden ob sich der Kommentar am Ende der Zeile befindet */

    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
    std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");
    size_t number_of_character = one_line_from_xml_file.length();
    for(size_t pos_idx = pos_end_comment + 2 ; pos_idx < number_of_character; pos_idx++)
    {
        if(one_line_from_xml_file.at(pos_idx) !=' ')
        {
            // Das Schlüsselwort $CCP hat hinter dem Ende des Kommentars weitere druckbare
            // Zeichen
            return -301;
        }

    }

    return 0;
}

/*******************************************************************************************
 *
 * Function 03
 *
 ******************************************************************************************/

int check_T03_semicolon(std::string& one_line_from_xml_file)
{
    /*
     * Es soll kontrolliert werden ob sich for dem Kommentarstart ein Semikolon befindet
     * und das nur maximal ein Selmikolon in der Zeile steht.
     */
    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
    std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");
    size_t pos_semicolon = one_line_from_xml_file.find(";");
    if(pos_semicolon == std::string::npos)
    {
        // Es wurde kein Semikolon in der Zeile gefunden. Die CCP-Markierung kann somit mit
        // keiner Variable in Verbindung gebracht werden.
        return -301;
    }

    // Überprüfe ob sich das Semikolon vor dem Kommentar befindet.
    if(pos_semicolon > pos_start_comment)
    {
        // Das erst Semikolon befindet sich innerhalb des Kommentares
        // Die CCP variable kann nicht weiter untersucht werden.
        return -302;
    }
    // Suche nach einem weiteren Semikolon in der Zeile.
    pos_semicolon = one_line_from_xml_file.find(";", pos_semicolon +1);
    if(pos_semicolon != std::string::npos)
    {
        // Es wurde ein weiteres Semikolon in der Zeile gefunden. Zwei Anweisungen in
        // einer Zeile sind nicht erlaubt für $CPP-Markierungen.
        return -303;
    }

    return 0;
}

/*******************************************************************************************
 *
 * Function 04 - extract the instruction
 *
 ******************************************************************************************/

int check_T04_extract_instruction(std::string& one_line_from_xml_file,std::string& instruction)
{
    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");

    instruction = one_line_from_xml_file.substr(0,pos_start_comment);

    return 0;
}

/*******************************************************************************************
 *
 * Function 05 - find the out the datatype
 *
 ******************************************************************************************/

 int check_T05_get_var_properties(std::string& instruction,ECU_variable& tmp_variable)
{
    int datatypes_found=0;
    std::string::size_type pos_tmp, pos_datatype, pos_variablename, pos_endvarname, pos_semicolon;

    pos_tmp = instruction.find(" uint8_t ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_u8);
    }

    pos_tmp = instruction.find(" int8_t ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_i8);
    }

    pos_tmp = instruction.find(" uint16_t ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_u16);
    }

    pos_tmp = instruction.find(" int16_t ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_i16);
    }

    pos_tmp = instruction.find(" uint32_t ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_u32);
    }

    pos_tmp = instruction.find(" int32_t ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_i32);
    }

    pos_tmp = instruction.find(" float ");
    if(pos_tmp != std::string::npos)
    {
        datatypes_found++;
        pos_datatype = pos_tmp;
        tmp_variable.SetDataType(type_f32);
    }

    if(datatypes_found == 0)
    {
        // Kein Datentyp wurde in der Anweisung gefunden
        return -501;
    }
     if(datatypes_found > 1)
    {
        // Zu viele Datentypen wurden in der Anweisung gefunden.
        return -502;
    }

    //Finde die Position des Leerzeichens welches nach dem Datentyp folgt.

    pos_variablename = instruction.find(" ",pos_datatype + 1);
    pos_variablename = instruction.find_first_not_of(" ",pos_variablename);
    if(pos_variablename == std::string::npos)
    {
        // Die Position des Variablennames konnte nicht in der Anweisung gefunden werden.
        return -503;
    }


    /*
     *Finde das Ende des Variablennames. Nach einem Variablennamen folgt generell entwerden:
     *
     *  ;  [  =  Leerzeichen
     */
    pos_semicolon = instruction.find(";",pos_variablename);
    if(pos_semicolon == std::string::npos)
    {
        // Kein Semikolon konnte in der Anweisung gefunden werden.
        return -504;
    }

    std::string::size_type pos_array = instruction.find("[",pos_variablename);
    if(pos_array !=  std::string::npos)
    {
        // Die CCP variable beschreibt ein Array welche im Augenblick nicht unterstützt werden.
        return -505;
    }

    std::string::size_type pos_equal = instruction.find("=",pos_variablename);
    std::string::size_type pos_whitespace = instruction.find(" ",pos_variablename);

    if(pos_semicolon < pos_array)
        pos_endvarname = pos_semicolon - 1;
    else
        pos_endvarname = pos_array - 1;

    if(pos_equal < pos_endvarname)
        pos_endvarname = pos_equal - 1;

    if(pos_whitespace < pos_endvarname)
        pos_endvarname = pos_whitespace - 1;

    if(pos_endvarname == std::string::npos)
    {
        // Das Ende des Variablennamens konnte nicht bestimmt werden.
        return -506;
    }
    if(pos_endvarname < pos_variablename)
    {
        // Das Ende des Variablennames scheint vor dem Anfang zu sein. Es gibt die Matrix!
        return -507;
    }

    tmp_variable.SetName(instruction.substr(pos_variablename,pos_endvarname-pos_variablename+1));

    return 0;
}

/*******************************************************************************************
 *
 * Function 06 - find out if the description as a min value
 *
 ******************************************************************************************/

int check_T06_extract_thres(std::string& str_comment,ECU_variable& tmp_variable, EnumSearchEle element)
{

    std::string::size_type pos_start_min = str_comment.find("min:");
    std::string::size_type pos_start_max = str_comment.find("max:");
    std::string::size_type pos_start_des = str_comment.find("des-");
    std::string::size_type pos_search_element;
    switch(element)
    {
        case min_value:
            pos_search_element = pos_start_min;
            break;
        case max_value:
            pos_search_element = pos_start_max;
            break;
        default:
            // Programmierfehler falscher Wert wurde als 3 Element übergeben.
            return -603;
    }

    if(pos_search_element == std::string::npos)
    {
        // Das Schlüsselwort min: konnte nicht gefunden werden.
        return -601;
    }
    std::string::size_type pos_start_value = str_comment.find_first_of("-1234567890.",pos_search_element);
    std::string::size_type pos_end_value = str_comment.find_first_not_of("-1234567890.",pos_start_value);

    // TODO verschiedenste Überprüfungen hinzufügen
    if( pos_start_value == std::string::npos || pos_end_value == std::string::npos)
    {
        // Der Anfang oder das Ende der Zeichenfolge, welche den Zahlenwert beschreibt konnte
        // nicht gefunden werden.
        std::cout << "Kein minimaler Wert wurde mit angegeben." << std::endl;
        return -602;
    }

    std::string str_value = str_comment.substr(pos_start_value,pos_end_value - pos_start_value);

    switch(element)
    {
        case min_value:
            tmp_variable.SetMinValue_Int(std::stoll(str_value));
            tmp_variable.SetMinValue_Fl(std::stof(str_value));
            tmp_variable.str_MinValue = str_value;
            break;
        case max_value:
            tmp_variable.SetMaxValue_Int(std::stoll(str_value));
            tmp_variable.SetMaxValue_Fl(std::stof(str_value));
            tmp_variable.str_MaxValue = str_value;
            break;
        default:
            // Programmierfehler falscher Wert wurde als 3 Element übergeben.
            return -603;
    }



    return 0;
}

/*******************************************************************************************
 *
 * Function 07 - extract the comment
 *
 ******************************************************************************************/

int check_T07_extract_comment(std::string& one_line_from_xml_file,std::string& str_comment)
{
    std::string::size_type pos_start_comment = one_line_from_xml_file.find("/*");
    std::string::size_type pos_end_comment = one_line_from_xml_file.find("*/");
    if(pos_start_comment == std::string::npos)
    {
        // Es konnte nicht die Zeichenfolge für den Start eines Kommentars gefunden werden
        return -701;
    }
    if(pos_end_comment == std::string::npos)
    {
        // Es konnte nicht die Zeichenfolge für das Ende eines Kommentars gefunden werden
        return -702;
    }
    // Die +2 in nächsten Codezeile bezwecken das auch die Zeichenfolge für das Ende des Kommentars mit aufgenommen wird
    str_comment = one_line_from_xml_file.substr(pos_start_comment,pos_end_comment-pos_start_comment+2);

    return 0;
}
