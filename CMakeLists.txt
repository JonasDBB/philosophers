cmake_minimum_required(VERSION 3.17)
project(philosophers C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
#                                       can add -fsanitize=address up here
set(CMAKE_LINKER_FLAGS)
include_directories( ${philo_one})

add_executable(philo_one philo_one/main.c philo_one/ft_helper_functions.c philo_one/philosophers.h philo_one/cleanup.c philo_one/setup.c philo_one/philo_loop.c philo_one/locked_writing.c)

add_executable(philo_two philo_two/main.c philo_two/ft_helper_functions.c philo_two/philosophers.h philo_two/cleanup.c philo_two/setup.c philo_two/philo_loop.c philo_two/locked_writing.c)