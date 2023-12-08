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
        exe = os.path.join(build_dir, 'gitlab-downloader.exe')
    else:
        exe = os.path.join(build_dir, 'gitlab-downloader')

    cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + build_dir]
    cmake_args += ['-G', 'Ninja']
    
    try:
        p = subprocess.run(['cmake', "../src/app/"] + cmake_args, cwd=build_dir, check=True)
        p = subprocess.run(['cmake', '--build', '.'], cwd=build_dir, check=True)
        #p = subprocess.run([exe, *args], cwd=build_dir, capture_output=True, text=True)
        #proc = subprocess.Popen([exe, *args], stdin = subprocess.PIPE, stdout=subprocess.PIPE,universal_newlines=True, shell=True)
        #stdout, stderr = proc.communicate(str.encode(' '.join(exe,*args)))
        #print(stdout, stderr)

        #print(p.stdout, colored(p.stderr, 'red'))
    except KeyboardInterrupt:
        p.terminate()    
    except Exception as e:
        print(colored(e, 'red'))

if __name__ == '__main__':
    args = []
    if len(sys.argv) > 1:
        args += sys.argv[1:]
    main(*args)
