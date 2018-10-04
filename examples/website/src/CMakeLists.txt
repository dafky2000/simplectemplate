cmake_minimum_required(VERSION 3.2)

set (PROJECT "simplecgen")

add_definitions("-DPACKAGE_BUGREPORT=\"andy400-dev@yahoo.com\"")

add_definitions("-DVERSION=\"0.1.02-dev\"")

add_executable(${PROJECT} simplecgen.c utils.c template_functions.c)
