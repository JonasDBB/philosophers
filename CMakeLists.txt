cmake_minimum_required(VERSION 3.17)
project(philosophers C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -Wall -Wextra -Werror -fsanitize=thread")
#                                       can add -fsanitize=address up here
set(CMAKE_LINKER_FLAGS)
include_directories( ${philo_one})

add_executable(philo_one philo_one/main.c philo_one/ft_atoi.c philo_one/philosophers.h)