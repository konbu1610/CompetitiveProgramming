import os
import subprocess
import sys

from multiprocessing import Pool

import time
import make_input

current_path = os.getcwd()

JUDGE_COMMAND = 'python ./tester/judge.py'
TEST_NUM = [make_input.TEST_NUM//10, make_input.TEST_NUM//2, make_input.TEST_NUM]

args = sys.argv

exe_name = args[1]
test_name = args[2]
test_kind = 0
if len(args) == 4:
    test_kind = int(args[3])

exe_path = current_path + '/exe/'+exe_name
out_path = current_path + '/test/'+exe_name+'_'+test_name

if test_kind == -1:
    seed = make_input.BASE
    input_file = current_path + '/input/input_'+'{:04}'.format(seed)
    output_file = current_path + '/tmp'
    subprocess.run(exe_path + ' < ' + input_file + ' > ' + output_file, shell = True)
    res = subprocess.run(JUDGE_COMMAND + ' ' + input_file + ' ' + output_file, shell = True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    try:
        res_f = float(res.stdout.decode('utf-8'))
    except:
        res_f = 0
        print("error "+ str(seed) + '!!', res, sys.exc_info()[0])
    print(res_f)
    quit()

def run(seed):
    print('start seed:' + str(seed))
    input_file = current_path + '/input/input_'+'{:04}'.format(seed)
    output_file = out_path + '/output/output_'+'{:04}'.format(seed)
    subprocess.run(exe_path + ' < ' + input_file + ' > ' + output_file, shell = True)
    res = subprocess.run(JUDGE_COMMAND + ' ' + input_file + ' ' + output_file, shell = True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    # print(type(res.stdout))
    try:
        res_f = float(res.stdout.decode('utf-8'))
    except:
        res_f = 0
        print("error "+ str(seed) + '!!', res, sys.exc_info()[0])
    return (res_f, seed)

def write_result(res):
    result_path = out_path + '/result'
    with open(result_path, mode='w') as f:
        f.write('seed: {0} - {1}'.format(make_input.BASE, make_input.BASE+TEST_NUM[test_kind])+'\n')
        f.write('average: ' + str(ave) + '\n')
        f.write('minScores\n')
        for i in range(0, 3):
            f.write('seed:{0[1]} score:{0[0]}\n'.format(res[i]))
        f.write('maxScores\n')
        for i in range(len(res)-1, len(res)-4, -1):
            f.write('seed:{0[1]} score:{0[0]}\n'.format(res[i]))

if __name__ == "__main__":
    print('makedirs')
    os.makedirs(out_path)
    os.makedirs(out_path+'/output')

    print('start run')
    start = time.time()

    p = Pool(32) # プロセス数をxに設定
    res = p.map(run, range(make_input.BASE, make_input.BASE+TEST_NUM[test_kind])) 
    res.sort()

    ave = 0.0
    for score, seed in res:
        ave += score / TEST_NUM[test_kind]
    write_result(res)

    elapsed_time = time.time() - start
    print('average: {}'.format(ave))
    print ("elapsed_time:{0}".format(elapsed_time) + "[sec]")
