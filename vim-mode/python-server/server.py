#!/usr/bin/env python
from pynvim import attach
import requests
import time

def vimmode():
    nvim = attach('socket', path='/tmp/nvim')
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
        print(mode);
        return "0"

def main():
    while True:
        mode = vimmode()
        requests.get("http://192.168.178.78", headers={"mode":mode});
        time.sleep(0.5)

if __name__ == "__main__":
    main()
