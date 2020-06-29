import os
import subprocess

TEST_NUM = 100
COMMAND = 'python3 ./tester/generator.py'
BASE = 1600
path = './input'
os.makedirs(path, exist_ok=True)

for i in range(TEST_NUM):
    case = BASE + i
    name = 'input_' + '{:04}'.format(case)
    subprocess.run(COMMAND + ' ' + str(case) + ' > ' + path + '/' + name, shell=True)
