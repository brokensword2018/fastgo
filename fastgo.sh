#!/bin/bash


install_path=$HOME/.fastgo
export PATH=~/.fastgo:"${PATH}"

go() {
    $install_path/fastgo
    path=$(cat  $install_path/ret_path.txt)

    if [[ -d "${path}" ]]; then
        if [ -t 1 ]; then  # if stdout is a terminal, use colors
            echo -e "\\033[31m${path}\\033[0m"
        else
            echo -e "${path}"
        fi
        cd "${path}"
    else
        if [ -n "$path" ]; then
            echo $path does not exist, use 'fastgo -l' to reload path
        fi
    fi 
}