/**
 * [ M E G A D E V ]   a Sega Mega CD devkit
 *
 * @file cd_header.s
 * @brief Mega CD software header
 * 
 * @warning
 * Do not change the order of the defines in this file!
 */

#define STRINGIFY(x) #x

#define STR(x) XSTR(x)
#define XSTR(x) #x


#ifndef PROJECT_NAME_DOMESTIC
#error PROJECT_NAME_DOMESTIC not defined!
#endif


