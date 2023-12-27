import os
import platform
import subprocess
import sys
import time
from termcolor import colored


def main(*args):
    current_directory = os.getcwd()
    build_dir = os.path.join(current_directory, 'build')

    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    if platform.system() == 'Windows':
        exe = os.path.join(build_dir, 'widgets-impl.exe')
    else:
        exe = os.path.join(build_dir, 'widgets-impl')

    for arg in args:
        if arg in ['test', 'tests', 't']:
            exe = os.path.join(build_dir, 'core-tests.exe')
    
    cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + build_dir]
    cmake_args += ['-G', 'Ninja']

    for arg in args:
        if arg in ['debug-find']:
            cmake_args += '--debug-find'

    try:
        p = subprocess.run(['cmake', ".", f'-B{build_dir}', '-D', 'CMAKE_BUILD_TYPE=Debug', '-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON', '-DCMAKE_PREFIX_PATH=C:\Program Files (x86)\qcoro', '-DC=msvc', '-DCXX=msvc', '-DCMAKE_EXPORT_COMPILE_COMMANDS=1'] + cmake_args, check=True)
        p = subprocess.run(['cmake', '--build', '.'], cwd=build_dir, check=True)
        print(p.stdout, colored(p.stderr, 'red'))
        proc = subprocess.Popen([exe, *args], cwd=build_dir, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    
    except KeyboardInterrupt:
        p.terminate()    
    except Exception as e:
        print(colored(e, 'red'))
    for line in proc.stdout:
        print(line)

if __name__ == '__main__':
    args = []
    if len(sys.argv) > 1:
        args += sys.argv[1:]
        print(args)
    main(*args)
