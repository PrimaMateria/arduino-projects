#!/usr/bin/env python
from pynvim import attach
import requests
import time
import click

def get_vim_mode(socket):
    nvim = attach('socket', path=socket)
    mode = nvim.command_output("echo nvim_get_mode().mode")
    # print(mode, end=' ', flush=True)
    if "n" in mode:
        return "1"
    elif mode == "v" or mode == "V" or mode == "^V":
        return "2"
    elif "i" in mode:
        return "3"
    elif mode == "c":
        return "4"
    elif mode == "t":
        return "5"
    else:
        print(mode)
        return "0"

@click.command()
@click.option('-s', '--socket', required=True, help='socket file for calling NVIM API')
@click.option('-t', '--target', required=True, help='IP address of the vim-mode box')
@click.option('-f', '--frequency', default=0.5, help='How frequent in seconds will be the neovim socket API queried', show_default='0.5')
def main(socket, target, frequency):
    """It periodically queries NVIM API through socket file to acquire current mode and passes it via HTTP call to the vim-mode box"""
    while True:
        mode = get_vim_mode(socket)
        requests.get(target, headers={"mode":mode})
        time.sleep(frequency)

if __name__ == "__main__":
    # pylint: disable=no-value-for-parameter
    main()
